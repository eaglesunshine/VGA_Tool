#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VGA_Tool.h"
#include <string>
#include <time.h>
#include "visual_revealing.h"
#include "visual_analysis.h"

class VGA_Tool : public QMainWindow
{
	Q_OBJECT

public:
	VGA_Tool(QWidget *parent = Q_NULLPTR);

	QString qs_AccessLinks_file, qs_VisibilityLinks_file, qs_Visibility_vga_file, qs_output_file_name, qs_FromStr;
	std::string AccessLinks_file, VisibilityLinks_file, Visibility_vga_file, output_file_name, FromStr;

	clock_t start, finish;

private slots:
	// import files
	void on_pushButton_clicked();	
	void on_pushButton_2_clicked();

	void on_pushButton_4_clicked();
	void on_pushButton_7_clicked();
	void on_pushButton_5_clicked();

	void on_pushButton_8_clicked();
	void on_pushButton_11_clicked();

	// calculate
	void on_pushButton_3_clicked();		// visual_revealing
	void on_pushButton_6_clicked();		// visual_analysis
	void on_pushButton_10_clicked();	// View and Viewed Analysis
private:
	Ui::VGA_Tool *ui;
};
