#include <opencv2/core.hpp>
#include <opencv2/core/utils/logger.hpp>
namespace test {
	namespace mat_demo {
		using namespace std;
		void at_test();
		void iterator_test();
		void nary_iterator_test();
		void sparse_mat_test();
		template<class T>
		void print_sparse_matrix(const cv::SparseMat_<T>& sm);

		void basics()
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

			at_test();
			iterator_test();
			nary_iterator_test();

			sparse_mat_test();
		}

		void at_test() {

			//访问数组元素
			//1通道单位矩阵
			cv::Mat m4 = cv::Mat::eye(5, 5, CV_32FC1);
			//printf("element (3,3) is %f", m4.at<float>(3, 3));
			CV_LOG_INFO(NULL, "element (3,3) is " << m4.at<float>(3, 3));

			//2通道单位矩阵，多维数组时，只有第一个通道会设置1，其余通道为0
			cv::Mat m5 = cv::Mat::eye(5, 5, CV_32FC2);
			CV_LOG_INFO(NULL, "element (4,4)[0] is " << m5.at<cv::Vec2f>(4, 4)[0]);
			CV_LOG_INFO(NULL, "element (4,4)[1] is " << m5.at<cv::Vec2f>(4, 4)[1]);
		}

		//迭代器
		void iterator_test() {
			int sz[3]{ 4,4,4 };
			cv::Mat m6(3, sz, CV_32FC3);//三维数组：4x4x4
			cv::randu(m6, -1.0f, 1.0f);//填充随机数字

			float max = 0.0f;
			//cv::MatConstIterator_<cv::Vec3f> it = m6.begin<cv::Vec3f>();
			auto it = m6.begin<cv::Vec3f>();
			while (it != m6.end<cv::Vec3f>()) {
				float len2 = (*it)[0] * (*it)[0] + (*it)[1] * (*it)[1] + (*it)[2] * (*it)[2];
				if (len2 > max) {
					max = len2;
				}
				it++;
			}
			CV_LOG_INFO(NULL, "max value: " << max);
		}
		//n-ary迭代器
		void nary_iterator_test() {
			const int n_mat_size = 5;
			const int n_mat_sz[] = { n_mat_size,n_mat_size,n_mat_size };
			cv::Mat n_mat0(3, n_mat_sz, CV_32FC1);
			cv::Mat n_mat1(3, n_mat_sz, CV_32FC1);

			cv::RNG rng;//随机数生成器
			rng.fill(n_mat0, cv::RNG::UNIFORM, 0.f, 1.f);
			rng.fill(n_mat1, cv::RNG::UNIFORM, 0.f, 1.f);

			//初始化NAryMat迭代器
			const cv::Mat* arrays[] = { &n_mat0,&n_mat1,0 };//数组必须以0或NULL终止
			cv::Mat my_planes[2];
			cv::NAryMatIterator it(arrays, my_planes);

			float s = 0.f;//所有planes求和
			int n = 0;//planes总数
			for (int p = 0;p < it.nplanes;p++, ++it) {
				s += cv::sum(it.planes[0])[0];//planes[0]包含来自n_mat0的一个面
				s += cv::sum(it.planes[1])[0];//planes[1]包含来自n_mat1的一个面
				n++;
			}
			CV_LOG_INFO(NULL, "sum=" << s << ",n=" << n);

		}
		//稀疏矩阵
		void sparse_mat_test()
		{
			std::cout << "==============sparse_mat_test================" << std::endl;
			int size[]{ 10,10 };
			cv::SparseMat sm(2, size, CV_32F);
			cv::SparseMat_<float> sm1(2, size);

			//填充数组
			for (int i = 0;i < 10;i++) {
				int idx[2];
				idx[0] = size[0] * rand();
				idx[1] = size[1] * rand();

				sm.ref<float>(idx) += 1.0f;
				sm1.ref(idx) += 2.0f;
			}
			//打印非0元素
			cv::SparseMatConstIterator_<float> it = sm.begin<float>();
			cv::SparseMatConstIterator_<float> it_end = sm.end<float>();

			for (;it != it_end;++it) {
				const cv::SparseMat::Node* node = it.node();
				//CV_LOG_INFO(NULL, "(" << node->idx[0] << "," << node->idx[1] << ") " << *it);
				printf(" (%3d,%3d) %f\n", node->idx[0], node->idx[1], *it);
			}


			print_sparse_matrix(sm1);

		}
		//模板结构
		template<typename T>
		void print_sparse_matrix(const cv::SparseMat_<T>& sm) {
			std::cout << "==============print_sparse_matrix================" << std::endl;

			cv::SparseMatConstIterator_<T> it = sm.begin();
			cv::SparseMatConstIterator_<T> it_end = sm.end();

			for (;it != it_end;++it) {
				const typename cv::SparseMat_<T>::Node* node = it.node();
				cout << "(" << node->idx[0] << "," << node->idx[1] << ") = " << *it << endl;
			}
		}
	}
}