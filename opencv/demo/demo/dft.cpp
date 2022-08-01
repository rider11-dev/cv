#include<opencv2/opencv.hpp>
#include <iostream>
namespace test {
	namespace dft {
		using namespace std;
		/// <summary>
		/// ∏µ¿Ô“∂±‰ªª≤‚ ‘
		/// </summary>
		/// <param name="argc"></param>
		/// <param name="argv"></param>
		void dft_test(int argc, char** argv) {
			if (argc != 2) {
				cout << "∏µ¿Ô“∂±‰ªª\nUsage: " << argv[0] << " <imagename>\n" << endl;
				return;
			}

			cv::Mat src = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);//ª“∂»ÕºœÒ
			if (src.empty()) { cout << "can not load " << argv[1] << endl;return; }

			cv::Mat A;
			cv::resize(src, A, cv::Size(0, 0), 0.5f, 0.5f);

			cv::Size patch_size(100, 100);
			cv::Point top_left(A.cols / 2, A.rows / 2);
			cv::Rect roi(top_left.x, top_left.y, patch_size.width, patch_size.height);
			cv::Mat B = A(roi);

			int dft_M = cv::getOptimalDFTSize(A.rows + B.rows - 1);
			int dft_N = cv::getOptimalDFTSize(A.cols + B.cols - 1);

			cv::Mat dft_A = cv::Mat::zeros(dft_M, dft_N, CV_32F);
			cv::Mat dft_B = cv::Mat::zeros(dft_M, dft_N, CV_32F);

			cv::Mat dft_A_part = dft_A(cv::Rect(0, 0, A.cols, A.rows));
			cv::Mat dft_B_part = dft_B(cv::Rect(0, 0, B.cols, B.rows));

			A.convertTo(dft_A_part, dft_A_part.type(), 1, -cv::mean(A)[0]);
			B.convertTo(dft_B_part, dft_B_part.type(), 1, -cv::mean(B)[0]);

			cv::dft(dft_A, dft_A, 0, A.rows);
			cv::dft(dft_B, dft_B, 0, B.rows);

			cv::mulSpectrums(dft_A, dft_B, dft_A, 0, true);
			cv::idft(dft_A, dft_A, cv::DFT_SCALE, A.rows + B.rows - 1);

			cv::Mat corr = dft_A(cv::Rect(0, 0, A.cols + B.cols - 1, A.rows + B.rows - 1));
			cv::normalize(corr, corr, 0, 1, cv::NORM_MINMAX, corr.type());
			cv::pow(corr, 3., corr);

			B ^= cv::Scalar::all(255);

			cv::imshow("‘≠Õº", A);
			cv::imshow("correlation", corr);

			cv::waitKey();
		}
	}
}