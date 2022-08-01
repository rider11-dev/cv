#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
namespace test {
	/// <summary>
	/// 背景提取测试
	/// </summary>
	namespace bgr_extract {
		using namespace std;
		/// <summary>
		/// 提取视频文件的像素
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void read_rgb(int argc, char** argv);
		void bgr_substract();
		void bgr_extract_test(int argc, char** argv) {
			//read_rgb(argc, argv);
			bgr_substract();
		}

		void read_rgb(int argc, char** argv) {
			cv::VideoCapture cap;
			if (argc != 2 || !cap.open(argv[1])) {
				cout << "\n"
					<< "read out RGB pixel values and store them to disk\ncall:\n"
					<< argv[0] << " video_file\n"
					<< "\n this will store to files blines.csv,glines.csv and rlines.csv\n\n"
					<< endl;
				return;
			}

			cv::Point pt1(900, 100), pt2(950, 200);
			int max_buffer;
			cv::Mat rawImage;
			ofstream b, g, r;
			string folder = "D:/learn/opencv/demo/x64/Debug/lines";
			b.open(folder + "/blines.csv");
			g.open(folder + "/glines.csv");
			r.open(folder + "/rlines.csv");

			for (;;) {
				cap >> rawImage;
				if (!rawImage.data) {
					break;
				}
				//cout << rawImage.size << endl; break;
				cv::LineIterator it(rawImage, pt1, pt2, 8);
				for (int j = 0;j < it.count;++j, ++it) {
					int b_v = (int)(*it)[0];
					int g_v = (int)(*it)[1];
					int r_v = (int)(*it)[2];
					cout << "(" << b_v << "," << g_v << "," << r_v << ")" << endl;
					b << b_v << ", ";
					g << g_v << ", ";
					r << r_v << ", ";
					(*it)[2] = 255;
				}
				b << "\n";g << "\n";r << "\n";
				cv::imshow(argv[0], rawImage);
				int c = cv::waitKey(10);
				if (c == 27) {
					break;
				}
			}
			b << endl;g << endl;r << endl;
			b.close();g.close();r.close();
			cout << "\n"
				<< "data stored to files: blines.csv,glines.csv and rlines.csv\n\n"
				<< endl;
		}
		void bgr_substract() {
			cv::VideoCapture cap;
			cap.open(0);
			cv::Ptr<cv::BackgroundSubtractorMOG2> mog2 = cv::createBackgroundSubtractorMOG2();
			cv::Mat frame;
			cv::Mat foreground;
			while (cap.isOpened()) {
				cap >> frame;
				if (frame.empty()) break;

				mog2->apply(frame, foreground);

				cv::imshow("video", foreground);
				if (cv::waitKey(30) == 27) {
					break;
				}
			}

			mog2.release();
			cap.release();
		}

	}
}