#ifndef KMEANS_H
#define KMEANS_H
#pragma once

#include <iostream>  
#include <fstream>  
#include <vector>  
#include <math.h>  
#include "opencv\highgui.h"
#include "opencv\cv.h"

/*
#ifndef NDEBUG
#pragma comment(lib, "..\\lib\\opencv_core300d.lib")
#pragma comment(lib, "..\\lib\\opencv_highgui300d.lib")
#pragma comment(lib, "..\\lib\\opencv_imgproc300d.lib")
#else // NDEBUG
#pragma comment(lib, "..\\OpenCV\\lib\\opencv_core231.lib")
#pragma comment(lib, "..\\OpenCV\\lib\\opencv_highgui231.lib")
#pragma comment(lib, "..\\OpenCV\\lib\\opencv_imgproc231.lib")
#endif
*/

using namespace std;  

/*kmeans�㷨ʵ�֣�����Ԫ�������Ե������ 
*@File:kmeans.cpp 
*@Author:Kan Chen (chenkan0007@gmail.com)
*@Create:2011-12-10 
*@Last Modified:2013-05-07 
*/ 

//const int NUM_clusters = 3;
const double THRES_Diff = 5.0e3;
const int THRES_ITERATION_TIMES = 150;

//���Ԫ���������Ϣ  
struct Tuple{  
	double attr1;  
	double attr2;  
	int num_elem;
	double* data;
};  

struct row_data
{
	double row;
	int label;
};

// ����Kmeans������������Ԫ����Ŀ�Ӵ�С����ĺ���
bool lessmark(const row_data &s1, const row_data &s2);

// ����Kmeans������������Ԫ����Ŀ��С��������ĺ���
bool greatermark(const row_data &s1, const row_data &s2);

// ��ʾÿ�����ݵ���Ϣ�ĺ���(���ڵ���)
void print_Tuple(Tuple tuple);

// ��ʼ��ÿ�����ݵ�
void init_Tuple(Tuple &tuple, int num_element);

// ��ͼ������ת��Ϊ���ݵ�
void readin_data(Tuple &tuple, int num, CvMat* &Matdata);

// ��ȡ���ݵ����Ϣ
void readout_data(Tuple &tuple, int frameH, int frameW);

// ɾ�����ݵ�
void delete_tuple(Tuple &tuple);

//��������Ԫ����ŷ�������  
double getDistXY(Tuple t1, Tuple t2);

//�������ģ�������ǰԪ�������ĸ���  
int clusterOfTuple(Tuple means[],Tuple tuple, int num_cluster);

//��ø����ؼ���ƽ�����  
double getVar(Tuple* &tuples,Tuple means[], int num_cluster, vector<int>* labels);

//��õ�ǰ�صľ�ֵ�����ģ�  
void cal_mean(Tuple* &cluster, Tuple & mean_tuple, vector<int> labels); 

//kMeans �㷨ʵ��
vector<int>* KMeans(Tuple* &tuples, int num_cluster, double thres_diff, int length);

#endif