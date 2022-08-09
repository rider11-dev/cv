#include <opencv2/opencv.hpp>
#include<iostream>

namespace test {
	/// <summary>
	/// ͼ��任
	/// </summary>
	namespace transform {
		using namespace std;
		/// <summary>
		/// ����任����
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void warp_affine_test(int argc, char** argv);
		/// <summary>
		/// ͸�ӱ任����
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void warp_perspective_test(int argc, char** argv);
		/// <summary>
		/// ����-���������
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void log_polar_test(int argc, char** argv);

		void transform_test(int argc, char** argv) {
			//warp_affine_test(argc, argv);
			//warp_perspective_test(argc, argv);
			log_polar_test(argc, argv);
		}

		void warp_affine_test(int argc, char** argv) {
			if (argc != 2) {
				cout << "warp affine\nUsage: " << argv[0] << " <imagename>\n" << endl;
				return;
			}

			cv::Mat orig = cv::imread(argv[1], cv::IMREAD_COLOR);
			if (orig.empty()) { cout << "can not load " << argv[1] << endl;return; }

			//cout << "rows=" << src.rows << ",cols=" << src.cols << endl;
			cv::Mat src;
			cv::resize(orig, src, cv::Size(0, 0), 0.5f, 0.5f);

			cv::Point2f srcTri[] = {//ԭͼ3��������
				cv::Point2f(0,0),//���Ͻ�
				cv::Point2f(src.cols - 1,0),//���Ͻ�
				cv::Point2f(0,src.rows - 1)//���½�
			};

			cv::Point2f dstTri[] = {//Ŀ��3��������
				cv::Point2f(src.cols * 0.f,src.rows * 0.33f),//���Ͻ�
				cv::Point2f(src.cols * 0.85f,src.rows * 0.25f),//���Ͻ�
				cv::Point2f(src.cols * 0.15f,src.rows * 0.7f)//���½�
			};
			//�������任����M���ˣ�
			cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
			cv::Mat dst;
			cv::warpAffine(src, dst, warp_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
			for (int i = 0;i < 3;++i) {
				cv::circle(dst, dstTri[i], 5, cv::Scalar(255, 0, 255), 1, cv::LINE_AA);
			}

			//�����任
			cv::Mat invert_warp_mat;
			cv::invertAffineTransform(warp_mat, invert_warp_mat);
			cv::Mat src1;
			cv::warpAffine(dst, src1, invert_warp_mat, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

			cv::imshow("1/2ԭͼ", src);
			cv::imshow("����任", dst);
			cv::imshow("�����任", src1);

			//��ת
			cv::Point2f center(src.cols * 0.5f, src.rows * 0.5f);
			cv::Mat dstR;
			for (int frame = 0;;++frame) {
				double angle = frame * 3 % 360;
				double scale = (cos((angle - 60) * CV_PI / 180) + 1.05) * 0.8;

				cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);

				cv::warpAffine(src, dstR, rot_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
				cv::imshow("ͼ����ת", dstR);
				if (cv::waitKey(30) > 0) {
					break;
				}
			}


			cv::waitKey();
		}

		void warp_perspective_test(int argc, char** argv) {
			if (argc != 2) {
				cout << "͸�ӱ任\nusage: " << argv[0] << " <imagename>\n" << endl;
				return;
			}

			cv::Mat orig = cv::imread(argv[1], cv::IMREAD_COLOR);
			if (orig.empty()) { cout << "can not load file: " << argv[1] << endl; return; };

			cv::Mat src;
			cv::resize(orig, src, cv::Size(0, 0), 0.5f, 0.5f);

			cv::Point2f srcQuad[] = {
				cv::Point2f(0,0),//����
				cv::Point2f(src.cols - 1,0),//����
				cv::Point2f(src.cols - 1,src.rows - 1),//����
				cv::Point2f(0,src.rows - 1)//����
			};

			cv::Point2f dstQuad[] = {
				cv::Point2f(src.cols * 0.05f,src.rows * 0.33f),//����
				cv::Point2f(src.cols * 0.9f,src.rows * 0.25f),//����
				cv::Point2f(src.cols * 0.8f,src.rows * 0.9f),//����
				cv::Point2f(src.cols * 0.2f,src.rows * 0.7f)//����
			};

			//͸�ӱ任
			cv::Mat warp_mat = cv::getPerspectiveTransform(srcQuad, dstQuad);
			cv::Mat dst;
			cv::warpPerspective(src, dst, warp_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
			for (int i = 0;i < 4;i++) {
				cv::circle(dst, dstQuad[i], 5, cv::Scalar(255, 0, 255), 1, cv::FILLED);
			}
			cv::imshow("͸�ӱ任", dst);
			cv::waitKey();

		}
		void log_polar_test(int argc, char** argv) {
			if (argc != 3) {
				cout << "����������任\nusage: " << argv[0] << " <imagename> <M value>\n"
					<< "<M value>~30 is usually good enough\n" << endl;
				return;
			}

			cv::Mat orig = cv::imread(argv[1], cv::IMREAD_COLOR);
			if (orig.empty()) { cout << "can not load file: " << argv[1] << endl; return; };

			cv::Mat src;
			cv::resize(orig, src, cv::Size(0, 0), 0.5f, 0.5f);

			double M = atof(argv[2]);
			cv::Mat dst(src.size(), src.type()), src2(src.size(), src.type());

			cv::logPolar(src, dst, cv::Point2f(src.cols * 0.5f, src.rows * 0.5f), M, cv::INTER_LINEAR | cv::WARP_FILL_OUTLIERS);
			cv::logPolar(dst, src2, cv::Point2f(src.cols * 0.5f, src.rows * 0.5f), M, cv::INTER_LINEAR | cv::WARP_INVERSE_MAP);

			cv::imshow("log-polar", dst);
			cv::imshow("inverse log-lopar", src2);
			
			cv::waitKey();
		}
	}
}