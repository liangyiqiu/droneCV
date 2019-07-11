#ifndef CV_HPP
#define CV_HPP

#include<string>
#include "opencv2/opencv.hpp"
#include "sys.hpp"

using namespace cv;
using namespace std;

extern int width;
extern int height;
extern int wait_time;
extern int morphology_flag;
extern int morphology_size;

extern Mat frame;
extern Mat src_image;
extern Mat output_image;
extern Mat gray_image;
extern Mat threshold_image;
extern Mat blur_image;
extern int blur_size;
extern Mat canny_image;
extern int canny_thresh;

#define STANDBY (-1)
#define GROUND 0
#define POINT 1
#define LINE 2
extern int task_flag;

extern void* task0(void* argv);
extern void* task1(void* argv);

extern void task_ground();
extern void task_point();
extern void task_line();

#endif // CV_HPP
