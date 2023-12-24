#include "De_Extraction.h"
#include "RemoveSmallRegion.h"
using namespace std;
using namespace cv;

void De_imageblur(Mat& src, Mat& dst, Size size, int threshold){
	int height = src.rows;
	int width = src.cols;
	blur(src, dst, size);
	for (int i = 0; i < height; i++)
	{
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			if (p[j] < threshold)
				p[j] = 0;
			else p[j] = 255;
		}
	}
}



//MsrcIMG�Ǿ����Ҷ�����ĸ���Ȥ����srcImage�Ǿ�������񻯺��ͼ��
void De_extraction(cv::Mat& srcImage, cv::Mat& dstImage, Rect rect, Point *point){
	//��ROI(����)ͼ�������ֵ�˲����˴������Ƚ��лҶȱ任���Ӷ��Ż����
	Mat edges;
	Canny(srcImage, edges, 32, 200, 3);
	//imshow("��Ե����", edges);
	Mat thre_IMG1;
	edges.copyTo(thre_IMG1);
	//�ҳ�ȱ�ݵ����λ�ã���ʱĬ������1/3���м�1/2
	int width = rect.width;
	int height = rect.height;
	//СROI��������Ͻ��Լ����½�
	Point small_left_up = Point(rect.x, rect.y );
	Point small_right_down = Point(int(rect.x + width / 3), rect.y + height);
	for (int y = 0; y < thre_IMG1.rows; y++){
		for (int x = 0; x < thre_IMG1.cols; x++){
			//�����������ı�Ե
			if (x>=small_left_up.x&&x<=small_right_down.x&&y>=small_left_up.y&&y<=small_right_down.y){
			}
			else{
				thre_IMG1.at<uchar>(y, x) = 0;
			}
		}
	}
	//imshow("������������ı�Ե", thre_IMG1);
	Mat ele = getStructuringElement(MORPH_CROSS, Size(3, 3));
	dilate(thre_IMG1, thre_IMG1, ele);//����
	dilate(thre_IMG1, thre_IMG1, ele);//����
	//imshow("��������ͺ��ͼ��", thre_IMG1);

	Mat fill_edges;
	thre_IMG1.copyTo(fill_edges);
	Size m_Size = thre_IMG1.size();
	Mat temimage = Mat::zeros(m_Size.height + 2, m_Size.width + 2, thre_IMG1.type());//��չͼ��
	thre_IMG1.copyTo(temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)));
	floodFill(temimage, Point(0, 0), Scalar(255));
	Mat cutImg;//�ü���չ��ͼ��
	temimage(Range(1, m_Size.height + 1), Range(1, m_Size.width + 1)).copyTo(cutImg);
	fill_edges = thre_IMG1 | (~cutImg);
	//imshow("���ͼ��", fill_edges);

	//��θ�ʴ�����Ĵ����ͣ��õ����յ�Ȧ
	erode(fill_edges, fill_edges, ele);//��ʴ1
	erode(fill_edges, fill_edges, ele);//��ʴ2
	erode(fill_edges, fill_edges, ele);//��ʴ3
	erode(fill_edges, fill_edges, ele);//��ʴ4
	erode(fill_edges, fill_edges, ele);//��ʴ5
	erode(fill_edges, fill_edges, ele);//��ʴ6
	erode(fill_edges, fill_edges, ele);//��ʴ7
	erode(fill_edges, fill_edges, ele);//��ʴ8
	erode(fill_edges, fill_edges, ele);//��ʴ9
	erode(fill_edges, fill_edges, ele);//��ʴ10
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	dilate(fill_edges, fill_edges, ele);//����
	//imshow("������ͼ��", fill_edges);
	De_imageblur(fill_edges, fill_edges, Size(3, 3), 128);
	//imshow("��Ե�⻬���ͼ��", fill_edges);
	Canny(fill_edges, thre_IMG1, 15, 40, 3);
	//imshow("��Եͼ��", thre_IMG1);
	
	for (int i = 0; i < 9; i++){
		line(dstImage, point[i], point[i + 1], Scalar(0, 255, 0), 1, 8, 0);
	}
	line(dstImage, point[9], point[0], Scalar(0, 255, 0), 1, 8, 0);


	//����ɫȱ�ݼ����,�������Ҫ��Բ����
	for (int y = 0; y < edges.rows; y++){
		for (int x = 0; x < edges.cols; x++){
			//��������Ǳ�Ե��
			if (thre_IMG1.at<uchar>(y, x) == 255){
				dstImage.at<Vec3b>(y, x)[0] = 0;
				dstImage.at<Vec3b>(y, x)[1] = 0;
				dstImage.at<Vec3b>(y, x)[2] = 255;
			}
		}
	}
	imshow("Ŀ��ͼ", dstImage);

	waitKey(0);
}