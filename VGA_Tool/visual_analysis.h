#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
#include <iomanip>
#include <time.h>

using namespace std;

class Analysis
{
public:
	Analysis(string AccessLinks_file, string VisibilityLinks_file, string Visibility_vga_file, string output_file_name);

	//ÎÄ¼þÂ·¾¶
	string AccessLinksFile, VisibilityLinksFile, Visibility_vgaFile, output_file_name;
	string input_directory, outputFile;

	map<int, set<int>> AccessLinks;
	map<int, set<int>> VisibilityLinks;
	map<int, string> Visibility_vga;
	map<int, int> ConnectivityData;

	void getLinksMap(map<int, set<int>> &LinksMap, string fileName);
	string getFirstLine(string fileName);
	void getVisibility_vga(map<int, string> &Visibility_vga, map<int, int> &ConnectivityData, string fileName);
	void output(map<int, string> &Visibility_vga, map<int, int> &ConnectivityData, map<int, int> &Result, string firstLine, string fileName);
	void outputResult(map<int, double> &viewResult, map<int, double> &viewedResult, string fileName);

	void Run();
	void analysis(string FromStr);
};