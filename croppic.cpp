// croppic.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgcodecs.hpp>

// Include Libraries
#include<opencv2/opencv.hpp>
#include <string>
#include <iostream>
#include <filesystem>
#include <opencv2\core\core.hpp>

using namespace std;
using namespace cv;
namespace fs = std::filesystem;


// using AND_CMD like an operator to concat commands -- you can also just type it in one string: "edit & shutdown -h"...
// this works in c, too, but no dynamic variables are possible
//#define AND_CMD " & "
// c++
#define AND_CMD +" & "+

int main(int argc, char** argv)
{
	std::cout << "Have " << argc << " arguments:\n";
	for (int i = 0; i < argc; ++i) {
		std::cout << argv[i] << "\n";
	}
	if (argv[1] == NULL) {
		cout << "Quen ten file roi thg lz main world mau S!";
		return 0;
	}
	if (argv[2] == NULL) {
		cout << "Nhap cai do dai vo";
		return 0;
	}
	if (argv[3] == NULL) {
		cout << "Nhap cai chieu cao (tinh tu duoi len) vo";
		return 0;
	}
	std::istringstream wid(argv[2]);
	int input_width;
	std::istringstream hei(argv[3]);
	int input_height;
	wid >> input_width;
	hei >> input_height;


	//Get files in folder
	std::string cwd = std::filesystem::current_path().string();
	std::string file_path = cwd + "/" + argv[1];
	size_t lastindex = file_path.find_last_of(".");
	string rawname = file_path.substr(0, lastindex);
	std::string folder_path = rawname;
	if (!fs::is_directory(folder_path ) || !fs::exists(folder_path)) { // Check if src folder exists
		fs::create_directory(folder_path ); // create src folder
	}
	if (!fs::is_directory(folder_path + "_cropped") || !fs::exists(folder_path + "_cropped")) { // Check if src folder exists
		fs::create_directory(folder_path + "_cropped"); // create src folder
	}
	string executeffmpeg = "chcp 65001 & @echo off & ffmpeg.exe -i \"" +string( argv[1]) + "\" -vf fps=2 \"" + rawname + "\\out%04d.png\"";
	cout << executeffmpeg;

	const char* executeff = executeffmpeg.c_str();
	system(executeff);

	if (!fs::is_regular_file(file_path) || !fs::exists(file_path)) {
		cout << file_path<<"\n";
		cout << "Khong co file ten nhu the trong folder thg dau buoi nay!!";
		return 0;
	}

	size_t lastindexOfFile = string(argv[1]).find_last_of(".");
	string nameonly =string(argv[1]).substr(0, lastindexOfFile);
	cout << rawname<<"dasaadaaa"<<endl;
	for (const auto& entry : fs::directory_iterator(rawname)) {
		std::cout << entry.path() << std::endl;
		// Read image
		Mat img = cv::imread(entry.path().string());
		int img_org_width = img.size().width;
		int img_org_height = img.size().height;
		cout << "Width : " << img_org_width << endl;
		cout << "Height: " << img_org_height << endl;
		cout << "input Width : " << input_width << endl;
		cout << "input Height: " << input_height << endl;
		Rect myROI(0, img_org_height - input_height, input_width, input_height);
		Mat cropped_image = img(myROI);
		cout << nameonly + "_cropped/" + entry.path().filename().string() << endl;
		cv::imwrite(nameonly + "_cropped/" + entry.path().filename().string(), cropped_image);
	}
	string executepyocr = "pyocr.exe --folder " + nameonly + "_cropped --image out0064.png";
	cout << executepyocr;
	const char* executeocr = executepyocr.c_str();
	system(executeocr);

	//// Read image
	//Mat img = cv::imread("test.png");
	//int img_org_width = img.size().width;
	//int img_org_height = img.size().height;

	//cout << "Width : " << img_org_width << endl;
	//cout << "Height: " << img_org_height << endl;
	//cout << "Channels: :" << img.channels() << endl;
	//// Crop image
	//Rect myROI(0, img_org_height-300, img_org_width, 300);
	//Mat cropped_image = img(myROI);

	////display image
	////cv::imshow(" Original Image", img);
	////cv::imshow("Cropped Image", cropped_image);

	////Save the cropped Image
	//cv::imwrite("Cropped Image.png", cropped_image);

	//// 0 means loop infinitely
	waitKey(0);
	//destroyAllWindows();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
