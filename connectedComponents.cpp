#include <iostream>
//#include <opencv2\opencv.hpp>
#include <fstream>
#include <string>
#include "HogDetection.h"

using namespace cv;
using namespace std;

//Just some convienience macros
#define CV_CVX_WHITE    CV_RGB(0xff,0xff,0xff)
#define CV_CVX_BLACK    CV_RGB(0x00,0x00,0x00)


CvSeq* ConnectedComponents(Mat &mask_process, int poly1_hull0, float perimScale, int number,
	Rect &bounding_box, Point &contour_centers)
{
	IplImage *mask = &IplImage(mask_process);
	int *num = &number;
	CvRect *bbs = &CvRect(bounding_box);
	CvPoint *centers = &CvPoint(contour_centers);
	static CvMemStorage*    mem_storage    = NULL;
	static CvSeq*            contours    = NULL;
	//CLEAN UP RAW MASK
	//���������ã�ƽ��������ȥ��ϸ��,�Ͽ�ȱ��
	cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_OPEN, 1 );//������mask���п�������CVCLOSE_ITRΪ�������Ĵ��������Ϊmaskͼ��
	
	//cvNamedWindow("openmask");
	//cvShowImage("openmask",mask);


	//���������ã�ƽ������������ȱ��
	cvMorphologyEx( mask, mask, NULL, NULL, CV_MOP_CLOSE, 1 );//������mask���бղ�����CVCLOSE_ITRΪ�ղ����Ĵ��������Ϊmaskͼ��
	
	//cvNamedWindow("closemask");
	//cvShowImage("closemask",mask);
	//cvWaitKey(0);
	//cvDestroyWindow("openmask");
	//cvDestroyWindow("closemask");

	//FIND CONTOURS AROUND ONLY BIGGER REGIONS
	if( mem_storage==NULL ) mem_storage = cvCreateMemStorage(0);
	else cvClearMemStorage(mem_storage);
	//CV_RETR_EXTERNAL=0����types_c.h�ж���ģ�CV_CHAIN_APPROX_SIMPLE=2Ҳ���ڸ��ļ��ж����
	CvContourScanner scanner = cvStartFindContours(mask,mem_storage,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
	CvSeq* c;
	int numCont = 0;
	//��while�ڲ�ֻ��ԱȽϴ���������߽����滻����
	while( (c = cvFindNextContour( scanner )) != NULL )
	{
		//double len = cvContourPerimeter( c );
		double len = cvContourArea(c);
		CvRect roi_temp = cvBoundingRect(c);
		double q = ((roi_temp.height+90) * (roi_temp.width+90)) / perimScale;
		//double q = (roi_temp.height + roi_temp.width) /perimScale;   //calculate perimeter len threshold
		if( len < q ) //Get rid of blob if it's perimeter is too small
		{
			cvSubstituteContour( scanner, NULL );    //��NULL����ԭ�����Ǹ�����
		}
		else //Smooth it's edges if it's large enough
		{
			CvSeq* c_new;
			if(poly1_hull0) //Polygonal approximation of the segmentation
				c_new = cvApproxPoly(c,sizeof(CvContour),mem_storage,CV_POLY_APPROX_DP, 2,0);
			else //Convex Hull of the segmentation
				c_new = cvConvexHull2(c,mem_storage,CV_CLOCKWISE,1);
			cvSubstituteContour( scanner, c_new ); //�ʼ��������͹�����߶���ʽ��������滻
			numCont++;
		}
	}
	contours = cvEndFindContours( &scanner );    //�����������Ҳ���
	// PAINT THE FOUND REGIONS BACK INTO THE IMAGE
	cvZero( mask );
	IplImage *maskTemp;
	//CALC CENTER OF MASS AND OR BOUNDING RECTANGLES
	if(*num != 0)
	{
		int N = *num, numFilled = 0, i=0;
		CvMoments moments;
		double M00, M01, M10;
		maskTemp = cvCloneImage(mask);
		for(i=0, c=contours; c != NULL; c = c->h_next,i++ )        //h_nextΪ���������е���һ������
		{
			if(i < N) //Only process up to *num of them
			{
				//CV_CVX_WHITE�ڱ��������ǰ�ɫ����˼
				cvDrawContours(maskTemp,c,CV_CVX_WHITE, CV_CVX_WHITE,-1,CV_FILLED,8);
				//Find the center of each contour
				if(centers != &cvPoint(-1, -1))
				{
					cvMoments(maskTemp,&moments,1);    //����maskͼ�����ߴ�3�׵ľ�
					M00 = cvGetSpatialMoment(&moments,0,0); //��ȡx��0�κ�y��0�ξ�
					M10 = cvGetSpatialMoment(&moments,1,0); //��ȡx��1�κ�y��0�ξ�
					M01 = cvGetSpatialMoment(&moments,0,1); //��ȡx��0�κ�y��1�ξ�
					centers[i].x = (int)(M10/M00);    //���þصĽ��������������ĵ�����
					centers[i].y = (int)(M01/M00);
				}
				//Bounding rectangles around blobs
				if(bbs != &CvRect())
				{
					bbs[i] = cvBoundingRect(c); //�������c����Ӿ���
				}
				cvZero(maskTemp);
				numFilled++;
			}
			//Draw filled contours into mask
			cvDrawContours(mask,c,CV_CVX_WHITE,CV_CVX_WHITE,-1,CV_FILLED,8); //draw to central mask
		} //end looping over contours
		*num = numFilled;
		cvReleaseImage( &maskTemp);
	}
	//ELSE JUST DRAW PROCESSED CONTOURS INTO THE MASK
	else
	{
		for( c=contours; c != NULL; c = c->h_next )
		{
			cvDrawContours(mask,c,CV_CVX_WHITE, CV_CVX_BLACK,-1,CV_FILLED,8);
		}
	}
	return contours;
}