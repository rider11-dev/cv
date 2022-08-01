#include <opencv2/opencv.hpp>
using namespace std;
namespace test {
	/// <summary>
	/// 滤波器测试
	/// </summary>
	namespace filter {
		//#define SUM_RGB_TEST
#define THRESHOLD_TEST

#ifdef SUM_RGB_TEST
		/// <summary>
		/// 对三个通道求和测试
		/// </summary>
		void sum_rgb_test(int argc, char** argv);
		void sum_rgb(const cv::Mat& src, cv::Mat& dst);
#endif
#ifdef THRESHOLD_TEST
		void threshold_test(int argc, char** argv);
#endif
		void filter_test(int argc, char** argv)
		{
#ifdef SUM_RGB_TEST
			sum_rgb_test(argc, argv);
#endif
#ifdef THRESHOLD_TEST
			threshold_test(argc, argv);
#endif
		}

#ifdef SUM_RGB_TEST
		void sum_rgb_test(int argc, char** argv) {
			if (argc < 2) {
				cout << "请指定图像" << endl;
				return;
			}
			cv::Mat src = cv::imread(argv[1]), dst;
			if (src.empty()) {
				cout << "打开图像文件失败:" << argv[1] << endl;
				return;
			}
			sum_rgb(src, dst);

			cv::imshow("src", src);
			cv::imshow("dst", dst);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}
		void sum_rgb(const cv::Mat& src, cv::Mat& dst) {
			//将图像拆分到颜色面
			vector<cv::Mat> planes;
			cv::split(src, planes);

			cv::Mat b = planes[0], g = planes[1], r = planes[2], s;
			//加权求和：将3个通道取相同权值后相加
			cv::addWeighted(r, 1. / 3, g, 1. / 3, 0.0, s);
			cv::addWeighted(s, 1., b, 1. / 3, 0.0, s);
			//阈值处理
			cv::threshold(s, dst, 100, 100, cv::THRESH_TRUNC);//截断处理
		}
#endif
#ifdef THRESHOLD_TEST
		void threshold_test(int argc, char** argv) {
			if (argc != 7) {
				cout << "usage: " << argv[0] << " fixed_threshold invert(0=off|1=on)"
					"adaptive_type(0=mean|1=gaussian) block_size offset image\n"
					"example: " << argv[0] << " 100 1 0 15 10 fruits.jpg\n";
				//.\demo.exe 15 1 1 71 15 pp.jpg
				return;
			}

			double fixed_threshold = (double)atof(argv[1]);
			int threshold_type = atoi(argv[2]) ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;//二值化操作
			int adaptive_method = atoi(argv[3]) ? cv::ADAPTIVE_THRESH_MEAN_C : cv::ADAPTIVE_THRESH_GAUSSIAN_C;

			int block_size = atoi(argv[4]);
			double offset = (double)atof(argv[5]);
			cv::Mat Igray = cv::imread(argv[6], cv::IMREAD_GRAYSCALE);//读取灰度图像
			if (Igray.empty()) {
				cout << "无法加载图像文件:" << argv[6] << endl;
				return;
			}
			
			cv::Mat It, Iat;
			//常规阈值化处理
			cv::threshold(Igray, It, fixed_threshold, 255, threshold_type);
			//自适应阈值化处理
			cv::adaptiveThreshold(Igray, Iat, 255, adaptive_method, threshold_type, block_size, offset);

			cv::imshow("src", Igray);
			cv::imshow("threshold", It);
			cv::imshow("adaptive threshold", Iat);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}
#endif
	}
}