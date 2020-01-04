#include "VGA_Tool.h"
#include <QFile>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <string>
#include <qstring>
#include <windows.h>
#include <thread>
#include <regex>
#include <iostream>
#include <fstream>

VGA_Tool::VGA_Tool(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::VGA_Tool;
	ui->setupUi(this);
}

void VGA_Tool::on_pushButton_clicked()
{
	ui->lineEdit->clear();

	this->qs_AccessLinks_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_AccessLinks_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit->setText(this->qs_AccessLinks_file);
	}
}

void VGA_Tool::on_pushButton_2_clicked()
{
	ui->lineEdit_2->clear();

	this->qs_Visibility_vga_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_Visibility_vga_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit_2->setText(this->qs_Visibility_vga_file);
	}
}

void VGA_Tool::on_pushButton_4_clicked()
{
	ui->lineEdit_4->clear();

	this->qs_AccessLinks_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_AccessLinks_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit_4->setText(this->qs_AccessLinks_file);
	}
}

void VGA_Tool::on_pushButton_7_clicked()
{
	ui->lineEdit_7->clear();

	this->qs_VisibilityLinks_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_VisibilityLinks_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit_7->setText(this->qs_AccessLinks_file);
	}
}

void VGA_Tool::on_pushButton_5_clicked()
{
	ui->lineEdit_5->clear();

	this->qs_Visibility_vga_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_Visibility_vga_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit_5->setText(this->qs_Visibility_vga_file);
	}
}

void VGA_Tool::on_pushButton_8_clicked()
{
	ui->lineEdit_8->clear();

	this->qs_AccessLinks_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_AccessLinks_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit_8->setText(this->qs_AccessLinks_file);
	}
}

void VGA_Tool::on_pushButton_11_clicked()
{
	ui->lineEdit_11->clear();

	this->qs_VisibilityLinks_file = QFileDialog::getOpenFileName(
		this, tr("open csv file"),
		"./", tr("CSV files(*.csv);;All files (*.*)"));

	if (this->qs_VisibilityLinks_file.isEmpty())
	{
		QMessageBox mesg;
		mesg.warning(this, "warning", "fail to open the file!");
		return;
	}
	else
	{
		ui->lineEdit_11->setText(this->qs_AccessLinks_file);
	}
}

// calculate
void VGA_Tool::on_pushButton_3_clicked()
{
	this->qs_output_file_name = ui->lineEdit_3->text();

	this->AccessLinks_file = std::string((const char *)this->qs_AccessLinks_file.toLocal8Bit());
	this->Visibility_vga_file = std::string((const char *)this->qs_Visibility_vga_file.toLocal8Bit());
	this->output_file_name = std::string((const char *)this->qs_output_file_name.toLocal8Bit());

	Reveal Reveal(this->AccessLinks_file, this->Visibility_vga_file, this->output_file_name);
	
	start = clock();  // 开始时间
	this->setWindowTitle(QString("VGA_Tool----Runing"));

	Reveal.Run();
	
	finish = clock();   //结束时间
	this->setWindowTitle(QString("VGA_Tool"));
	std::string str = "Run time is " + std::to_string(double(finish - start) / CLOCKS_PER_SEC) + "s.";
	QMessageBox::information(NULL, "Run Time", str.c_str());

}

void VGA_Tool::on_pushButton_6_clicked()
{
	this->qs_output_file_name = ui->lineEdit_6->text();

	this->AccessLinks_file = std::string((const char *)this->qs_AccessLinks_file.toLocal8Bit());
	this->VisibilityLinks_file = std::string((const char *)this->qs_VisibilityLinks_file.toLocal8Bit());
	this->Visibility_vga_file = std::string((const char *)this->qs_Visibility_vga_file.toLocal8Bit());
	this->output_file_name = std::string((const char *)this->qs_output_file_name.toLocal8Bit());

	Analysis Analysis(this->AccessLinks_file, this->VisibilityLinks_file, this->Visibility_vga_file, this->output_file_name);

	start = clock();  // 开始时间
	this->setWindowTitle(QString("VGA_Tool----Runing"));
	
	Analysis.Run();
	
	finish = clock();   //结束时间
	this->setWindowTitle(QString("VGA_Tool"));
	std::string str = "Run time is " + std::to_string(double(finish - start) / CLOCKS_PER_SEC) + "s.";
	QMessageBox::information(NULL, "Run Time", str.c_str());

}

void VGA_Tool::on_pushButton_10_clicked()
{
	this->qs_output_file_name = ui->lineEdit_10->text();
	this->qs_FromStr = ui->lineEdit_9->text();

	this->AccessLinks_file = std::string((const char *)this->qs_AccessLinks_file.toLocal8Bit());
	this->VisibilityLinks_file = std::string((const char *)this->qs_VisibilityLinks_file.toLocal8Bit());
	this->Visibility_vga_file = std::string((const char *)this->qs_Visibility_vga_file.toLocal8Bit());
	this->output_file_name = std::string((const char *)this->qs_output_file_name.toLocal8Bit());
	this->FromStr = std::string((const char *)this->qs_FromStr.toLocal8Bit());

	Analysis Analysis(this->AccessLinks_file, this->VisibilityLinks_file, this->Visibility_vga_file, this->output_file_name);

	start = clock();  // 开始时间
	this->setWindowTitle(QString("VGA_Tool----Runing"));

	Analysis.analysis(this->FromStr);

	finish = clock();   //结束时间
	this->setWindowTitle(QString("VGA_Tool"));
	std::string str = "Run time is " + std::to_string(double(finish - start) / CLOCKS_PER_SEC) + "s.";
	QMessageBox::information(NULL, "Run Time", str.c_str());
}
