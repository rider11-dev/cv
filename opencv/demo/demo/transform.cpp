#include <opencv2/opencv.hpp>
#include<iostream>

namespace test {
	/// <summary>
	/// 图像变换
	/// </summary>
	namespace transform {
		using namespace std;
		/// <summary>
		/// 仿射变换测试
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void warp_affine_test(int argc, char** argv);
		/// <summary>
		/// 透视变换测试
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void warp_perspective_test(int argc, char** argv);
		/// <summary>
		/// 对数-极坐标测试
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

			cv::Point2f srcTri[] = {//原图3个角坐标
				cv::Point2f(0,0),//左上角
				cv::Point2f(src.cols - 1,0),//右上角
				cv::Point2f(0,src.rows - 1)//左下角
			};

			cv::Point2f dstTri[] = {//目标3个角坐标
				cv::Point2f(src.cols * 0.f,src.rows * 0.33f),//左上角
				cv::Point2f(src.cols * 0.85f,src.rows * 0.25f),//右上角
				cv::Point2f(src.cols * 0.15f,src.rows * 0.7f)//左下角
			};
			//计算仿射变换矩阵M（核）
			cv::Mat warp_mat = cv::getAffineTransform(srcTri, dstTri);
			cv::Mat dst;
			cv::warpAffine(src, dst, warp_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
			for (int i = 0;i < 3;++i) {
				cv::circle(dst, dstTri[i], 5, cv::Scalar(255, 0, 255), 1, cv::LINE_AA);
			}

			//逆仿射变换
			cv::Mat invert_warp_mat;
			cv::invertAffineTransform(warp_mat, invert_warp_mat);
			cv::Mat src1;
			cv::warpAffine(dst, src1, invert_warp_mat, dst.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

			cv::imshow("1/2原图", src);
			cv::imshow("仿射变换", dst);
			cv::imshow("逆仿射变换", src1);

			//旋转
			cv::Point2f center(src.cols * 0.5f, src.rows * 0.5f);
			cv::Mat dstR;
			for (int frame = 0;;++frame) {
				double angle = frame * 3 % 360;
				double scale = (cos((angle - 60) * CV_PI / 180) + 1.05) * 0.8;

				cv::Mat rot_mat = cv::getRotationMatrix2D(center, angle, scale);

				cv::warpAffine(src, dstR, rot_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
				cv::imshow("图像旋转", dstR);
				if (cv::waitKey(30) > 0) {
					break;
				}
			}


			cv::waitKey();
		}

		void warp_perspective_test(int argc, char** argv) {
			if (argc != 2) {
				cout << "透视变换\nusage: " << argv[0] << " <imagename>\n" << endl;
				return;
			}

			cv::Mat orig = cv::imread(argv[1], cv::IMREAD_COLOR);
			if (orig.empty()) { cout << "can not load file: " << argv[1] << endl; return; };

			cv::Mat src;
			cv::resize(orig, src, cv::Size(0, 0), 0.5f, 0.5f);

			cv::Point2f srcQuad[] = {
				cv::Point2f(0,0),//左上
				cv::Point2f(src.cols - 1,0),//右上
				cv::Point2f(src.cols - 1,src.rows - 1),//右下
				cv::Point2f(0,src.rows - 1)//左下
			};

			cv::Point2f dstQuad[] = {
				cv::Point2f(src.cols * 0.05f,src.rows * 0.33f),//左上
				cv::Point2f(src.cols * 0.9f,src.rows * 0.25f),//右上
				cv::Point2f(src.cols * 0.8f,src.rows * 0.9f),//右下
				cv::Point2f(src.cols * 0.2f,src.rows * 0.7f)//左下
			};

			//透视变换
			cv::Mat warp_mat = cv::getPerspectiveTransform(srcQuad, dstQuad);
			cv::Mat dst;
			cv::warpPerspective(src, dst, warp_mat, src.size(), cv::INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar());
			for (int i = 0;i < 4;i++) {
				cv::circle(dst, dstQuad[i], 5, cv::Scalar(255, 0, 255), 1, cv::FILLED);
			}
			cv::imshow("透视变换", dst);
			cv::waitKey();

		}
		void log_polar_test(int argc, char** argv) {
			if (argc != 3) {
				cout << "对数极坐标变换\nusage: " << argv[0] << " <imagename> <M value>\n"
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