#include <opencv2/opencv.hpp>
#include <iostream>
namespace test {
	/// <summary>
	/// �ؼ�����
	/// </summary>
	namespace keypoints {
		using namespace std;
		void test1(int argc, char** argv) {
			if (argc != 3) {
				cout << "usage:\n" << argv[0] << " <image1> <image2>" << endl;
				return;
			}
			cv::Mat tmp1, tmp2,tmp2_color;
			if ((tmp1 = cv::imread(argv[1], cv::IMREAD_GRAYSCALE)).empty()) {
				cout << "cannot load image:" << argv[1] << endl;
				return;
			}
			if ((tmp2 = cv::imread(argv[2], cv::IMREAD_GRAYSCALE)).empty()) {
				cout << "cannot load image:" << argv[2] << endl;
				return;
			}
			tmp2_color = cv::imread(argv[2], cv::IMREAD_COLOR);

			cv::Mat imgA, imgB,imgC;
			uchar scale = 1;
			if (scale > 1) {
				cv::resize(tmp1, imgA, cv::Size(), 1.0f / scale, 1.0f / scale);
				cv::resize(tmp2, imgB, cv::Size(), 1.0f / scale, 1.0f / scale);
				cv::resize(tmp2_color, imgC, cv::Size(), 1.0f / scale, 1.0f / scale);
			}
			else {
				imgA = tmp1;
				imgB = tmp2;
				imgC = tmp2_color;
			}
			cv::Size img_size = imgA.size();
			int win_size = 10;

			//��ȡ�����ٵ�����ֵ
			vector<cv::Point2f> cornersA, cornersB;
			const int MAX_CORNERS = 500;
			cv::goodFeaturesToTrack(
				imgA,
				cornersA,//����ǵ�
				MAX_CORNERS,//���ǵ���
				0.01,//��������
				5,//�ǵ���С����
				cv::noArray(),//����
				3,//���ڴ�С
				false,//true:Harris�㷨��false��Shi-Tomasi�㷨
				0.04
			);
			//���������ؽǵ�
			cv::cornerSubPix(
				imgA,
				cornersA,
				cv::Size(win_size, win_size),
				cv::Size(-1, -1),
				cv::TermCriteria(
					cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
					20, //����������
					0.03)
			);
			//����Lucas Kanada�㷨
			vector<uchar> features_found;
			cv::calcOpticalFlowPyrLK(
				imgA,
				imgB,
				cornersA,
				cornersB,
				features_found,
				cv::noArray(),
				cv::Size(win_size * 2 + 1, win_size * 2 + 1),
				5,
				cv::TermCriteria(
					cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS,
					20,
					0.3
				)
			);
			for (size_t i = 0;i < cornersA.size();i++) {
				if (!features_found[i]) {
					continue;
				}
				cv::line(imgC, cornersA[i], cornersB[i], cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
			}

			cv::imshow("imageA", imgA);
			cv::imshow("imageB", imgB);
			cv::imshow("LK Optical Flow Example", imgC);

			cv::waitKey(0);
		}

		void keypoints_test(int argc, char** argv) {
			test1(argc, argv);
		}


	}
}