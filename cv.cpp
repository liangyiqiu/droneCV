#include "cv.hpp"

int width=160;
int height=120;
int wait_time=30;
int morphology_flag=0;
int morphology_size=6;

Mat frame;
Mat src_image;
Mat output_image;
Mat gray_image;
Mat threshold_image;
Mat blur_image;
int blur_size=3;
Mat canny_image;
int canny_thresh=160;

int task_flag=STANDBY;

void* task0(void* argv)
{
    VideoCapture capture(0);
    //capture.set(CV_CAP_PROP_FRAME_WIDTH,width);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT,height);

    Rect box;
    box.width=height;
    box.height=height;
    box.x=(width-height)/2-1;
    box.y=0;

    while(1)
    {
        capture>>frame;
        resize(frame, frame, Size(0, 0), width/ 640.0, height / 480.0, 0);
        Mat temp(frame,box);
        src_image=frame;
        if(sys.boot_mode==DEBUG_MODE)
            imshow("src",src_image);

        cvtColor(src_image, gray_image, COLOR_BGR2GRAY);
        blur(gray_image, blur_image, Size(blur_size, blur_size));

        switch(task_flag)
        {
        case STANDBY:output_image=src_image;break;
        case GROUND:task_ground();break;
        case POINT:task_point();break;
        case LINE:task_line();break;
        }

        waitKey(wait_time);
    }
}

void task_ground()
{
    pilot.z=0;
    static int stop_flag=0;
    static Timer timer(0,5,&stop_flag);
    timer.start();
    if(stop_flag==1)
    {
        pilot.z=100;
        task_flag=1;
    }
}

void task_point()
{   

    pilot.z=100;

    static int stop_flag=0;
    if(pilot.z_now>90)
    {
        static Timer timer(0,10,&stop_flag);
        timer.start();
    }
    if(stop_flag==1)
        pilot.z=0;


    morphology_flag=1;
    if(morphology_flag==1)
    {
        Mat dilate_kernel = getStructuringElement(MORPH_ELLIPSE,Size(morphology_size,morphology_size));
        threshold(blur_image,threshold_image,50,255,THRESH_BINARY);
        if(sys.boot_mode==DEBUG_MODE)
            imshow("threshold",threshold_image);
        dilate(threshold_image,blur_image,dilate_kernel);
    }

    Canny(blur_image,canny_image,canny_thresh,canny_thresh*2,3);
    if(sys.boot_mode==DEBUG_MODE)
        imshow("canny",canny_image);


    //定义一些参数
    double maxArea = 0;
    vector<cv::Point> maxContour;
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;

    // 使用Threshold检测边缘
    //	threshold(g_grayImage, threshold_output, g_nThresh, 255, THRESH_BINARY);
    //	imshow("threshold", threshold_output);

    // 找出轮廓
    findContours(canny_image, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

    // 获取矩形
    Rect maxRect;

    //一个循环，遍历所有部分，进行本程序最核心的操作
    if(contours.size()>0)
    {
        for (unsigned int i = 0; i < contours.size(); i++)
        {

            //find the max coutour
            double area = (contourArea(contours[i]));
            if (area > maxArea && area < (src_image.cols - 5)*(src_image.rows - 0))//avoid the edge contour of image
            {
                maxArea = area;
                maxContour = contours[i];
            }
        }

        if (maxArea > 0)
        {
            maxRect = boundingRect(Mat(maxContour));//计算点集的最外面（up-right）矩形边界

            if (maxRect.width - maxRect.height<10 && maxRect.width - maxRect.height>-10)
            {
                // 包围的矩形框
                //Mat drawing = Mat::zeros(threshold_output.size(), CV_8UC3);
                Mat drawing = src_image;
                rectangle(drawing, maxRect.tl(), maxRect.br(), Scalar(0, 0, 255), 2, 8, 0);//draw rectangle

                //find center
                Point2f center;
                center.x = maxRect.x + maxRect.width / 2;
                center.y = maxRect.y + maxRect.height / 2;

                //            printf("CENTER(w:%.0f,h:%.0f)\n", center.x, center.y);//print center
                circle(drawing, center, 2, Scalar(255, 0, 0), 2, 8, 0);//draw center
                stringstream ss;
                ss<<"w: "<<center.x<<" h: "<<center.y;
                putText(drawing,ss.str(),Point(5,110),FONT_HERSHEY_PLAIN,1,Scalar(0,0,255),1,8,false);
                output_image=drawing;

                pilot.x=center.x;
                pilot.y=center.y;
            }
            else
            {
                //            cout << "too low\n";
                output_image=src_image;
                putText(src_image,"NO OBS",Point(10,110),FONT_HERSHEY_PLAIN,1,Scalar(0,0,255),1,8,false);
            }
        }
    }
    else
    {
        output_image=src_image;
        putText(src_image,"TOO LOW",Point(10,110),FONT_HERSHEY_PLAIN,1,Scalar(0,0,255),1,8,false);
    }
}

void task_line()
{


    //    //exit this task when a circle is detected
    //    vector<Vec3f>circles;
    //    HoughCircles(canny_image,circles,HOUGH_GRADIENT,1,200,160,22.0,15,1000);
    //    for(size_t i=0; i<circles.size();i++)
    //    {
    //        Point center(cvRound(circles[i][0]),cvRound(circles[i][1]));
    //        int radius = cvRound(circles[i][2]);
    //        circle(g_srcImage,center,3,Scalar(0,255,0),-1,8,0);
    //        circle(g_srcImage,center,radius,Scalar(155,50,255),3,8,0);
    //    }
    //    if(circles.size()>0)//reach the circle,change into 'find' mode
    //    {
    //        x_ave=59;
    //        y_ave=59;
    //        mode=4;
    //    }
    //    else//not reach the circle, find line
    //    {
    //        y_ave=30;


    //        float ratio = 0;
    //        int x_sum=0;
    //        int x_sum_number=0;
    //        static int x_ave = 59;
    //        int y_sum = 0;
    //        int y_sum_number = 0;
    //        static int y_ave = 59;

    //        vector<Vec4i> lines;
    //        vector<Point> points;
    //        Vec4i fline;

    //        HoughLinesP(canny_image, lines, 1.0, CV_PI / 180, 25, 3, 3);

    //        if (lines.size() > 0)//if any line exists
    //        {
    //            for (size_t i = 0; i < lines.size(); i++)
    //            {
    //                int x1 = lines[i][0],
    //                        y1 = lines[i][1],
    //                        x2 = lines[i][2],
    //                        y2 = lines[i][3];
    //                ratio = ((float)y2 - y1) / (x2 - x1);
    //                if (ratio > 1 || ratio < -1)
    //                {
    //                    x_sum += x1 + x2;
    //                    x_sum_number+=2;
    //                    points.push_back(Point(x1,y1));
    //                    points.push_back(Point(x2,y2));
    //                    circle(g_srcImage, points[x_sum_number-1], 1, Scalar(255,0, 0));
    //                    circle(g_srcImage, points[x_sum_number-2], 1, Scalar(255, 0, 0));
    //                    line(g_srcImage, points[x_sum_number -2], points[x_sum_number -1], Scalar(0, 0, 255), 1, LINE_AA);
    //                }
    //                else
    //                {
    //                    y_sum += y1 + y2;
    //                    y_sum_number += 2;
    //                }
    //            }
    //            if (x_sum_number > 4)
    //                x_ave = x_sum / x_sum_number;

    //            //if (y_sum_number > 4)
    //            //y_ave = y_sum / y_sum_number;

    //            //cout << "("<<x_ave<<","<<y_ave<<")" <<"		"<<ratio<< endl;
    //            circle(g_srcImage, Point(x_ave, y_ave), 1, Scalar(0, 255, 255));

    //            //if(points.size()>0)
    //            //fitLine(points,fline,DIST_HUBER,0,1e-2,1e-2);
    //            //line(g_srcImage, Point(fline[0], fline[1]), Point(fline[2], fline[3]), Scalar(186, 0, 0), 1, LINE_AA);
    //        }
    //    }
    //    serial_transmit(fd,x_ave,y_ave,goal_height);
    //    stringstream ss;
    //    ss<<"w: "<<x_ave<<" h: "<<y_ave;
    //    putText(g_srcImage,ss.str(),Point(5,110),FONT_HERSHEY_PLAIN,1,Scalar(0,0,255),1,8,false);
    //    output_image = g_srcImage;



    //    //??4??????????л??????????
    //    //	for (size_t i = 0; i < lines.size(); i++)
    //    //	{
    //    //		Vec4i l = lines[i];
    //    //line(g_srcImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 1, LINE_AA);
    //    //	circle(g_srcImage, Point(l[0], l[1]), 1, Scalar(0, 0, 255));
    //    //	circle(g_srcImage, Point(l[2], l[3]), 1, Scalar(0, 0, 255));
    //    //	points.push_back( Point(l[0], l[1]));
    //    //	points.push_back(Point(l[2], l[3]));
    //    //	}
    //    //	fitLine(points, fline, cv::DIST_L2, 0, 1e-2, 1e-2);
    //    //	line(g_srcImage, Point(fline[0], fline[1]), Point(fline[2], fline[3]), Scalar(186, 0, 0), 1, LINE_AA);
    //    //imshow("lines", g_srcImage);

}
