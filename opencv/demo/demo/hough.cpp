#include <opencv2/opencv.hpp>
#include <iostream>
#include <math.h>
namespace test {
	/// <summary>
	/// ªÙ∑Ú±‰ªª
	/// </summary>
	namespace hough {
		using namespace std;
		/// <summary>
		/// ªÙ∑Ú‘≤±‰ªª
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void hough_circle(int argc, char** argv);
		void hough_test(int argc, char** argv) {
			hough_circle(argc, argv);
		}
		void hough_circle(int argc, char** argv) {
			if (argc != 2) {
				cout << "hough circle detect\nusage: " << argv[0] << " <image>\n" << endl;
				return;
			}
			cv::Mat temp,src, image;
			temp = cv::imread(argv[1], cv::IMREAD_COLOR);
			if (temp.empty()) {
				cout << "can not load " << argv[1] << endl;
				return;
			}
			cv::resize(temp, src, cv::Size(), 1.f / 5, 1.f / 5);

			cv::cvtColor(src, image, cv::COLOR_BGR2GRAY);
			cv::GaussianBlur(image, image, cv::Size(5, 5), 0, 0);

			vector<cv::Vec3f> circles;
			cv::HoughCircles(image, circles, cv::HOUGH_GRADIENT, 2, image.cols / 10);

			for (size_t i = 0;i < circles.size();i++) {
				cv::circle(src,
					cv::Point(cvRound(circles[i][0]), cvRound(circles[i][1])),
					cvRound(circles[i][2]),
					cv::Scalar(0, 0, 255),
					2,
					cv::LINE_AA
				);
			}

			cv::imshow("1", image);
			cv::imshow("ªÙ∑Ú‘≤±‰ªª", src);
			cv::waitKey(0);
			//cv::normalize()
		}
	}
}