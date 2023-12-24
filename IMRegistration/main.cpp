#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>    
#include <opencv2/highgui/highgui.hpp>
#include <cstring>        // for strcat()
#include <io.h>
#include <direct.h>
#include "Entropy.h"
#include "De_Extraction.h"

using namespace std;
using namespace cv;


Mat org, img, dst;
Point point[10] = { (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1), (-1, -1) };//�ܼ���ͼ�б��10��������
double alpha = 1.1; /*duibidu */
int beta = -70;  /*liangdu*/
// ���ȶԱȶȹ�ʽ��g = alpha*f+beta ��gΪ����������ֵ��fΪԭ����ֵ

//��ͼƬ��·��
char path[100] = "PHTO\\*";
char openpath[100];
_finddata_t findData;

void on_mouse(int event, int x, int y, int flags, void *ustc)//flags���������ק�¼�
{
	static Point pre_pt = Point(-1, -1);
	static Point cur_pt = Point(-1, -1);
	static int idx = 0;
	static int mark = 0;
	if (event == CV_EVENT_LBUTTONDOWN)//�������
	{
		org.copyTo(img);
		pre_pt = Point(x, y);
		cur_pt = Point(x, y);
		if (idx<10){
			point[idx] = Point(x, y);
			idx++;
		}
	}
	else if (event == CV_EVENT_MOUSEMOVE)//����ƶ�
	{
		if (idx < 10 && idx>0){//��껹û��
			org.copyTo(img);
			cur_pt = Point(x, y);
			for (int i = 0; i < idx - 1; i++){
				line(img, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
			}
			line(img, point[idx - 1], cur_pt, Scalar(0, 255, 0), 1, 8, 0);
			imshow("img", img);
		}
	}
	else if (event == CV_EVENT_LBUTTONUP)//�ſ����
	{
		org.copyTo(img);
		pre_pt = Point(x, y);
		cur_pt = Point(x, y);
		for (int i = 0; i < idx - 1; i++){
			line(img, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
		}
		if (idx >9){//�Ѿ���10������
			line(img, point[9], point[0], Scalar(0, 255, 0), 1, 8, 0);
		}
		imshow("img", img);
	}
	if (idx > 9){
		if (0 == mark){
			mark = 1;
		}
	}
}



int main(){
	//����ͼ��
	int mark = 0;//����Ƿ��ǵ�һ��ͼƬ
	intptr_t handle;

	//ʹ�û���Ϣ������׼����
	//��pathĿ¼���½�mutual_info�ļ���
	strcpy_s(openpath, path);
	openpath[strlen(openpath) - 1] = '\0';
	strcat_s(openpath, "mutual_info\\");
	_mkdir(openpath);

	handle = _findfirst(path, &findData);    // ����Ŀ¼�еĵ�һ���ļ�
	if (handle == -1){
		cout << "�򿪵�һ���ļ�ʧ��!\n";
		return -1;
	}

	Point left_up, right_down;
	Rect rect;
	Mat image_roi;
	double image_roi_entropy;
	do{
		if (findData.attrib & _A_SUBDIR){//�Ƿ�����Ŀ¼����"."��".."
			cout << findData.name << "\t<dir>\n";
		}
		else{
			strcpy_s(openpath, path);
			openpath[strlen(openpath) - 1] = '\0';
			strcat_s(openpath, findData.name);
			if (0 == mark){
				mark = 1;//�Ѿ�������һ��ͼƬ
				cout << openpath << endl;
				Mat srcImage = imread(openpath);//����ͼ���ַ
				//imshow("srcImage", srcImage); //��ʾԭͼ

				//�������ڴ�ŶԱȶȺ����ȱ仯���ͼ��
				Mat ThresholdImage = Mat::zeros(srcImage.size(), srcImage.type());

				//�����������ص㣬�ı����ȺͶԱȶ�
				for (int y = 0; y < srcImage.rows; y++)
				{
					for (int x = 0; x < srcImage.cols; x++)
					{
						for (int c = 0; c < 3; c++)
						{
							ThresholdImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(srcImage.at<Vec3b>(y, x)[c]) + beta);
						}
					}
				}

				Mat grayImage;
				//cvtColor(ThresholdImage, grayImage, COLOR_BGR2GRAY);   //ת���Ҷ�ͼ��
				GaussianBlur(ThresholdImage, grayImage, Size(3, 3), 1);  //��˹�˲��� ȥ����ƽ������

				//�����񻯺�
				Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

				Mat dstImage;
				//�񻯣���߱�Եϸ��
				filter2D(grayImage, dstImage, grayImage.depth(), kernel);

				//�������ڴ�ŶԱȶȺ����ȱ仯���ͼ��
				dstImage.copyTo(org);
				namedWindow("img");
				setMouseCallback("img", on_mouse, 0);
				imshow("img", org);

				//�ȴ���ͼ����
				while (1)
				{
					int key;
					key = waitKey(20);
					if (char(key) == 27)
					{
						break;
					}
				}
				
				left_up = point[0];
				right_down = point[0];
				for (int i = 1; i < 10; i++){
					if (point[i].x < left_up.x){
						left_up.x = point[i].x;
					}
					if (point[i].y < left_up.y){
						left_up.y = point[i].y;
					}
					if (point[i].x>right_down.x){
						right_down.x = point[i].x;
					}
					if (point[i].y>right_down.y){
						right_down.y = point[i].y;
					}
				}
				rect = Rect(left_up.x, left_up.y, right_down.x - left_up.x, right_down.y - left_up.y);
				dstImage(rect).copyTo(image_roi);

				image_roi_entropy = Entropy(image_roi);

				Mat tmp_image_roi;
				//��ԭͼ�Ͻ��и���Ȥ����
				srcImage(rect).copyTo(tmp_image_roi);
				////�ҳ�ROI����󣬶�ROI���򣨾��Σ����жԱȶ���ǿ��ͨ���Ҷ�������ʵ��
				int POINT_MAX[3] = { tmp_image_roi.at<Vec3b>(0, 0)[0], tmp_image_roi.at<Vec3b>(0, 0)[1], tmp_image_roi.at<Vec3b>(0, 0)[2] };
				int POINT_MIN[3] = { tmp_image_roi.at<Vec3b>(0, 0)[0], tmp_image_roi.at<Vec3b>(0, 0)[1], tmp_image_roi.at<Vec3b>(0, 0)[2] };
				for (int y = 0; y < tmp_image_roi.rows; y++){
					for (int x = 0; x < tmp_image_roi.cols; x++){
						for (int c = 0; c < 3; c++){
							if (POINT_MAX[c] < tmp_image_roi.at<Vec3b>(y, x)[c]){
								POINT_MAX[c] = tmp_image_roi.at<Vec3b>(y, x)[c];
							}
							if (POINT_MIN[c]>tmp_image_roi.at<Vec3b>(y, x)[c]){
								POINT_MIN[c] = tmp_image_roi.at<Vec3b>(y, x)[c];
							}
						}
					}
				}
				//cout << "POINT_MIN=" << POINT_MIN[0] << "  " << POINT_MIN[1] << "  " << POINT_MIN[2] << endl;
				//cout << "POINT_MAX=" << POINT_MAX[0] << "  " << POINT_MAX[1] << "  " << POINT_MAX[2] << endl;
				dstImage.copyTo(org);
				for (int y = 0; y < srcImage.rows; y++){
					for (int x = 0; x < srcImage.cols; x++){
						for (int c = 0; c < 3; c++){
							//��ͼ��ֻ�Ը���Ȥ�����������
							org.at<Vec3b>(y, x)[c] = 255 * (srcImage.at<Vec3b>(y, x)[c] - POINT_MIN[c]) / (POINT_MAX[c] - POINT_MIN[c]);
							if (org.at<Vec3b>(y, x)[c]>255){
								org.at<Vec3b>(y, x)[c] = 255;
							}
							else if (org.at<Vec3b>(y, x)[c] < 0){
								org.at<Vec3b>(y, x)[c] = 0;
							}
						}
					}
				}

				//�����������ص㣬�ı����ȺͶԱȶ�
				for (int y = 0; y < org.rows; y++)
				{
					for (int x = 0; x < org.cols; x++)
					{
						for (int c = 0; c < 3; c++)
						{
							ThresholdImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(org.at<Vec3b>(y, x)[c]) + beta);
						}
					}
				}

				//cvtColor(ThresholdImage, grayImage, COLOR_BGR2GRAY);   //ת���Ҷ�ͼ��
				GaussianBlur(ThresholdImage, grayImage, Size(3, 3), 1);  //��˹�˲��� ȥ����ƽ������
				filter2D(grayImage, org, grayImage.depth(), kernel);//�񻯣���߱�Եϸ��

				De_extraction(org, dstImage, rect, point);

				//�任��PHTO\\mutual_info\\A008�ַ�-18.bmp
				strcpy_s(openpath, path);
				openpath[strlen(openpath) - 1] = '\0';
				strcat_s(openpath, "mutual_info\\");
				strcat_s(openpath, findData.name);

				imwrite(openpath, dstImage);
			}
			else{
				cout << openpath << endl;
				Mat IMG = imread(openpath);//����ͼ���ַ

				IMG.copyTo(org);
				Mat ThresholdImage = Mat::zeros(org.size(), org.type());
				//�����������ص㣬�ı����ȺͶԱȶ�
				for (int y = 0; y < org.rows; y++){
					for (int x = 0; x < org.cols; x++){
						for (int c = 0; c < 3; c++){
							ThresholdImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(org.at<Vec3b>(y, x)[c]) + beta);
						}
					}
				}
				Mat grayImage;
				//cvtColor(ThresholdImage, grayImage, COLOR_BGR2GRAY); //ת���Ҷ�ͼ��
				GaussianBlur(ThresholdImage, grayImage, Size(3, 3), 1);  //��˹�˲��� ȥ����ƽ������
				Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);//�����񻯺�
				Mat dstIMG;
				filter2D(grayImage, dstIMG, grayImage.depth(), kernel);//�񻯣���߱�Եϸ��
				//cout << image_roi.rows << "   " << image_roi.cols << endl;

				dstIMG.copyTo(org);
				for (int i = 0; i < 9; i++){
					line(org, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
				}
				line(org, point[9], point[0], Scalar(0, 255, 0), 1, 8, 0);

				//�任��IMG\\Hough\\A009����-*.bmp
				strcpy_s(openpath, path);
				openpath[strlen(openpath) - 1] = '\0';
				strcat_s(openpath, "mutual_info\\");
				strcat_s(openpath, findData.name);
				imwrite(openpath, org);
				imshow("org", org);
				cvWaitKey(0);
				cout << "��ʼ��׼" << endl;
				

				double *MI = new double[(dstIMG.rows - image_roi.rows + 1)*(dstIMG.cols - image_roi.cols + 1)];
				for (int j = 0; j <= (dstIMG.rows - image_roi.rows); j++){
					for (int i = 0; i <= (dstIMG.cols - image_roi.cols); i++){
						rect = Rect(i, j, image_roi.cols, image_roi.rows);
						Mat ROI = dstIMG(rect);
						MI[j*(dstIMG.cols - image_roi.cols) + i] = ComEntropy(image_roi, ROI, image_roi_entropy);
					}
				}

				//�ҳ�����Ϣ�������꣬˵������ƥ��
				double MAX = MI[0];
				Point PMAX = (0, 0);
				for (int j = 0; j <= (dstIMG.rows - image_roi.rows); j++){
					for (int i = 0; i <= (dstIMG.cols - image_roi.cols); i++){
						if (MAX < MI[j*(dstIMG.cols - image_roi.cols) + i]){
							MAX = MI[j*(dstIMG.cols - image_roi.cols) + i];
							PMAX.x = i;
							PMAX.y = j;
						}
					}
				}

				Point dis = PMAX - left_up;

				Point tmp_point[10];
				for (int i = 0; i < 10; i++){
					tmp_point[i] = point[i] + dis;
				}
				Rect tmp_rect = Rect(left_up.x + dis.x, left_up.y + dis.y, right_down.x - left_up.x, right_down.y - left_up.y);
				Mat tmp_image_roi;
				//��ԭͼ�Ͻ��и���Ȥ����
				IMG(tmp_rect).copyTo(tmp_image_roi);
				////�ҳ�ROI����󣬶�ROI���򣨾��Σ����жԱȶ���ǿ��ͨ���Ҷ�������ʵ��
				int POINT_MAX[3] = { tmp_image_roi.at<Vec3b>(0, 0)[0], tmp_image_roi.at<Vec3b>(0, 0)[1], tmp_image_roi.at<Vec3b>(0, 0)[2] };
				int POINT_MIN[3] = { tmp_image_roi.at<Vec3b>(0, 0)[0], tmp_image_roi.at<Vec3b>(0, 0)[1], tmp_image_roi.at<Vec3b>(0, 0)[2] };
				for (int y = 0; y < tmp_image_roi.rows; y++){
					for (int x = 0; x < tmp_image_roi.cols; x++){
						for (int c = 0; c < 3; c++){
							if (POINT_MAX[c] < tmp_image_roi.at<Vec3b>(y, x)[c]){
								POINT_MAX[c] = tmp_image_roi.at<Vec3b>(y, x)[c];
							}
							if (POINT_MIN[c]>tmp_image_roi.at<Vec3b>(y, x)[c]){
								POINT_MIN[c] = tmp_image_roi.at<Vec3b>(y, x)[c];
							}
						}
					}
				}
				//cout << "POINT_MIN=" << POINT_MIN[0] << "  " << POINT_MIN[1] << "  " << POINT_MIN[2] << endl;
				//cout << "POINT_MAX=" << POINT_MAX[0] << "  " << POINT_MAX[1] << "  " << POINT_MAX[2] << endl;

				dstIMG.copyTo(org);
				for (int y = 0; y < IMG.rows; y++){
					for (int x = 0; x < IMG.cols; x++){
						for (int c = 0; c < 3; c++){
							//��ͼ��ֻ�Ը���Ȥ�����������
							org.at<Vec3b>(y, x)[c] = 255 * (IMG.at<Vec3b>(y, x)[c] - POINT_MIN[c]) / (POINT_MAX[c] - POINT_MIN[c]);
							if (org.at<Vec3b>(y, x)[c]>255){
								org.at<Vec3b>(y, x)[c] = 255;
							}
							else if (org.at<Vec3b>(y, x)[c] < 0){
								org.at<Vec3b>(y, x)[c] = 0;
							}
						}
					}
				}

				//�����������ص㣬�ı����ȺͶԱȶ�
				for (int y = 0; y < org.rows; y++)
				{
					for (int x = 0; x < org.cols; x++)
					{
						for (int c = 0; c < 3; c++)
						{
							ThresholdImage.at<Vec3b>(y, x)[c] = saturate_cast<uchar>(alpha*(org.at<Vec3b>(y, x)[c]) + beta);
						}
					}
				}
				//cvtColor(ThresholdImage, grayImage, COLOR_BGR2GRAY);   //ת���Ҷ�ͼ��
				GaussianBlur(ThresholdImage, grayImage, Size(3, 3), 1);  //��˹�˲��� ȥ����ƽ������

				//�񻯣���߱�Եϸ��
				filter2D(grayImage, org, grayImage.depth(), kernel);

				De_extraction(org, dstIMG, tmp_rect, tmp_point);

				//�任��PHTO\\mutual_info\\A009����-*.bmp
				strcpy_s(openpath, path);
				openpath[strlen(openpath) - 1] = '\0';
				strcat_s(openpath, "mutual_info\\");
				strcat_s(openpath, findData.name);
				openpath[strlen(openpath) - 4] = '\0';
				strcat_s(openpath, "_��׼ͼ.bmp");
				imwrite(openpath, dstIMG);
				cout << "��׼��ɣ���ʼ��һ��" << endl;

			}
		}
	} while (_findnext(handle, &findData) == 0);    // ����Ŀ¼�е���һ���ļ�

	_findclose(handle);    // �ر��������

	system("pause");
	return 0;
}
