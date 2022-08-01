#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>
#include "image.hpp"
using namespace std;
namespace test {
	namespace image {
		void show_image(string file)
		{
			cv::Mat img = cv::imread(file, -1);
			if (img.empty())
			{
				return;
			}
			string wName = "image_show";
			cv::namedWindow(wName, cv::WINDOW_AUTOSIZE);
			cv::imshow(wName, img);
			cv::waitKey(0);
			cv::destroyWindow(wName);
		}
		/// <summary>
		/// 平滑处理
		/// </summary>
		/// <param name="file"></param>
		void smooth_image(std::string file)
		{
			cv::Mat image = cv::imread(file, -1);
			if (image.empty())
			{
				CV_LOG_INFO(NULL, "image read failed!");
				return;
			}
			string wNameIn = "smooth_in", wNameOut = "smooth_out";
			cv::namedWindow(wNameIn, cv::WINDOW_NORMAL);
			cv::namedWindow(wNameOut, cv::WINDOW_NORMAL);

			cv::imshow(wNameIn, image);

			cv::Mat out;
			//高斯模糊
			cv::GaussianBlur(image, out, cv::Size(5, 5), 3, 3);
			cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);

			cv::imshow(wNameOut, out);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}
		/// <summary>
		/// 边缘检测
		/// </summary>
		/// <param name="file"></param>
		void edge_detect(std::string file)
		{
			cv::Mat img_rgb, img_gry, img_cny, img_pyr, img_pyr2,img_pyr3;
			string name_gray = "gray", name_canny = "canny";
			cv::namedWindow(name_gray, cv::WINDOW_NORMAL);
			cv::namedWindow(name_canny, cv::WINDOW_NORMAL);

			img_rgb = cv::imread(file);
			cv::cvtColor(img_rgb, img_gry, cv::COLOR_BGR2GRAY);
			cv::pyrDown(img_gry, img_pyr);
			cv::imshow(name_gray, img_pyr);

			//cv::pyrDown(img_pyr, img_pyr2);
			//cv::pyrDown(img_pyr2, img_pyr3);
			//cv::imshow(name_gray, img_pyr3);

			//cv::Canny(img_pyr3, img_cny, 10, 100, 3, true);
			cv::Canny(img_pyr, img_cny, 10, 100, 3, true);
			cv::imshow(name_canny, img_cny);

			cv::waitKey(0);

			cv::destroyAllWindows();
		}
	}
}