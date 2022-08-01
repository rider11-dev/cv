
#ifndef TEST_HELLO_H
#define TEST_HELLO_H
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>
namespace test {
	namespace hello {
		void test() {
			cv::VideoCapture cap;
			cap.open(0);
			cv::Mat frame;
			while (cap.isOpened()) {
				cap >> frame;
				if (frame.empty()) {
					break;
				}
				cv::imshow("camera", frame);
				if (cv::waitKey(30) == 27) {//esc
					break;
				}
			}
			cap.release();
		}
	}
}
#endif