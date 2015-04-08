#ifndef HOGTRACKING_H
#define HOGTRACKING_H
#pragma once

/*HogTracking.h�㷨ʵ��
*@File:HogTracking.cpp 
*@Author:Kan Chen (chenkan0007@gmail.com), 
*@Last Modified:2013-06-16 
*/ 

#include "HogDetection.h"

using namespace std;
//using namespace cv;

const double MERGEDISTANCE=32;
const int FRAMENUMBER=3;//���ٵ��ۻ���⵽��֡��
const int TIMEFRAME=5;//�ۻ�����֡��
const int CAPACITY=30;//һ֡������ļ�⵽��Ŀ��ĸ���

struct part
{
	int cen_h;
	int cen_w;
	double width;
	double height;

	double predict_h;
	double predict_w;
	double predict_hv;
	double predict_wv;

	int state;
	//state=0: �õ�û���õ�
	//state=1: �ռ��ĵ㣬�ȴ��ж�(Ԥ��̬)
	//state=2: ��ʾ�ĵ㣬�����ܵ���ע�ĵ�
	//state=3: ��ʾ�ĵ㣬������������·

	int state_count;
	int seriesnumber;
	int tracknumber;

	int flag;
	int flagcircle;
};

struct info
{
	part point[CAPACITY];
};

class model
{
public:
	CvFont font;
	double objectInfo[3];
	info dot[TIMEFRAME];
	info temdot;

	feature *classifier[TOTAL_STAGE];
	int numweak[TOTAL_STAGE];
	integral **source;
	integral **s;

	CvMat *polygon;
	CvRect roi;
	CvPoint crosslinept1;
	CvPoint crosslinept2;

	std::set<int> front;
	std::set<int> back;
};

struct output
{
	int front;
	int back;
	int num;
	CvPoint position[30];
};

// �����㷨�еļ�⺯��������rect_seq�ľ������У��������������˵ľ������򣬱�Detection_t����
void Detection(
	IplImage *img, IplImage *gray, CvRect roi_cur,
	feature *classifier[], int numweak[],
	integral **source, integral **s, CvSeq* &rect_seq,
	double smin, double smax,
	double scalestep, int slidestep, int neighbor);

// �������⺯������combo_Video_Tracking��������
output DetectionTracking(
	IplImage* frame, IplImage* frame_pre, CvRect roi0,
	feature *classifier[], int numweak[],
	double smin, double smax,
	double scalestep, int slidestep, int neighbor, IplImage* bkg1, model &md);

// ����ͼ������˼�⣬����pedestrians���У���Detection��������
void combo_DetectPicture_t(
	IplImage* frame, IplImage* frame_pre, 
	CvSeq* &pedestrians, CvRect roi0,
	feature *classifier[], int numweak[],
	double smin, double smax,
	double scalestep, int slidestep, int neighbor, IplImage* bkg1);

// ������˵ĺ�������DetectionTracking��������
info Detection_t(
	IplImage* frame, IplImage* frame_pre, CvRect roi0,
	feature *classifier[], int numweak[],
	double smin, double smax,
	double scalestep, int slidestep, int neighbor, IplImage* bkg1, model &md);

// ��Ƶ���˸��ٺ���
void combo_Video_Tracking(
	const char *pIn, const char *pOut, const char* pPolygon,
	bool isdiff, feature *classifier[], int numweak[],
	double smin, double smax, double scale,
	double scalestep, int slidestep, int neighbor, IplImage* bkg1);

// �ж�����״̬
void JudgeState(model &md);

// ���Ƹ��ٵ�����
void DrawTarget(IplImage *img, model &md, output &result);

// ����Ԥ����Ϣ
void UpdatePrediction(info dot[]);

#endif