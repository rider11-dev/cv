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

	cout << endl;//Ϊ��ˢ������ض��򻺳���������д���ļ�������
	return 0;
}