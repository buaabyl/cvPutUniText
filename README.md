cvPutUniText
============
This function depend:
  * libiconv: convert encoding
  * freetype: render bitmap from char

Interface:
```c
CvUniLibrary* cvInitUniLibrary();
void cvCleanUniLibrary(CvUniLibrary* font_lib);

CvUniFont* cvInitUniFont(CvUniLibrary* font_lib, const char* font_face, int font_size);
void cvCleanUniFont(CvUniFont* font);

CvRect cvPutUniText(IplImage* img, const char* text,
        CvPoint org, CvUniFont* font, CvScalar color,
        const char* encoding, int calc);
```

Example:
```c
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

    img = cvLoadImage("cat.jpg", CV_LOAD_IMAGE_COLOR);
    y = img->height - 10;
    sz = cvPutUniText(img, s, cvPoint(x,y), ft_face, CV_RGB(0,0x3,0x8F), "UTF-8", 1);
    cvPutUniText(img, s, cvPoint(x,y), ft_face, CV_RGB(0,0x3,0x8F), "UTF-8", 0);
    cvSaveImage("cat-font.jpg", img, 0);
    cvWaitKey(-1);

    cvReleaseImage(&img);
    cvCleanUniFont(ft_face);
    cvCleanUniLibrary(ft_lib);

    return 0;
}
````

Original image:

![image](https://github.com/buaabyl/cvPutUniText/raw/master/cvUniText/cat.jpg)

Put chinese font to image:

![image](https://github.com/buaabyl/cvPutUniText/raw/master/cvUniText/cat-font.jpg)


  * red cross: font location
  * green line:font top
  * red line:baseline
  * blue line:font bottom
  
