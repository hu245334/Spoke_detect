//#include <iostream>
//#include <opencv2/opencv.hpp>
//#include <opencv2/core/core.hpp>    
//#include <opencv2/highgui/highgui.hpp>
//#include <cstring>
//#include "Entropy.h"
//
//using namespace std;
//using namespace cv;
//
//Mat org, img, dst;
//Mat srcImage, grayImage, dstImage;
//
//int alpha = 1.1; /*duibidu */
//int beta = -70;  /*liangdu*/
//// ���ȶԱȶȹ�ʽ��g = alpha*f+beta ��gΪ����������ֵ��fΪԭ����ֵ
//
//Point point[10] = { (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1) };//�ܼ���ͼ�б��10��������
//Point local[2] = { (-1, -1), (-1, -1) };
//int width, height;
//
//void on_mouse(int event, int x, int y, int flags, void *ustc)//flags���������ק�¼�
//{
//	static Point pre_pt = Point(-1, -1);
//	static Point cur_pt = Point(-1, -1);
//	static int idx = 0;
//	static int mark = 0;
//	if (event == CV_EVENT_LBUTTONDOWN)//�������
//	{
//		org.copyTo(img);
//		pre_pt = Point(x, y);
//		cur_pt = Point(x, y);
//		if (idx<10){
//			point[idx] = Point(x, y);
//			idx++;
//		}
//	}
//	else if (event == CV_EVENT_MOUSEMOVE)//����ƶ�
//	{
//		if (idx < 10 && idx>0){//��껹û��
//			org.copyTo(img);
//			cur_pt = Point(x, y);
//			for (int i = 0; i < idx - 1; i++){
//				line(img, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
//			}
//			line(img, point[idx - 1], cur_pt, Scalar(0, 255, 0), 1, 8, 0);
//			imshow("img", img);
//		}
//	}
//	else if (event == CV_EVENT_LBUTTONUP)//�ſ����
//	{
//		org.copyTo(img);
//		pre_pt = Point(x, y);
//		cur_pt = Point(x, y);
//		for (int i = 0; i < idx - 1; i++){
//			line(img, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
//		}
//		if (idx >9){//�Ѿ���10������
//			line(img, point[9], point[0], Scalar(0, 255, 0), 1, 8, 0);
//
//			//���10�����Χ�ķ�Χ�������Сֵ
//			Point min, max;
//			for (int i = 0; i < 10; i++){
//				if (0 == i){
//					max.x = min.x = point[i].x;
//					max.y = min.y = point[i].y;
//				}
//				else
//				{
//					if (max.x < point[i].x){
//						max.x = point[i].x;
//					}
//					if (min.x > point[i].x){
//						min.x = point[i].x;
//					}
//					if (max.y < point[i].y){
//						max.y = point[i].y;
//					}
//					if (min.y > point[i].y){
//						min.y = point[i].y;
//					}
//				}
//			}
//
//			//����Χ�ֱ���������һ��
//			if (min.x - 3 > 0){
//				min.x = min.x - 3;
//			}
//			else{
//				min.x = 0;
//			}
//			if (min.y - 3 > 0){
//				min.y = min.y - 3;
//			}
//			else{
//				min.y = 0;
//			}
//			if (max.x + 3 < org.cols){
//				max.x = max.x + 3;
//			}
//			else{
//				max.x = org.cols - 1;
//			}
//			if (max.y + 3 < org.rows){
//				max.y = max.y + 3;
//			}
//			else{
//				max.y = org.rows - 1;
//			}
//
//			width = abs(max.x - min.x);
//			height = abs(max.y - min.y);
//			if (width == 0 || height == 0)
//			{
//				return;
//			}
//			dst = org(Rect(min.x, min.y, width, height));
//			local[0].x = min.x;
//			local[0].y = min.y;
//		}
//		imshow("img", img);
//	}
//	if (idx > 9){
//		if (0 == mark){
//			org.copyTo(img);
//			for (int i = 0; i < idx - 1; i++){
//				line(img, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
//			}
//			line(img, point[9], point[0], Scalar(0, 255, 0), 1, 8, 0);
//
//
//			mark = 1;
//		}
//	}
//}
//
//int main()
//{
//	//����ͼ��
//	srcImage = imread("PHTO\\Hough\\A008�ַ�-18.bmp");//����ͼ���ַ
//
//	//�������ڴ�ŶԱȶȺ����ȱ仯���ͼ��
//	Mat ThresholdImage = Mat::zeros(srcImage.size(), srcImage.type());
//
//	//�����������ص㣬�ı����ȺͶԱȶ�
//	for (int y = 0; y < srcImage.rows; y++)
//	{
//		for (int x = 0; x < srcImage.cols; x++)
//		{
//			for (int c = 0; c < 3; c++)
//			{
//				ThresholdImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(srcImage.at<Vec3b>(y, x)[c]) + beta);
//			}
//		}
//	}
//
//	cvtColor(ThresholdImage, grayImage, COLOR_BGR2GRAY); //ת���Ҷ�ͼ��
//	GaussianBlur(grayImage, grayImage, Size(3, 3), 1);  //��˹�˲��� ȥ����ƽ������
//
//	//�����񻯺�
//	Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
//
//	//�񻯣���߱�Եϸ��
//	filter2D(grayImage, dstImage, grayImage.depth(), kernel);
//
//
//
//	//�������ڴ�ŶԱȶȺ����ȱ仯���ͼ��
//	dstImage.copyTo(org);
//	org.copyTo(img);
//	namedWindow("img");
//	setMouseCallback("img", on_mouse, 0);
//	imshow("img", org);
//	//�ȴ���ͼ����
//	while (1)
//	{
//		int key;
//		key = waitKey(20);
//		if (char(key) == 27)
//		{
//			break;
//		}
//	}
//
//	Point left_up, right_down;
//	left_up = point[0];
//	right_down = point[0];
//	for (int i = 1; i < 10; i++){
//		if (point[i].x < left_up.x){
//			left_up.x = point[i].x;
//		}
//		if (point[i].y < left_up.y){
//			left_up.y = point[i].y;
//		}
//		if (point[i].x>right_down.x){
//			right_down.x = point[i].x;
//		}
//		if (point[i].y>right_down.y){
//			right_down.y = point[i].y;
//		}
//	}
//	Rect rect(left_up.x,left_up.y,right_down.x-left_up.x, right_down.y-left_up.y);
//	Mat image_roi = dstImage(rect);
//
//
//
//	Mat IMG = imread("PHTO\\Hough\\A008�ַ�-2.bmp");//����ͼ���ַ
//	ThresholdImage = Mat::zeros(IMG.size(), IMG.type());
//	//�����������ص㣬�ı����ȺͶԱȶ�
//	for (int y = 0; y < IMG.rows; y++){
//		for (int x = 0; x < IMG.cols; x++){
//			for (int c = 0; c < 3; c++){
//				ThresholdImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(IMG.at<Vec3b>(y, x)[c]) + beta);
//			}
//		}
//	}
//	cvtColor(ThresholdImage, grayImage, COLOR_BGR2GRAY); //ת���Ҷ�ͼ��
//	GaussianBlur(grayImage, grayImage, Size(3, 3), 1);  //��˹�˲��� ȥ����ƽ������
//	kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
//	Mat dstIMG;
//	filter2D(grayImage, dstIMG, grayImage.depth(), kernel);
//
//
//	double *MI = new double[(dstIMG.rows - image_roi.rows + 1)*(dstIMG.cols - image_roi.cols + 1)];
//	for (int j = 0; j <= (dstIMG.rows - image_roi.rows); j++){
//		for (int i = 0; i <= (dstIMG.cols - image_roi.cols); i++){
//			rect=Rect(i, j, image_roi.cols, image_roi.rows);
//			Mat ROI = dstIMG(rect);
//			MI[j*(dstIMG.cols - image_roi.cols) + i] = ComEntropy(image_roi, ROI);
//		}
//	}
//
//
//	//�ҳ�����Ϣ�������꣬˵������ƥ��
//	double MAX = MI[0];
//	Point PMAX = (0, 0);
//	for (int j = 0; j <= (dstIMG.rows - image_roi.rows); j++){
//		for (int i = 0; i <= (dstIMG.cols - image_roi.cols); i++){
//			if (MAX < MI[j*(dstIMG.cols - image_roi.cols) + i]){
//				MAX = MI[j*(dstIMG.cols - image_roi.cols) + i];
//				PMAX.x = i;
//				PMAX.y = j;
//			}
//		}
//	}
//
//	Point dis=PMAX-left_up;
//	
//	for (int i = 0; i < 9; i++){
//		line(IMG, point[i] + dis, point[i + 1] + dis, Scalar(0, 255, 0), 1, 8, 0);
//	}
//	line(IMG, point[9] + dis, point[0] + dis, Scalar(0, 255, 0), 1, 8, 0);
//	imshow();
//	rect = Rect(PMAX.x, PMAX.y, image_roi.cols, image_roi.rows);
//	Mat ROI = dstIMG(rect);
//
//	imshow("��ƥ��", ROI);
//
//
//
//	// ESC->EXIT
//	while (1)
//	{
//		int key;
//		key = waitKey(20);
//		if (char(key) == 27)
//		{
//			break;
//		}
//	}
//
//	return 0;
//}
//
//
//
//
//
//
