#include <opencv2/opencv.hpp>
#include <iostream>
#include "utils.hpp"
namespace car_detect {
	using namespace std;

	void detect(int argc, char** argv) {
		if (argc < 2) {
			cout << "\nusage:" << argv[0] << " <video_file>" << endl;
			return;
		}
		string file = argv[1];
		cv::VideoCapture cap;
		if (!cap.open(file)) {
			cout << "can not load video file:" << file << endl;
			return;
		}
		cout << "load video file ok:" << file << endl;
		double fps = cap.get(cv::CAP_PROP_FPS);
		cout << "fps:" << fps << endl;
		/*double wait = 1000.f / fps;
		cout << "wait:" << wait << endl;*/
		cv::Mat src, frame, gray, blur, mask, erode, dilate, close, dst;
		//cv::Ptr<cv::BackgroundSubtractorMOG2> bgs = cv::createBackgroundSubtractorMOG2();
		auto bgs = cv::createBackgroundSubtractorKNN();
		cv::Mat kernel = cv::getStructuringElement(cv::MorphShapes::MORPH_RECT, cv::Size(5, 5));//腐蚀、膨胀操作的卷积核
		//uchar min_height = 100, min_width = 110;
		uchar min_height = 50, min_width = 55;
		//int line_y = 600;
		int line_y = 300;
		cv::Point line_left(10, line_y), line_right(1260, line_y);
		int carno = 0;
		vector<cv::Point> cars;
		while (cap.isOpened()) {
			cap >> src;
			if (src.empty()) {
				cout << "video is over" << endl;
				break;
			}
			cv::resize(src, frame, cv::Size(), 1.f / 2, 1.f / 2);//缩小，可以提高处理速度

			//原图灰度化，去噪
			cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
			cv::GaussianBlur(gray, blur, cv::Size(3, 3), 5);//说明：噪声点比较小，所以用3 x 3的核，可以多试试，包括后面的sigmaX						
			//去除背景
			bgs->apply(blur, mask);
			//腐蚀，去除细小噪声
			cv::erode(mask, erode, kernel);
			//膨胀，放大“还原”
			cv::dilate(erode, dilate, kernel, cv::Point(-1, -1), 2);//膨胀2次

			//闭运算:消除内部的小方块
			cv::morphologyEx(dilate, close, cv::MorphTypes::MORPH_CLOSE, kernel);

			//找轮廓
			vector<vector<cv::Point>> contours;
			cv::findContours(close, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
			dst = close;
			//原图划线：在线前后范围内的才计数
			cv::line(frame, line_left, line_right, cv::Scalar(0, 255, 0));

			//计算最大外接矩形
			for (int idx = 0;idx < contours.size();idx++) {
				//cv::drawContours(frame, contours, idx, cv::Scalar(0, 0, 255));
				cv::Rect rect = cv::boundingRect(contours[idx]);
				//过滤掉小的矩形（噪声、远处的车)
				if (rect.width >= min_width && rect.height >= min_height) {
					cv::rectangle(frame, rect, cv::Scalar(0, 0, 255));
					//矩形中心点在线范围内时，进行计数
					int cx = rect.x + rect.width / 2;
					int cy = rect.y + rect.height / 2;
					/*if ((cy > (line_y - 5) && cy < (line_y + 5))) {
						carno += 1;
					}*/
					cv::Point cp = cv::Point(cx, cy);
					cars.push_back(cp);
					//cv::circle(frame, cv::Point(cx, cy), 1, cv::Scalar(0, 0, 255), 3);//画圆心，测试中心点是否正确

					//车辆计数逻辑没有理清楚，如何实现去重？？？
					//判断汽车是否过线
					for (int i = cars.size() - 1;i >= 0;i--) {
						if ((cars[i].y > (line_y - 5) && cars[i].y < (line_y + 5))) {
							carno += 1;
							cars.erase(cars.begin() + i);
						}
					}
				}
			}

			////判断汽车是否过线
			//for (int i = cars.size() - 1;i >= 0;i--) {
			//	if ((cars[i].y > (line_y - 5) && cars[i].y < (line_y + 5))) {
			//		carno += 1;
			//		cars.erase(cars.begin() + i);
			//	}
			//}
			//cout << "cars:" << carno << "," << cars.size() << endl;
			cv::putText(frame, "vehicles:" + to_string(carno), cv::Point(frame.cols / 2 + 10, 40), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
			cv::imshow(car_detect::Window, dst);
			cv::imshow("src", frame);
			if (cv::waitKey(1) == 27) {
				break;
			}
		}
		cap.release();
		cv::destroyAllWindows();
	}
}