/*  
 *  Copyright 2013 buaa.byl@gmail.com
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <cvUtils.h>
#include <cvUniText.h>

int main(int argc, char* argv[])
{
    IplImage* img;
    CvUniLibrary* ft_lib;
    CvUniFont* ft_face;
    CvRect sz;
    int x = 50;
    int y = 20;
    char s[] = "这是一只小猫：cat.jpg";

    ft_lib = cvInitUniLibrary();
    ft_face = cvInitUniFont(ft_lib, "C://WINDOWS//Fonts//simsun.ttc", 32);


    //img = cvCreateImage(cvSize(400, 300), IPL_DEPTH_8U, 3);
    //cvxFill(img, CV_RGB(255,255,255));

    img = cvLoadImage("cat.jpg", CV_LOAD_IMAGE_COLOR);
    //img = cvLoadImage("cat.jpg", CV_LOAD_IMAGE_GRAYSCALE);

    y = img->height - 10;

    
    sz = cvPutUniText(img, s, cvPoint(x,y), ft_face, CV_RGB(0,0x3,0x8F), "UTF-8", 1);

    //top
    cvLine(img,   cvPoint(sz.x, sz.y-sz.height),    cvPoint(sz.x+sz.width, sz.y-sz.height),
            CV_RGB(0,255,0),  1,  0, 0);
    //baseline
    cvLine(img,   cvPoint(x, y),                    cvPoint(x+sz.width, y),
            CV_RGB(255,0,0),  1,  0, 0);
    //bottom
    cvLine(img,   cvPoint(sz.x, sz.y),              cvPoint(sz.x+sz.width, sz.y),
            CV_RGB(0,255,255),  1,  0, 0);

    //red-cross
    cvCircle(img, cvPoint(x, y),    3,                CV_RGB(255,0,0), -1,  0, 0);
    cvLine(img,   cvPoint(x-10, y), cvPoint(x+10, y), CV_RGB(255,0,0),  1,  0, 0);
    cvLine(img,   cvPoint(x, y+10), cvPoint(x, y-10), CV_RGB(255,0,0),  1,  0, 0);


    cvPutUniText(img, s, cvPoint(x,y), ft_face, CV_RGB(0,0x3,0x8F), "UTF-8", 0);


    cvShowImage("tt", img);
    cvSaveImage("cat-font.jpg", img, 0);
    cvWaitKey(-1);


    cvReleaseImage(&img);
    cvCleanUniFont(ft_face);
    cvCleanUniLibrary(ft_lib);

    return 0;
}

