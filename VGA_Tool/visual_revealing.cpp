#include "visual_revealing.h"

Reveal::Reveal(string AccessLinks_file, string Visibility_vga_file, string output_file_name)
{
	this->AccessLinksFile = AccessLinks_file;
	this->Visibility_vgaFile = Visibility_vga_file;
	this->output_file_name = output_file_name;

	char str[500];
	strncpy(str, AccessLinks_file.c_str(), AccessLinks_file.length() + 1);
	char *ptr= strrchr(str, '/');
	string tmp = (ptr+1);
	this->input_directory = AccessLinks_file.substr(0, AccessLinks_file.length() - tmp.length());

	this->outputFile = this->input_directory + this->output_file_name;
}

void Reveal::getAccessLinks(map<int, set<int>> &AccessLinks, string fileName)
{
	AccessLinks.clear();

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

		AccessLinks[from_id].insert(to_id);
		AccessLinks[to_id].insert(from_id);
	}
}

string Reveal::getFirstLine(string fileName)
{
	string firstLine;
	ifstream inFile(fileName, ios::in);		// 读文件
	string lineStr;

	while (getline(inFile, lineStr))
	{
		return lineStr;
	}
}

void Reveal::getVisibility_vga(map<int, map<string, double>> &Visibility_vga, string fileName)
{
	Visibility_vga.clear();

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
		int id = 0;

		int subcount = 0;
		while (getline(ss, str, ','))	// 按照逗号分隔,读取一行
		{
			switch (subcount)
			{
			case 0:id = atoi(str.c_str()); break;
			case 1: Visibility_vga[id]["x"] = atof(str.c_str()); break;
			case 2: Visibility_vga[id]["y"] = atof(str.c_str()); break;
			case 3: Visibility_vga[id]["Connectivity"] = atof(str.c_str()); break;
			case 4: Visibility_vga[id]["Isovist Area"] = atof(str.c_str()); break;
			case 5: Visibility_vga[id]["Isovist Compactness"] = atof(str.c_str()); break;
			case 6: Visibility_vga[id]["Isovist Drift Angle"] = atof(str.c_str()); break;
			case 7: Visibility_vga[id]["Isovist Drift Magnitude"] = atof(str.c_str()); break;
			case 8: Visibility_vga[id]["Isovist Max Radial"] = atof(str.c_str()); break;
			case 9: Visibility_vga[id]["Isovist Min Radial"] = atof(str.c_str()); break;
			case 10: Visibility_vga[id]["Isovist Occlusivity"] = atof(str.c_str()); break;
			case 11: Visibility_vga[id]["Isovist Perimeter"] = atof(str.c_str()); break;
			case 12: Visibility_vga[id]["Point First Moment"] = atof(str.c_str()); break;
			case 13: Visibility_vga[id]["Point Second Moment"] = atof(str.c_str()); break;
			default: break;
			}

			++subcount;
		}
	}
}

void Reveal::output(map<int, map<string, double>> &Visibility_vga, string firstLine, string fileName)
{
	ofstream out;
	out.open(fileName);

	//写入第一行
	firstLine = firstLine + ",revealing_conn_mean,revealing_conn_std,revealing_area_mean,revealing_area_std";
	out << firstLine << endl;

	for (auto it = Visibility_vga.begin(); it != Visibility_vga.end(); it++)
	{
		int id = it->first;

		out << id << "," << setprecision(16) << setprecision(16) << setprecision(16) << Visibility_vga[id]["x"] << "," << setprecision(16) << Visibility_vga[id]["y"] << "," << setprecision(16) << Visibility_vga[id]["Connectivity"] << "," << setprecision(16)
			<< Visibility_vga[id]["Isovist Area"] << "," << setprecision(16) << Visibility_vga[id]["Isovist Compactness"] << "," << setprecision(16) << Visibility_vga[id]["Isovist Drift Angle"] << "," << setprecision(16)
			<< Visibility_vga[id]["Isovist Drift Magnitude"] << "," << setprecision(16) << Visibility_vga[id]["Isovist Max Radial"] << "," << setprecision(16) << Visibility_vga[id]["Isovist Min Radial"] << "," << setprecision(16)
			<< Visibility_vga[id]["Isovist Occlusivity"] << "," << setprecision(16) << Visibility_vga[id]["Isovist Perimeter"] << "," << setprecision(16) << Visibility_vga[id]["Point First Moment"] << "," << setprecision(16)
			<< Visibility_vga[id]["Point Second Moment"] << "," << setprecision(16) << Visibility_vga[id]["revealing_conn_mean"] << "," << setprecision(16) << Visibility_vga[id]["revealing_conn_std"] << "," << setprecision(16)
			<< Visibility_vga[id]["revealing_area_mean"] << "," << setprecision(16) << Visibility_vga[id]["revealing_area_std"] << endl;
	}
	out.close();
}

inline void Reveal::calculate(vector<double> &resultSet, double &mean, double &stdev)
{
	double sum = std::accumulate(std::begin(resultSet), std::end(resultSet), 0.0);
	//cout << "sum = " << setprecision(16) << sum << endl;
	mean = sum / double(resultSet.size()); //均值

	double accum = 0.0;
	std::for_each(std::begin(resultSet), std::end(resultSet), [&](const double d) {
		accum += (d - mean)*(d - mean);
	});

	stdev = sqrt(accum / double(resultSet.size() - 1)); //方差
}

void Reveal::Start()
{
	//读取文件数据
	getAccessLinks(AccessLinks, AccessLinksFile);
	getVisibility_vga(Visibility_vga, Visibility_vgaFile);

	vector<double> ConnectivityAll;
	vector<double> IsovistAreaAll;
	for (auto it = AccessLinks.begin(); it != AccessLinks.end(); it++)
	{
		int id = it->first;

		ConnectivityAll.clear();
		IsovistAreaAll.clear();

		for (auto iter = it->second.begin(); iter != it->second.end(); iter++)
		{
			int to_id = *iter;
			ConnectivityAll.push_back(Visibility_vga[to_id]["Connectivity"]);
			IsovistAreaAll.push_back(Visibility_vga[to_id]["Isovist Area"]);
		}

		//计算
		double conn_mean = 0, conn_std = 0, iso_area_mean = 0, iso_area_std = 0;
		calculate(ConnectivityAll, conn_mean, conn_std);
		calculate(IsovistAreaAll, iso_area_mean, iso_area_std);

		//存储
		Visibility_vga[id]["revealing_conn_mean"] = conn_mean;
		Visibility_vga[id]["revealing_conn_std"] = conn_std;
		Visibility_vga[id]["revealing_area_mean"] = iso_area_mean;
		Visibility_vga[id]["revealing_area_std"] = iso_area_std;
	}

	//输出结果文件
	string firstLine = getFirstLine(Visibility_vgaFile);
	output(Visibility_vga, firstLine, outputFile);
}