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

void Analysis::Start()
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