#include <string>
#include <iostream>
#include <fstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/core/utils/logger.hpp"

#include "video.hpp"

using namespace std;
namespace test {
	namespace video {
		void open_camera()
		{
			int64 start = cv::getTickCount();
			cv::VideoCapture cap;
			cap.open(0);//打开第一个摄像头

			//写入文件相关
			double fps = cap.get(cv::CAP_PROP_FPS);
			cv::Size size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH), (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT));
			cv::VideoWriter writer;
			//bool opened = writer.open("camera.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, size);
			bool opened = writer.open("camera.mpg", cv::VideoWriter::fourcc('D', 'I', 'V', 'X'), fps, size);
			if (!opened) {
				CV_LOG_INFO(NULL, "视频文件打开失败");
			}
			else {
				CV_LOG_INFO(NULL, "视频文件打开成功");
			}

			cv::Mat bgr_frame, logpolar_frame;//极坐标
			for (;;)
			{
				cap >> bgr_frame;
				if (bgr_frame.empty())
				{
					CV_LOG_INFO(NULL, "no frame from camera" << "!");
					break;
				}
				cv::imshow("bgr", bgr_frame);
				cv::logPolar(bgr_frame, logpolar_frame, cv::Point2f(bgr_frame.cols / 2, bgr_frame.rows / 2), 40, cv::WARP_FILL_OUTLIERS);
				cv::imshow("log_polar", logpolar_frame);
				//writer << logpolar_frame;
				writer << bgr_frame;

				char c = cv::waitKey(10);
				if (c == 27)//esc
				{
					CV_LOG_INFO(NULL, "open_camera exit!");
					writer.release();
					break;
				}
			};
			cap.release();

			int64 stop = cv::getTickCount();
			CV_LOG_INFO(NULL, "执行耗时:" << ((stop - start) / cv::getTickFrequency()) << "s");
		}

		int g_slider_position = 0;
		int g_run = 1, g_dontset = 0;//单步模式
		cv::VideoCapture g_cap;

		void onTrackbarSlide(int pos, void*)
		{
			g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
			if (!g_dontset)
			{
				g_run = 1;
			}
			g_dontset = 0;
		}

		void video_play(std::string file)
		{
			string wName = "video_play";
			cv::namedWindow(wName, cv::WINDOW_AUTOSIZE);
			if (!g_cap.open(file))
			{
				CV_LOG_WARNING(NULL, "video open failed!");
				return;
			}
			int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
			int tmpW = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
			int tmpH = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
			CV_LOG_INFO(NULL, "video has " << frames << " frames of dimensions(" << tmpW << ", " << tmpH << ").");
			cv::createTrackbar("Position", wName, &g_slider_position, frames, onTrackbarSlide);

			cv::Mat frame;
			for (;;) {
				if (g_run != 0)
				{
					g_cap >> frame;
					if (frame.empty())
					{
						CV_LOG_INFO(NULL, "frame is empty");
						break;
					}
					int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
					g_dontset = 1;

					cv::setTrackbarPos("Position", wName, current_pos);
					cv::imshow(wName, frame);

					g_run -= 1;
				}
				char c = (char)cv::waitKey(10);
				if (c == 's')
				{
					g_run = 1;
					CV_LOG_INFO(NULL, "single step,run = " << g_run);
				}
				if (c == 'r')
				{
					g_run = -1;
					CV_LOG_INFO(NULL, "run mode,run = " << g_run);
				}
				if (c == 27)
				{
					break;
				}
			}
		}
	}
}