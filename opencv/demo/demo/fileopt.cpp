#include <opencv2/opencv.hpp>
#include <time.h>
using namespace std;
namespace test {
	namespace fileopt {
		void write_yaml(const string file);
		void read_yaml(const string file);

		void file_storage_test() {
			std::cout << "==============file_storage_test begin==============" << std::endl;
			string file("test.yml");
			write_yaml(file);
			read_yaml(file);
			std::cout << "==============file_storage_test end==============" << std::endl;
		}


		void write_yaml(const string file) {
			std::cout << "==============write_yaml begin==============" << std::endl;
			cv::FileStorage fs(file, cv::FileStorage::WRITE);
			fs << "frameCount" << 5;

			time_t raw_time;
			time(&raw_time);
			fs << "calibrationDate" << asctime(localtime(&raw_time));

			cv::Mat cameraMatrix = (
				cv::Mat_<double>(3, 3)
				<< 1000, 0, 320, 0, 1000, 240, 0, 0, 1
				);
			cv::Mat distCoeffs = (
				cv::Mat_<double>(5, 1)
				<< 0.1, 0.01, -0.001, 0, 0
				);
			fs << "cameraMatrix" << cameraMatrix << "distCoeffs" << distCoeffs;

			fs << "features" << "[";
			for (int i = 0;i < 3;i++) {
				int x = rand() % 640;
				int y = rand() % 480;
				uchar lbp = rand() % 256;

				fs << "{:" << "x" << x << "y" << y << "lbp" << "[:";
				for (int j = 0;j < 8;j++) {
					fs << ((lbp >> j) & 1);
				}
				fs << "]" << "}";
			}
			fs << "]";
			fs.release();
			std::cout << "==============write_yaml end==============" << std::endl;
		}

		void read_yaml(const string file) {
			std::cout << "==============read_yaml begin==============" << std::endl;
			cv::FileStorage fs(file, cv::FileStorage::READ);
			int frameCount = (int)fs["frameCount"];//使用FileNode的type操作符

			string date;
			fs["calibrationDate"] >> date;//使用FileNode操作符>>

			cv::Mat cameraMatrix, distCoeffs;
			fs["cameraMatrix"] >> cameraMatrix;
			fs["distCoeffs"] >> distCoeffs;

			cout << "frameCount: " << frameCount << endl;
			cout << "calibrationDate: " << date << endl;
			cout << "cameraMatrix: " << cameraMatrix << endl;
			cout << "distCoeffs: " << distCoeffs << endl;

			cv::FileNode features = fs["features"];
			cv::FileNodeIterator it = features.begin(), it_end = features.end();
			int idx = 0;
			std::vector<uchar> lbpval;

			//遍历sequence
			for (;it != it_end;++it, idx++) {
				cout << "feature #" << idx << ": ";
				cout << "x=" << (int)(*it)["x"] << ", y=" << (int)(*it)["y"] << ", lbp: (";
				(*it)["lbp"] >> lbpval;
				for (int i = 0;i < (int)lbpval.size();i++) {
					cout << " " << (int)lbpval[i];
				}
				cout << ")" << endl;
			}
			fs.release();
			std::cout << "==============read_yaml end==============" << std::endl;
		}
	}
}