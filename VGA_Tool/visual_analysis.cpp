#include "visual_analysis.h"

Analysis::Analysis(string AccessLinks_file, string VisibilityLinks_file, string Visibility_vga_file, string output_file_name)
{
	this->AccessLinksFile = AccessLinks_file;
	this->VisibilityLinksFile = VisibilityLinks_file;
	this->Visibility_vgaFile = Visibility_vga_file;
	this->output_file_name = output_file_name;

	char str[500];
	strncpy(str, AccessLinks_file.c_str(), AccessLinks_file.length() + 1);
	char *ptr = strrchr(str, '/');
	string tmp = (ptr + 1);
	this->input_directory = AccessLinks_file.substr(0, AccessLinks_file.length() - tmp.length());

	this->outputFile = this->input_directory + this->output_file_name;
}

void Analysis::getLinksMap(map<int, set<int>> &LinksMap, string fileName)
{
	LinksMap.clear();

	ifstream inFile(fileName, ios::in);		// 读文件
	string lineStr;

	int count = 0;
	while (getline(inFile, lineStr))
	{
		++count;
		if (count == 1)		//跳过第一行
			continue;

		stringstream ss(lineStr);
		string str;
		int from_id = 0, to_id = 0;

		int subcount = 0;
		while (getline(ss, str, ','))	// 按照逗号分隔,读取一行
		{
			if (subcount == 0)
			{
				from_id = atoi(str.c_str());
				++subcount;
			}
			else
			{
				to_id = atoi(str.c_str());
			}
		}

		LinksMap[from_id].insert(to_id);
		LinksMap[to_id].insert(from_id);
	}
}

string Analysis::getFirstLine(string fileName)
{
	string firstLine;
	ifstream inFile(fileName, ios::in);		// 读文件
	string lineStr;

	while (getline(inFile, lineStr))
	{
		return lineStr;
	}
}

void Analysis::getVisibility_vga(map<int, string> &Visibility_vga, map<int, int> &ConnectivityData, string fileName)
{
	Visibility_vga.clear();
	ConnectivityData.clear();

	ifstream inFile(fileName, ios::in);		// 读文件
	string lineStr;

	int count = 0;
	while (getline(inFile, lineStr))
	{
		++count;
		if (count == 1)		//第一行
			continue;

		stringstream ss(lineStr);
		string str;
		int id = 0;

		int subcount = 0;
		while (getline(ss, str, ','))	// 按照逗号分隔,读取一行
		{
			if (subcount == 0)
			{
				id = atoi(str.c_str());
			}
			if (subcount == 3)
			{
				ConnectivityData[id] = atoi(str.c_str());
				break;
			}
			++subcount;
		}

		Visibility_vga[id] = lineStr;
	}
}

void Analysis::output(map<int, string> &Visibility_vga, map<int, int> &ConnectivityData, map<int, int> &Result, string firstLine, string fileName)
{
	ofstream out;
	out.open(fileName);

	//写入第一行
	firstLine = firstLine + ",accessible_views,added_views";
	out << firstLine << endl;

	for (auto it = Visibility_vga.begin(); it != Visibility_vga.end(); it++)
	{
		int id = it->first;
		//cout << Result[id] << "," << Result[id] - ConnectivityData[id] << endl;
		out << Visibility_vga[id] << "," << Result[id] << "," << Result[id] - ConnectivityData[id] << endl;
	}
	out.close();
}

void Analysis::outputResult(map<int, double> &viewResult, map<int, double> &viewedResult, string fileName)
{
	ofstream out;
	out.open(fileName);

	//写入第一行
	out <<"startID,viewCount,viewedCount" << endl;

	for (auto it = viewResult.begin(); it != viewResult.end(); it++)
	{
		int id = it->first;
		out << id << "," << viewResult[id] << "," << viewedResult[id]<< endl;
	}
	out.close();
}

void getIDset(set<int> &IDset, string FromStr)
{
	IDset.clear();

	stringstream ss(FromStr);
	string str;

	while (getline(ss, str, ','))	// 按照逗号分隔,读取一行
	{
		IDset.insert(atoi(str.c_str()));
	}
}

void Analysis::Run()
{
	//读取文件数据
	getLinksMap(AccessLinks, this->AccessLinksFile);
	getLinksMap(VisibilityLinks, this->VisibilityLinksFile);
	getVisibility_vga(Visibility_vga, ConnectivityData, this->Visibility_vgaFile);

	//计算一次拓扑的可见栅格数目
	set<int> validLinks;
	map<int, int> Result;
	for (auto it = AccessLinks.begin(); it != AccessLinks.end(); it++)
	{
		validLinks.clear();
		int id = it->first;

		//当前栅格可见域
		validLinks.insert(VisibilityLinks[id].begin(), VisibilityLinks[id].end());

		//一次拓扑可见域
		int to_id = 0;
		for (auto iter = AccessLinks[id].begin(); iter != AccessLinks[id].end(); iter++)
		{
			to_id = *iter;
			validLinks.insert(VisibilityLinks[to_id].begin(), VisibilityLinks[to_id].end());
		}

		Result[id] = int(validLinks.size());
	}

	//输出结果文件
	string firstLine = getFirstLine(this->Visibility_vgaFile);
	output(Visibility_vga, ConnectivityData, Result, firstLine, this->outputFile);
}

void Analysis::analysis(string FromStr)
{
	//获取起点id
	set<int> IDset;
	getIDset(IDset, FromStr);

	//读取文件数据
	getLinksMap(AccessLinks, this->AccessLinksFile);
	getLinksMap(VisibilityLinks, this->VisibilityLinksFile);

	//view analysis：找出当前栅格一次拓扑能够到达的所有栅格，再根据这些栅格找出它们可以看见的所有栅格
	set<int> validLinks;	//所有可见域
	set<int> accessPoints;	//所有可达域
	set<int> searchedIDs;

	map<int, double> viewResult;
	map<int, map<int, int>> viewCountMap;
	map<int, int> subViewCountMap;
	for (auto it = IDset.begin(); it != IDset.end(); it++)
	{
		int startID = *it;
		searchedIDs.clear();
		validLinks.clear();
		accessPoints.clear();
		accessPoints.insert(startID);

		int viewCount = 0;
		subViewCountMap.clear();
		while (validLinks.size() < VisibilityLinks.size())
		{
			//1次拓扑
			set<int> accessSet;
			for (auto it = accessPoints.begin(); it != accessPoints.end(); it++)
			{				
				int id = *it;

				//如果是已经被检索过的ID，就跳出
				if (searchedIDs.count(id) == 1)
					continue;

				//当前栅格可见域
				validLinks.insert(VisibilityLinks[id].begin(), VisibilityLinks[id].end());
				searchedIDs.insert(id);

				//一次拓扑可达的栅格
				accessSet.insert(AccessLinks[id].begin(), AccessLinks[id].end());
			}
			++viewCount;
			subViewCountMap[viewCount] = int(accessPoints.size());

			accessPoints.clear();
			accessPoints.insert(accessSet.begin(), accessSet.end());
			
		}

		viewCountMap.insert(make_pair(startID, subViewCountMap));

		double sum = 0, subsum = 0;
		for (auto iter = subViewCountMap.begin(); iter != subViewCountMap.end(); iter++)
		{
			subsum += double(iter->second);
			sum += double(iter->first)*double(iter->second);
		}
		double result = sum / subsum;
		viewResult.insert(make_pair(startID, result));
	}
	
	//viewed analysis
	set<int> viewPoints;
	map<int, double> viewedResult;
	map<int, map<int, int>> viewedCountMap;
	map<int, int> subViewedCountMap;
	for (auto it = IDset.begin(); it != IDset.end(); it++)
	{
		int startID = *it;
		searchedIDs.clear();
		validLinks.clear();
		viewPoints.clear();
		viewPoints.insert(startID);

		int viewCount = 0;
		subViewedCountMap.clear();
		while (validLinks.size() < VisibilityLinks.size())
		{
			//1次拓扑
			set<int> viewSet;
			for (auto it = viewPoints.begin(); it != viewPoints.end(); it++)
			{
				int id = *it;

				//如果是已经被检索过的ID，就跳出
				if (searchedIDs.count(id) == 1)
					continue;

				//当前栅格可见域
				validLinks.insert(VisibilityLinks[id].begin(), VisibilityLinks[id].end());
				searchedIDs.insert(id);

				//一次拓扑可视的栅格
				viewSet.insert(VisibilityLinks[id].begin(), VisibilityLinks[id].end());
			}
			++viewCount;
			subViewedCountMap[viewCount] = int(viewPoints.size());

			viewPoints.clear();
			viewPoints.insert(viewSet.begin(), viewSet.end());
		}

		viewedCountMap.insert(make_pair(startID, subViewedCountMap));

		double sum = 0, subsum = 0;
		for (auto iter = subViewedCountMap.begin(); iter != subViewedCountMap.end(); iter++)
		{
			subsum += double(iter->second);
			sum += double(iter->first)*double(iter->second);
		}
		double result = sum / subsum;
		viewedResult.insert(make_pair(startID, result));
	}

	//输出结果
	outputResult(viewResult, viewedResult, outputFile);
}