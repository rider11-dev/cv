//#include "hello.hpp"
#include <iostream>
#include<opencv2/core/utils/logger.hpp>
#include "car_detect.hpp"
using namespace std;
using namespace cv::utils;
int main(int argc, char** argv) {
	logging::setLogLevel(logging::LOG_LEVEL_VERBOSE);
	//test::hello::test();
	car_detect::detect(argc, argv);

	cout << endl;//为了刷新输出重定向缓冲区，避免写入文件后不完整
	return 0;
}