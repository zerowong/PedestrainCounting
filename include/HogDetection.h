#ifndef HOGDETECTION_H
#define HOGDETECTION_H

/*HogDetection�㷨ʵ��
*@File:HogDetection.cpp
*@Author:Kan Chen (chenkan0007@gmail.com),
*@Last Modified:2013-06-16
*/

#define _CRT_NONSTDC_NO_WARNINGS

#include <math.h>
#include <tchar.h>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <set>
#include <vector>
#include "kmeans.h"
#include "meanshift.h"

#include "Classifier.h"
using namespace std;

#include "opencv\cv.h"
#include "opencv\highgui.h"

const double pi = 3.1416;
const int TOTAL_STAGE = 20; // number of classifier stages
const int HEIGHT = 64; // model height
const int WIDTH = 32; // model width
const int BKG_DETECTION_NUM_CLUSTER = 3;		// ������ȡʱkmeans���ಽ�����趨�ľ�����Ŀ
const int BKG_VIDEO_PIC_NUM_ALL = 50;				// ������ȡʱѡȡ�Ĵ���֡��Ŀ
const double THRES_DIFF_BINARY = 0.1 * 255;		// ��������ʱ��ȡ��ͨ���趨����ֵ
const int EDGE_WIDTH = 60;									// ��ͨ�����Χ��������ͨ������ı߳�
const int GAUSSIAN_SMOOTH_R = 3;					// ���и�˹ģ��ʱ�İ뾶
const float THRES_PERIMETER_RATIO = 20.0;		// ��ͨ�����ʱ���õ��ܳ�����Χ�����ܳ��ȵ���ֵ
const double THRES_AREA_ROI_MAX = 0.005;		// ��һ��ɸѡʱ�������������ֵ
const double THRES_AREA_ROI_MIN = 0.001;		// ��һ��ɸѡʱ�������������ֵ
const double THRES_DIFF_FRAME_BINARY = 0.03 * 255;	// ֡���ȡ��ͨ��ʱ�趨����ֵ
const double THRES_JUD_RATIO = 0.75;				// ֡��ж���������ͨ������Χ���ε��������ֵ


struct rect
{
	int x1;
	int y1;
	int x2;
	int y2;
	int re;
	int count;
};

struct drawbox
{
	CvPoint point1;
	CvPoint point2;
	IplImage* image;
	IplImage* temp;
	bool isDraw;
};

// ��һ��ɸѡʱ�б��Ƿ�Ϊ����
bool judge_pedestrian(
	IplImage* frame, IplImage* frame_pre,
	CvRect roi, double thres_max, double thres_min,
	double thres_binary, double thres_ratio);

// �˹��궨�������ʱ�������Ӧ����
void cvMouseCallback(int mouseEvent, int x, int y, int flags, void* param);

// ʹ��HOG�������������˼��
void Detection(
	IplImage *img, IplImage *gray,
	double smin, double smax,
	double scalestep, int slidestep, int neighbor);

// ��ͨ���������
CvSeq* ConnectedComponents(cv::Mat &mask_process, int poly1_hull0, float perimScale, int number = 0,
	cv::Rect &bounding_box = cv::Rect(), cv::Point &contour_centers = cv::Point(-1, -1));


// ʹ��HOG���������������������˼��(����ͼ��)
void combo_DetectPicture(
	const TCHAR *pIn, const TCHAR *pOut,
	double smin, double smax,
	double scalestep, int slidestep, int neighbor, IplImage* background);

// ʹ��HOG������������������֡������˼�⣬�����Ѿ�������˵�ͼ��(����ͼ��)
IplImage* combo_DetectPicture(
	IplImage* frame, IplImage* frame_pre, CvRect roi,
	double smin, double smax,
	double scalestep, int slidestep, int neighbor, IplImage* background);

// ʹ��HOG���������������������˼�⣬�����Ѿ�������˵�ͼ��(����ͼ��)
IplImage* combo_DetectPicture(
	IplImage* frame,
	double smin, double smax,
	double scalestep, int slidestep, int neighbor, IplImage* background);

// ʹ��HOG��������֡������˼�⣬�����Ѿ�������˵�ͼ��(����ͼ��)
IplImage* combo_DetectPicture_diff(
	IplImage* frame, IplImage* frame_pre,
	double smin, double smax, double thres_binary_frame,
	double scalestep, int slidestep, int neighbor);

// ʹ�ñ���������֡�����Ƶ���˼�⣬�����м��˹��궨�����ʱ��
double combo_DetectVideo(
	const TCHAR *pIn, const TCHAR *pOut, bool isdraw, bool isdiff,
	double smin, double smax, double scale,
	double scalestep, int slidestep, int neighbor, IplImage* bkg1);

// ����ʹ��HOG���������е����˼��(��Ƶ)
void DetectVideo(
	const TCHAR *pIn, const TCHAR *pOut, const TCHAR *pPolygon,
	double smin, double smax, double scale,
	double scalestep, int slidestep, int neighbor);

// ����ʹ��HOG���������е����˼��(����ͼ��)
void DetectPicture(
	const TCHAR *pIn, const TCHAR *pOut, const TCHAR *pPolygon,
	double smin, double smax,
	double scalestep, int slidestep, int neighbor);

// ��ͼ������ת��ΪKmeans�㷨ִ��ʱ�����ݽṹ
void readin_tuples(
	TCHAR pIn[], Tuple* &tuples, int pic_num_all, CvRect roi,
	int frameH, int frameW);

// ������ȡʱ�Դ���֡����meanshift����õ���ͼƬ������pOut���ڵ��ļ�����
void create_meanshift_queue(const TCHAR *pIn,
	const TCHAR* pOut, int r, int pic_num_all);

// ������ȡ������������ȡ�ı���ͼ��
IplImage* video_bkg_detect(
	const TCHAR *pIn, const TCHAR* pTemp, int num_cluster, int r, int pic_num_all,
	int H_thres, int W_thres);

// ʹ��֡���HOG����������Ƶ���˼�⺯��
void combo_DetectVideo_framediff(
	const TCHAR *pIn, const TCHAR *pOut,
	double smin, double smax, double thres_binary_frame,
	double scalestep, int slidestep, int neighbor, IplImage* bkg1);

// ��ȡ֡��ͼ�����ͨ��
CvSeq* get_framediff_components(
	IplImage* &frame, IplImage* frame_pre,
	double thres_binary_frame);

#endif