//c++
#include <iostream>
//opencv2
#include<opencv2/core/utils/logger.hpp>
//mine
//#include "image.hpp"
//#include "video.hpp"
//#include "mat_demo.hpp"
//#include "fileopt.hpp"
//#include "window.hpp"
//#include "filter.hpp"
//#include "transform.hpp"
//#include "dft.hpp"
//#include "hough.hpp"
//#include "histogram.hpp"
//#include "contours.hpp"
//#include "bgr_extract.hpp"
//#include "operator_test.hpp"
//#include "bgr_extract.hpp"
//#include "keypoints.hpp"
#include "trace.hpp"

using namespace std;
using namespace cv::utils;

int main(int argc, char** argv)
{
	logging::setLogLevel(logging::LOG_LEVEL_VERBOSE);
	//test::image::show_image("C:/Users/Administrator/Desktop/todo-cpp.png");
	//test::video::open_camera();
	//test::video::video_play("test.mkv");
	//test::image::smooth_image("pp.jpg");
	//test::image::edge_detect("pp.jpg");
	//test::mat_demo::basics();
	//test::fileopt::file_storage_test();
	//test::window::window_test(argc, argv);
	//test::filter::filter_test(argc, argv);
	//test::transform::transform_test(argc, argv);
	//test::dft::dft_test(argc, argv);
	//test::hough::hough_test(argc, argv);
	//test::histogram::histogram_test(argc, argv);
	//test::contours::contours_test(argc, argv);
	//test::bgr_extract::bgr_extract_test(argc, argv);
	//test::operator_test::test(argc, argv);
	//test::contours::contours_test(argc, argv);
	//test::keypoints::keypoints_test(argc, argv);
	test::trace::trace_test(argc, argv);

	cout << endl;//为了刷新输出重定向缓冲区，避免写入文件后不完整
	return 0;

}



