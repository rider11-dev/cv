#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
namespace test {
	namespace mat {
		void create()
		{
			//create
			cv::Mat m0;
			m0.create(3, 10, CV_32FC3);//3ͨ����32λ��������
			m0.setTo(cv::Scalar(1.0f, 0.0f, 1.0f));//�ֱ�����1��2��3ͨ����ֵ

			//���캯��
			cv::Mat m(3, 10, CV_32FC3, cv::Scalar(1.0f, 0.0f, 1.0f));
			//����
			cv::Mat m1(m);
			cv::Mat m2(m, cv::Range(0, 2), cv::Range(0, 2));//ָ�����и���
			cv::Mat m3(m, cv::Rect(0, 0, 2, 2));//ָ��������

			//��̬����
			cv::Mat m4 = cv::Mat::eye(5, 5, CV_32FC1);//��λ����
			//printf("element (3,3) is %f", m4.at<float>(3, 3));
			CV_LOG_INFO(NULL, "element (3,3) is " << m4.at<float>(3, 3));
		}
	}
}