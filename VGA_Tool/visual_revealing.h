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

class Reveal
{
public:
	Reveal(string AccessLinks_file, string Visibility_vga_file, string output_file_name);

	//ÎÄ¼þÂ·¾¶
	string AccessLinksFile, Visibility_vgaFile, output_file_name;
	string input_directory, outputFile;

	map<int, set<int>> AccessLinks;
	map<int, map<string, double>> Visibility_vga;

	void getAccessLinks(map<int, set<int>> &AccessLinks, string fileName);
	void getVisibility_vga(map<int, map<string, double>> &Visibility_vga, string fileName);
	void output(map<int, map<string, double>> &Visibility_vga, string firstLine, string fileName);
	string getFirstLine(string fileName);

	void Run();
	inline void calculate(vector<double> &resultSet, double &mean, double &stdev);
};