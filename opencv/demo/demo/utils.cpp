#include <opencv2/opencv.hpp>
#include <iostream>
#include"utils.hpp"
namespace test {
	namespace utils {
		using namespace std;
		cv::Mat read_image(int argc, char** argv) {
			cv::Mat img;
			if (argc < 2 || (img = cv::imread(argv[1], cv::IMREAD_COLOR)).empty()) {
				cout << "cannot load image" << endl;
			}
			return img;
		}
	}
}