// opencv_samples_cpp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include<opencv2/highgui.hpp>
//#include "camshift_demo.hpp"
//#include "lk_demo.hpp"
#include "objdetect_demo.hpp"

int main(int argc, char** argv)
{
	std::cout << "Hello World!\n";

	//samples::camshift::test(argc, argv);
	//samples::lk::test(argc, argv);
	samples::objdetect::test(argc, argv);

	//cv::waitKey(0);
}
