#include <opencv2/opencv.hpp>
#include <iostream>
namespace test {
	/*
	* 查找用户指定尺寸的棋盘，尽可能多获取用户要求的完整图像（即，可以找到所有棋盘角点的图像）以及计算相机内在参数和畸变参数。
	* 使用时，需要在成功捕获图像后，大幅度改变棋盘视图。否则用于求解标定参数的点的矩阵可能形成一个病态（秩缺陷）矩阵，并且最终会得到一个不好的结果或根本没有结果
	*/
	namespace camera_calibration {
		using namespace std;

		void test(int argc, char** argv) {
			int n_boards = 0;
			float image_sf = 0.5f;
			float delay = 1.f;
			int board_w = 0;
			int board_h = 0;

			if (argc < 4 || argc>6) {
				cout << "\nerror: wrong number of input parameters";
				return;
			}
			board_w = atoi(argv[1]);
			board_h = atoi(argv[2]);
			n_boards = atoi(argv[3]);
			if (argc > 4) {
				delay = atoi(argv[4]);
			}
			if (argc > 5) {
				delay = atoi(argv[5]);
			}
			int board_n = board_w * board_h;
			cv::Size board_sz = cv::Size(board_w, board_h);

			cv::VideoCapture capture(0);
			if (!capture.isOpened()) {
				cout << "could not open the camera\n";
				return;
			}
			//allocate storage
			vector<vector<cv::Point2f>> image_points;//图像坐标
			vector<vector<cv::Point3f>> object_points;//物理坐标

			//capture corner views: loop until got n_boards successful
			//captures(all corners on the board are found)
			double last_captured_timestamp = 0;
			cv::Size image_size;

			while (image_points.size() < (size_t)n_boards) {
				cv::Mat image0, image;
				capture >> image0;
				image_size = image0.size();
				cv::resize(image0, image, cv::Size(), image_sf, image_sf, cv::INTER_LINEAR);

				//find the board
				vector<cv::Point2f> corners;
				bool foundAll = cv::findChessboardCorners(image, board_sz, corners);
				cout << "foundAll=" << foundAll << ",corners=" << corners.size() << "/" << n_boards << endl;
				//draw
				cv::drawChessboardCorners(image, board_sz, corners, foundAll);

				//if got a good board,add it to data
				double timestamp = (double)clock() / CLOCKS_PER_SEC;
				if (foundAll && timestamp - last_captured_timestamp > 1) {
					last_captured_timestamp = timestamp;
					image ^= cv::Scalar::all(255);

					cv::Mat mcorners(corners);//do not copy the data
					mcorners *= (1.f / image_sf);//scale the corner coordinates
					image_points.push_back(corners);
					object_points.push_back(vector<cv::Point3f>());
					vector<cv::Point3f>& opts = object_points.back();//创建引用
					opts.resize(board_n);
					for (int j = 0;j < board_n;j++) {
						opts[j] = cv::Point3f((float)(j / board_w), (float)(j % board_w), 0.f);
					}
					cout << "collected our " << (int)image_points.size() <<
						" of " << n_boards << " needed chessboard images\n" << endl;
				}
				cv::imshow("calibration", image);//show in color if we did collect the image
				if ((cv::waitKey(10) & 255) == 27) {
					return;
				}
			}
			//end collection while loop

			cv::destroyWindow("calibration");
			cout << "\n\n*** calibrating the camera...\n" << endl;

			//calibrate the camera
			cv::Mat intrinsic_matrix, distortion_coeffs;//相机内参，畸变系数
			double err = cv::calibrateCamera(
				object_points,
				image_points,
				image_size,
				intrinsic_matrix,
				distortion_coeffs,
				cv::noArray(),
				cv::noArray(),
				cv::CALIB_ZERO_TANGENT_DIST | cv::CALIB_FIX_PRINCIPAL_POINT
			);

			//save the intrinsic and distortions
			cout << " ***done!\n\nreprojection error is " << err
				<< "\nstoring intrinsics.xml and distortions.xml files\n\n" << endl;
			cv::FileStorage fs("intrinsics.xml", cv::FileStorage::WRITE);

			fs << "image_width" << image_size.width << "image_height" << image_size.height
				<< "camera_matrix" << intrinsic_matrix << "distortion_coefficients" << distortion_coeffs;
			fs.release();

			//example of loading theset matrics back in:
			fs.open("intrinsics.xml", cv::FileStorage::READ);
			cout << "\nimage width: " << (int)fs["image_width"];
			cout << "\nimage height: " << (int)fs["image_height"];

			cv::Mat intrinsic_matrix_loaded, distortion_coeffs_loaded;
			fs["camera_matrix"] >> intrinsic_matrix_loaded;
			fs["distortion_coefficients"] >> distortion_coeffs_loaded;
			cout << "\nintrinsic matrix:" << intrinsic_matrix_loaded;
			cout << "\ndistortion coefficients:" << distortion_coeffs_loaded << endl;

			//build undistort map which we will use for all subsequent frames
			cv::Mat map1, map2;
			cv::initUndistortRectifyMap(
				intrinsic_matrix_loaded,
				distortion_coeffs_loaded,
				cv::Mat(),
				intrinsic_matrix_loaded,
				image_size,
				CV_16SC2,
				map1,
				map2
			);

			//just run the camera to the screen,now showing the raw and the undistorted image
			for (;;) {
				cv::Mat image, image0;
				capture >> image0;
				if (image0.empty()) {
					break;
				}
				cv::remap(
					image0,
					image,
					map1,
					map2,
					cv::INTER_LINEAR,
					cv::BORDER_CONSTANT,
					cv::Scalar()
				);
				cv::imshow("undistorted", image);
				if ((cv::waitKey(30) & 255) == 27) {
					break;
				}
			}
		}
	}
}