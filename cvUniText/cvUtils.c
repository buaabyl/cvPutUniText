#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

void cvxFill(IplImage* img, CvScalar color)
{
    CvPoint pt1;
    CvPoint pt2;

    pt1.x = 0;
    pt1.y = 0;
    pt2.x = img->width;
    pt2.y = img->height;

    cvRectangle(img, pt1, pt2, color, -1, CV_AA, 0/*shift*/);
}

