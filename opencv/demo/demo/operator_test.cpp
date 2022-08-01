#include <opencv2/opencv.hpp>
#include <iostream>

namespace test {
	/// <summary>
	/// 算子测试（边缘）
	/// </summary>
	namespace operator_test {
		using namespace std;
		/// <summary>
		/// 索贝尔算法
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void sobel_test(int argc, char** argv);
		/// <summary>
		/// 沙尔算子，只用于3 x 3内核
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void scharr_test(int argc, char** argv);
		/// <summary>
		/// 拉普拉斯算子
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void laplacian_test(int argc, char** argv);
		void canny_test(int argc, char** argv);
		/// <summary>
		/// 手动调整阈值
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void canny_mannual(int argc, char** argv);
		/// <summary>
		/// 轮廓矩形
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void contour_rect(int argc, char** argv);
		cv::Mat read_image(int argc, char** argv);
		void test(int argc, char** argv) {
			//sobel_test(argc, argv);
			//scharr_test(argc, argv);
			//laplacian_test(argc, argv);
			//canny_test(argc, argv);
			canny_mannual(argc, argv);
		}
		cv::Mat read_image(int argc, char** argv) {
			cv::Mat img;
			if (argc != 2 || (img = cv::imread(argv[1], cv::IMREAD_COLOR)).empty()) {
				cout << "cannot load image" << endl;
			}
			return img;
		}

		void sobel_test(int argc, char** argv) {
			cv::Mat src = read_image(argc, argv);
			if (src.empty()) { return; };
			//索贝尔算子，必须对x、y轴分别计算然后合并才可以
			cv::Mat dstX, dstY;
			cv::Sobel(src, dstX, CV_64F, 1, 0, 3);
			cv::Sobel(src, dstY, CV_64F, 0, 1, 3);
			cv::Mat dst;
			cv::add(dstX, dstY, dst);

			cv::imshow("src_sobel", src);
			cv::imshow("dst_sobel", dst);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}
		void scharr_test(int argc, char** argv) {
			cv::Mat src = read_image(argc, argv);
			if (src.empty()) { return; };
			cv::Mat dstX, dstY;
			cv::Scharr(src, dstX, CV_64F, 1, 0);
			cv::Scharr(src, dstY, CV_64F, 0, 1);
			cv::Mat dst;
			cv::add(dstX, dstY, dst);

			cv::imshow("src_scharr", src);
			cv::imshow("dst_scharr", dst);

			cv::waitKey(0);

			cv::destroyAllWindows();

		}
		void laplacian_test(int argc, char** argv) {
			cv::Mat src = read_image(argc, argv);
			if (src.empty()) return;

			//去噪
			cv::Mat blur;
			//blur = src;
			cv::GaussianBlur(src, blur, cv::Size(3, 3), 0, 0);

			cv::Mat dst;
			int ddepth = CV_64F;
			//int ddepth = CV_32F;
			//int ddepth = CV_8U;
			//int ddepth = -1;
			cv::Laplacian(blur, dst, ddepth, 3);

			cv::imshow("src_laplacian", src);
			cv::imshow("dst_laplacian", dst);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}
		void canny_test(int argc, char** argv) {
			cv::Mat src = read_image(argc, argv);
			if (src.empty()) return;

			//cv::Canny(image, edges, threshold1, threshold2, L2gradient);
			cv::Mat dst1, dst2, dst3;
			cv::Canny(src, dst1, 100, 200);//阈值越小，细节越丰富
			cv::Canny(src, dst2, 60, 90);
			cv::Canny(src, dst3, 10, 50);

			cv::imshow("src", src);
			cv::imshow("threshold:100~200", dst1);
			cv::imshow("threshold:60~90", dst2);
			cv::imshow("threshold:10~50", dst3);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}

		int g_minThreshold = 80, g_maxThreshold = 100;
		string dstWindow = "dst";
		void thresholdTrackbar_callback(int pos, void* userdata) {
			cout << "min=" << g_minThreshold << ",max=" << g_maxThreshold << endl;
			cv::Mat img = *(cv::Mat*)(userdata);

			cv::Mat dst;

			cv::Canny(img, dst, g_minThreshold, g_maxThreshold);
			cv::imshow(dstWindow, dst);
		}
		void canny_mannual(int argc, char** argv) {
			cv::Mat src = read_image(argc, argv);
			if (src.empty()) return;


			cv::namedWindow(dstWindow, cv::WINDOW_NORMAL);

			cv::createTrackbar("minThreshold", dstWindow, &g_minThreshold, 500, thresholdTrackbar_callback, &src);
			cv::createTrackbar("maxThreshold", dstWindow, &g_maxThreshold, 500, thresholdTrackbar_callback, &src);

			cv::imshow("src", src);
			cv::waitKey(0);
			cv::destroyAllWindows();
		}
		void contour_rect(int argc, char** argv) {
			cv::Mat src = read_image(argc, argv);
			if (src.empty()) return;

			cv::Mat gray;
			cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

		}

	}
}