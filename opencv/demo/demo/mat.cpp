#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
namespace test {
	namespace mat {
		void create()
		{
			//create
			cv::Mat m0;
			m0.create(3, 10, CV_32FC3);//3通道的32位浮点数据
			m0.setTo(cv::Scalar(1.0f, 0.0f, 1.0f));//分别设置1、2、3通道的值

			//构造函数
			cv::Mat m(3, 10, CV_32FC3, cv::Scalar(1.0f, 0.0f, 1.0f));
			//复制
			cv::Mat m1(m);
			cv::Mat m2(m, cv::Range(0, 2), cv::Range(0, 2));//指定行列复制
			cv::Mat m3(m, cv::Rect(0, 0, 2, 2));//指定区域复制

			//静态函数
			cv::Mat m4 = cv::Mat::eye(5, 5, CV_32FC1);//单位矩阵
			//printf("element (3,3) is %f", m4.at<float>(3, 3));
			CV_LOG_INFO(NULL, "element (3,3) is " << m4.at<float>(3, 3));
		}
	}
}