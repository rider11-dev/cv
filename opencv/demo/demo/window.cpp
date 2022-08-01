#include <opencv2/opencv.hpp>
namespace test {
	namespace window {
		using namespace std;
		//#define MOUSE_TEST
#define TRACKBAR_TEST

#ifdef MOUSE_TEST
		void my_mouse_callback(int event, int x, int y, int flags, void* param);
		void draw_box(cv::Mat& img, cv::Rect box);
		void mouse_callback_test();

		cv::Rect box;
		bool drawing_box = false;
#endif

#ifdef TRACKBAR_TEST
		//使用滚动条模拟实现开关
		int g_switch_value = 1;
		void switch_off_function() { cout << "pause\n"; }
		void switch_on_function() { cout << "run\n"; }
		void switch_callback(int position, void*);
		void help() {
			cout << "call:test.mkv" << endl;
			cout << "shows putting a pause button in a video." << endl;
		}
		void trackbar_test(int argc, char** argv);
#endif

		void window_test(int argc, char** argv) {
#ifdef MOUSE_TEST
			mouse_callback_test();//鼠标回调测试
#endif


#ifdef TRACKBAR_TEST
			trackbar_test(argc, argv);//滚动条/开关测试
#endif
		}
		//鼠标回调测试相关
#ifdef MOUSE_TEST
		void mouse_callback_test() {
			box = cv::Rect(-1, -1, 0, 0);
			cv::Mat image(200, 200, CV_8UC3), temp;
			image.copyTo(temp);

			box = cv::Rect(-1, -1, 0, 0);
			image = cv::Scalar::all(0);
			string wName = "box_example";
			cv::namedWindow(wName);

			cv::setMouseCallback(wName, my_mouse_callback, (void*)&image);
			for (;;) {
				image.copyTo(temp);
				if (drawing_box) {
					draw_box(temp, box);
				}
				cv::imshow(wName, temp);
				if (cv::waitKey(15) == 27) {//esc
					break;
				}
			}
		}

		void my_mouse_callback(int event, int x, int y, int flags, void* param) {
			cv::Mat image = *(cv::Mat*)param;
			switch (event) {
			case cv::EVENT_MOUSEMOVE: {
				//cout << "鼠标移动" << endl;
				if (drawing_box) {
					cout << "绘制box" << endl;
					box.width = x - box.x;
					box.height = y - box.y;
				}
			}
									break;
			case cv::EVENT_LBUTTONDOWN: {
				cout << "鼠标左键按下:drawing_box => true" << endl;
				drawing_box = true;
				box = cv::Rect(x, y, 0, 0);
			}
									  break;
			case cv::EVENT_LBUTTONUP: {
				cout << "鼠标左键释放:drawing_box => false" << endl;
				drawing_box = false;
				if (box.width < 0) {
					box.x += box.width;
					box.width *= -1;
				}
				if (box.height < 0) {
					box.y += box.height;
					box.y *= -1;
				}
				draw_box(image, box);
			}
									break;
			}
		}
		void draw_box(cv::Mat& img, cv::Rect box) {
			cout << "绘制box" << endl;
			cv::rectangle(img, box.tl(), box.br(), cv::Scalar(0x00, 0x00, 0xff));//red
		}
#endif

		//滚动条测试相关
#ifdef TRACKBAR_TEST
		void switch_callback(int position, void*) {
			if (position == 0) {
				switch_off_function();
			}
			else {
				switch_on_function();
			}
		}
		void trackbar_test(int argc, char** argv) {
			cv::Mat frame;
			cv::VideoCapture g_capture;
			help();
			if (argc < 2 || !g_capture.open(argv[1])) {
				cout << "打开视频文件失败:" << argv[1] << endl << endl;
				return;
			}
			string wName = "trackbar_test";
			cv::namedWindow(wName);

			cv::createTrackbar("switch", wName, &g_switch_value, 1, switch_callback);

			for (;;) {
				if (g_switch_value) {
					g_capture >> frame;
					if (frame.empty()) {
						break;
					}
					cv::imshow(wName, frame);
				}
				if (cv::waitKey(10) == 27) {
					break;
				}
			}
		}
#endif
	}
}