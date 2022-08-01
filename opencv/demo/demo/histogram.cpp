#include <opencv2/opencv.hpp>
#include <iostream>
#include "utils.hpp"
namespace test {
	/// <summary>
	/// ֱ��ͼ����
	/// </summary>
	namespace histogram {
		using namespace std;

		void test1(int argc, char** argv);
		void test2(int argc, char** argv);
		void template_match(int argc, char** argv);
		void histogram_test(int argc, char** argv) {
			//test1(argc, argv);
			test2(argc, argv);
			//template_match(argc, argv);
		}
		void test1(int argc, char** argv) {
			if (argc != 2) {
				cout << "computer color histogram\nusage: " << argv[0] << " <image>" << endl;
				return;
			}

			cv::Mat temp, src;
			temp = cv::imread(argv[1], 1);
			if (temp.empty()) {
				cout << "cannot load " << argv[1] << endl;
				return;
			}
			cv::resize(temp, src, cv::Size(), 1.f / 2, 1.f / 2);

			//����HSVͼ�񣬷ָ�
			cv::Mat hsv;
			cv::cvtColor(src, hsv, cv::COLOR_BGR2HSV);

			float h_ranges[] = { 0,180 };//hue��ɫ��
			float s_ranges[] = { 0,256 };//���Ͷ�
			const float* ranges[] = { h_ranges,s_ranges };
			int histSize[] = { 30,32 }, ch[] = { 0,1 };

			cv::Mat hist;//ֱ��ͼ
			cv::calcHist(&hsv, //ԭͼ(����)
				1, //ͼ�����
				ch, //ͨ����
				cv::noArray(), //����
				hist, //ֱ��ͼ���
				2, //ά��
				histSize, //ÿ��ά��ֱ��ͼ�Ĵ�С
				ranges, //��ά��ֱ��ͼ��Χ
				true);//
			cv::normalize(hist, hist, 0, 255, cv::NORM_MINMAX);//��һ��

			int scale = 10;
			cv::Mat hist_img(histSize[0] * scale, histSize[1] * scale, CV_8UC3);

			//��ֱ��ͼ
			for (int h = 0;h < histSize[0];h++) {
				for (int s = 0;s < histSize[1];s++) {
					float hval = hist.at<float>(h, s);
					cv::rectangle(hist_img,
						cv::Rect(h * scale, s * scale, scale, scale),
						cv::Scalar::all(hval),
						-1
					);
				}
			}

			cv::imshow("image", src);
			cv::imshow("H-S histogram", hist_img);
			cv::waitKey(0);
		}
		void test2(int argc, char** argv) {
			cv::Mat src = utils::read_image(argc, argv);
			if (src.empty()) return;

			cv::Mat gray;
			cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

			cv::Mat hist;
			int channels[] = { 0 };
			int hist_size[] = { 100 };
			float s_ranges[] = { 0,255 };
			const float* ranges[] = { s_ranges };
			cv::calcHist(
				&gray, //ԭͼ
				1, //ͼ�����
				channels,//ͨ����
				cv::noArray(), //����
				hist, //ֱ��ͼ���
				1, //ά��
				hist_size, //ÿ��ά��ֱ��ͼ�Ĵ�С
				ranges, //��ά��ֱ��ͼ��Χ
				true
			);
			cout << hist.size() << endl;
			//cout << hist << endl;
			//����ֱ��ͼ
			cv::Mat imgHist(gray.rows, gray.cols, CV_16UC3);
			int step = imgHist.cols / hist_size[0];
			uchar histWidth = step - 2;
			vector<int> hist_array = (vector<int>)(hist.reshape(1, 1));
			auto itMaxPos = max_element(hist_array.begin(), hist_array.end());
			int maxHist = hist_array[itMaxPos - hist_array.begin()];
			int scale = maxHist / imgHist.rows;
			cout << "max:" << maxHist << ",scale:" << scale << endl;

			int x_0 = 0, y_0 = 0, x_1, y_1;
			for (int idx = 0;idx < hist_array.size();idx++) {
				x_0 += step;
				x_1 = x_0 + histWidth;
				int val = hist_array[idx];
				//cout << "val:" << val << endl;
				y_1 = val / scale;

				cv::Point pt1(x_0, 0);
				cv::Point pt2(x_1, y_1);
				cv::rectangle(imgHist, pt1, pt2, cv::Scalar(0, 0, 255), 1);
			}

			cv::imshow("hist", imgHist);
			cv::waitKey(0);
			cv::destroyAllWindows();
		}
		void template_match_help(char** argv) {
			cout << "\n"
				<< "example of using matchTemplate(). The call is:\n"
				<< "\n"
				<< argv[0] << " template image_to_be_searched\n"
				<< "\n"
				<< "\t this routing will search using all methods:\n"
				<< "\t\tcv::TM_SQDIFF		 0\n"
				<< "\t\tcv::TM_SQDIFF_NORMED 1\n"
				<< "\t\tcv::TM_CCORR  		 2\n"
				<< "\t\tcv::TM_CCORR_NORMED	 3\n"
				<< "\t\tcv::TM_CCOEFF 		 4\n"
				<< "\t\tcv::TM_CCOEFF_NORMED 5\n"
				<< "\n";
		}
		void template_match(int argc, char** argv) {
			if (argc != 3) {
				template_match_help(argv);
				return;
			}

			cv::Mat src, templ, ftmp[6], tmp;
			//��ģ��
			if ((templ = cv::imread(argv[1], 1)).empty()) {
				cout << "error reading template " << argv[1] << endl;
				template_match_help(argv);
				return;
			}
			//��ԭͼ
			if ((tmp = cv::imread(argv[2], 1)).empty()) {
				cout << "error reading src image " << argv[1] << endl;
				template_match_help(argv);
				return;
			}
			cv::resize(tmp, src, cv::Size(), 1.f / 5, 1.f / 5);

			//ģ��ƥ��
			for (int i = 0;i < 6;i++) {
				cv::matchTemplate(src, templ, ftmp[i], i);//i��ӦTM_SQDIFF��TM_SQDIFF_NORMED������
				cv::normalize(ftmp[i], ftmp[i], 1, 0, cv::NORM_MINMAX);
			}

			//��ʾ
			cv::imshow("template", templ);
			cv::imshow("src", src);
			cv::imshow("SQDIFF", ftmp[0]);
			cv::imshow("SQDIFF_NORMED", ftmp[1]);
			cv::imshow("CCORR", ftmp[2]);
			cv::imshow("CCORR_NORMED", ftmp[3]);
			cv::imshow("CCOEFF", ftmp[4]);
			cv::imshow("CCOEFF_NORMED", ftmp[5]);

			cv::waitKey(0);
		}
	}
}