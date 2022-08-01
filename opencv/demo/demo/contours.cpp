#include <opencv2/opencv.hpp>
#include <iostream>
#include <algorithm>
#include "utils.hpp"
namespace test {
	namespace contours {
		using namespace std;
		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void contours_trackbar(int argc, char** argv);
		/// <summary>
		/// ������ͨ����
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void connected_component(int argc, char** argv);
		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void contour_rect(int argc, char** argv);
		/// <summary>
		/// ��������
		/// </summary>
		void contours_test(int argc, char** argv) {
			//contours_trackbar(argc, argv);
			//connected_component(argc, argv);
			contour_rect(argc, argv);
		}
		cv::Mat g_gray, g_binary;
		int g_thresh = 100;
		string wName = "contours";

		void on_trackbar(int, void*) {
			//��ֵ��
			cv::threshold(g_gray, g_binary, g_thresh, 255, cv::THRESH_BINARY);
			//cout << "g_binary:" << g_binary.rows << "," << g_binary.cols << endl;
			vector<vector<cv::Point>> contours;
			//ʶ������
			cv::findContours(
				g_binary,
				contours,
				cv::noArray(),
				cv::RETR_LIST,
				cv::CHAIN_APPROX_SIMPLE
			);
			cout << "��������: " << contours.size() << endl;
			/*for (int i = 0;i < contours.size();i++) {
				cout << contours[i] << endl;
			}*/
			g_binary = cv::Scalar::all(0);//��ɫ
			/*if (contours.size() > 0) {
				cout << "��һ������:" << contours[0] << endl;
			}*/
			cv::drawContours(g_binary, contours, -1, cv::Scalar::all(255));//��ɫ
			cv::imshow(wName, g_binary);
		}
		void contours_trackbar(int argc, char** argv) {
			cv::Mat src;
			if (argc != 2 || (src = cv::imread(argv[1], 0)).empty()) {
				cout << "find threshold dependent\nusage: " << argv[0]
					<< " <image>" << endl;
				return;
			}
			cv::resize(src, g_gray, cv::Size(), 1.f / 4, 1.f / 4);
			//g_gray = src;
			//cout << "g_gray:" << g_gray.rows << "," << g_gray.cols << endl;
			cv::imshow("gray", g_gray);

			cv::namedWindow(wName, cv::WINDOW_AUTOSIZE);
			cv::resizeWindow(wName, cv::Size2i(g_gray.cols, g_gray.rows));

			cv::createTrackbar(
				"threshold",
				wName,
				&g_thresh,
				255,
				on_trackbar
			);
			on_trackbar(0, 0);

			cv::waitKey(0);
		}
		void connected_component(int argc, char** argv) {
			cv::Mat img, img_edge, labels, img_color, stats;

			if (argc != 2
				|| (img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE)).empty()) {
				cout << "\nexample drawing connected components\n"
					<< "call is:\n" << argv[0] << " image\n\n";
				return;
			}
			cv::resize(img, img, cv::Size(), 1.f / 4, 1.f / 4);
			cv::imshow("image", img);

			cv::threshold(img, img_edge, 128, 255, cv::THRESH_BINARY);//��ֵ��
			cv::imshow("image after threshold", img_edge);

			cv::Mat centroids;
			int i, nccomps = cv::connectedComponentsWithStats(
				img_edge,
				labels,
				stats,
				centroids//��cv::noArray���ܵ��³�����������ﴫһ������
			);
			cout << "total connected components detected: " << nccomps << endl;

			vector<cv::Vec3b> colors(nccomps + 1);
			colors[0] = cv::Vec3b::all(0);//������ɫ
			for (int i = 1;i <= nccomps;i++) {
				colors[i] = cv::Vec3b(rand() % 256, rand() % 256, rand() % 256);
				if (stats.at<int>(i - 1, cv::CC_STAT_AREA) < 100) {
					colors[i] = cv::Vec3b::all(0);//С����ҲΪ��ɫ
				}
			}
			img_color = cv::Mat::zeros(img.size(), CV_8UC3);
			for (int y = 0;y < img_color.rows;y++) {
				for (int x = 0;x < img_color.cols;x++) {
					int label = labels.at<int>(y, x);
					CV_Assert(0 <= label && label <= nccomps);
					img_color.at<cv::Vec3b>(y, x) = colors[label];
				}
			}
			cv::imshow("labeled map", img_color);
			cv::waitKey(0);
		}

		void contour_rect(int argc, char** argv) {
			cv::Mat imgSrc = utils::read_image(argc, argv);
			if (imgSrc.empty()) return;

			//��ͨ���ڰ���Ƭ
			cv::Mat imgGray;
			cv::cvtColor(imgSrc, imgGray, cv::COLOR_BGR2GRAY);
			//��ֵ��
			cv::Mat imgBinary;
			cv::threshold(imgGray, imgBinary, 128, 255, cv::ThresholdTypes::THRESH_BINARY);
			//��������
			vector<vector<cv::Point>> contours;
			cv::findContours(imgBinary, contours, cv::RetrievalModes::RETR_LIST, cv::ContourApproximationModes::CHAIN_APPROX_SIMPLE);
			cout << "find contours:" << contours.size() << endl;
			//����һ��С�����������ܳ���
			double len_threshold = 400.f;//��������
			for (int idx = contours.size() - 1;idx >= 0;idx--) {
				double len = cv::arcLength(contours[idx], true);
				if (len < len_threshold) {
					contours.erase(contours.begin() + idx);
				}
				else {
					cout << "length:" << len << endl;
				}
			}
			cout << "contours after filter:" << contours.size() << endl;

			//��������
			cv::Mat imgContour(imgGray.rows, imgGray.cols, CV_8UC3);
			////������Ҫ����������
			//for (int idx = 0;idx < contours.size();idx++) {
			//	cout << "contours[" << idx << "]" << endl;
			//	cv::drawContours(imgContour, contours, idx, cv::Scalar(0, 0, 255));
			//	cv::imshow("test", imgContour);

			//	uchar code = cv::waitKey(0);
			//	if (code == 27) {//ESC
			//		break;
			//	}
			//}


			int contourIdx = -1;
			//int contourIdx = 1;
			cv::drawContours(imgContour, contours, contourIdx, cv::Scalar(0, 0, 255));

			//������С��Ӿ���
			cv::RotatedRect minRect = cv::minAreaRect(contours[0]);
			//�������4��������
			cv::Point2f rect_points[4];
			minRect.points(rect_points);
			//�����Σ���4����
			for (uchar idx = 0;idx < 4;idx++) {
				cv::Point pt1 = rect_points[idx];//���Զ���������
				//cout << rect_points[idx] << "," << pt1 << endl;
				cv::Point pt2 = rect_points[(idx + 1) % 4];
				cv::line(imgContour, pt1, pt2, cv::Scalar(0, 255, 0));
			}

			//���������Ӿ���
			cv::Rect maxRect = cv::boundingRect(contours[0]);
			cv::rectangle(imgContour, maxRect, cv::Scalar(255, 0, 0));

			cv::imshow("src", imgGray);
			cv::imshow("contours", imgContour);
			cv::waitKey(0);

			cv::destroyAllWindows();

		}
	}

}