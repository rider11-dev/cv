#include <opencv2/opencv.hpp>
#include <iostream>
namespace test {
	/// <summary>
	/// 跟踪
	/// </summary>
	namespace trace {
		/// <summary>
		/// 卡尔曼滤波器
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void kalman_test(int argc, char** argv);
		void trace_test(int argc, char** argv) {
			kalman_test(argc, argv);
		}

		using namespace std;
#define phi2xy(mat) cv::Point(cvRound(img.cols/2+img.cols/3*cos(mat.at<float>(0))),cvRound(img.rows/2-img.cols/3*sin(mat.at<float>(0))))
		void kalman_test(int argc, char** argv) {

			cv::Mat img(500, 500, CV_8UC3);
			cv::KalmanFilter kalman(2, 1, 0);
			//随机数
			cv::Mat x_k(2, 1, CV_32F);
			cv::randn(x_k, 0., 0.1);
			//process noise
			cv::Mat w_k(2, 1, CV_32F);

			cv::Mat z_k = cv::Mat::zeros(1, 1, CV_32F);

			float F[] = { 1,1,0,1 };
			kalman.transitionMatrix = cv::Mat(2, 2, CV_32F, F).clone();
			//初始化卡尔曼滤波器其他参数
			cv::setIdentity(kalman.measurementMatrix, cv::Scalar(1));
			cv::setIdentity(kalman.processNoiseCov, cv::Scalar(1e-5));
			cv::setIdentity(kalman.measurementNoiseCov, cv::Scalar(1e-1));
			cv::setIdentity(kalman.errorCovPost, cv::Scalar(1));

			cv::randn(kalman.statePost, 0., 0.1);
			for (;;) {
				//预测位置
				cv::Mat y_k = kalman.predict();

				//生成测量
				cv::randn(z_k, 0., sqrt((double)kalman.measurementNoiseCov.at<float>(0, 0)));
				z_k = kalman.measurementMatrix * x_k + z_k;

				img = cv::Scalar::all(0);
				cv::circle(img, phi2xy(z_k), 4, cv::Scalar(128, 255, 255));//observed
				cv::circle(img, phi2xy(y_k), 4, cv::Scalar(255, 255, 255), 2);//predicted
				cv::circle(img, phi2xy(x_k), 4, cv::Scalar(0, 0, 255));//actual

				cv::imshow("Kalman", img);

				//调整滤波器state
				kalman.correct(z_k);

				cv::randn(w_k, 0., sqrt((double)kalman.processNoiseCov.at<float>(0, 0)));
				x_k = kalman.transitionMatrix * x_k + w_k;

				if (cv::waitKey(200) == 27) {
					break;
				}
			}
		}
	}
}