#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>
#include <dirent.h>
#include <sys/types.h>
#include <vector>
#include <string.h>
#include <math.h>
using namespace cv;
using namespace std;

IplImage* getIntensity(IplImage* img);
IplImage* applyFilter(IplImage* img, int para);
IplImage* getEigenValue(IplImage* ix, IplImage* iy, int wsize, int para);
IplImage* filterR(IplImage* img, int wsize);
IplImage* mixP(IplImage* img, IplImage* origin, int psize);
double* img2double(IplImage* img);
IplImage* double2img(double* d, IplImage* i);

int main (int argc, char *argv[]) {
    IplImage* image;
    IplImage* intensity;
    IplImage* ix;
    IplImage* iy;
    IplImage* le;
    IplImage* se;
    IplImage* r;
    IplImage* fr;
    IplImage* mix;
    
    cvNamedWindow("origin", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("intensity", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("ix", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("iy", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("l_eigenvalue", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("s_eigenvalue", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("r", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("fr", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("mix", CV_WINDOW_AUTOSIZE);

    //check the input parameters
    if (argc != 4) {
    	cout<<"parameters invalid"<<endl;
	return 0;
    }

    //get all the input parameters
    char* input_src = argv[1];
    float k = atof(argv[2]);
    int wsize = atoi(argv[3]);

    //read the image
    image  = cvLoadImage(input_src, 1);
    cvShowImage("origin", image);

    //compute the intensity of the image
    intensity = getIntensity(image);
    cvShowImage("intensity", intensity);
	cvSaveImage("./Resource/intensity.jpg", intensity);

    //compute the ix and iy of the image
    ix = applyFilter(intensity, 1);
    cvShowImage("ix", ix);
	cvSaveImage("./Resource/ix.jpg", ix);

    iy = applyFilter(intensity, 2);
    cvShowImage("iy", iy);
	cvSaveImage("./Resource/iy.jpg", iy);

    //compute the eigenvalue
    le = getEigenValue(ix, iy, wsize, 1);
    cvShowImage("l_eigenvalue", le);
	cvSaveImage("./Resource/le.jpg", le);
    se = getEigenValue(ix, iy, wsize, 2);
    cvShowImage("s_eigenvalue", se);
	cvSaveImage("./Resource/se.jpg", se);

    //compute the R value
    r = getEigenValue(ix, iy, wsize, 3);
    cvShowImage("r", r);
	cvSaveImage("./Resource/r.jpg", r);

    //filter the R value
    fr = filterR(r, 20);
    cvShowImage("fr", fr);
	cvSaveImage("./Resource/fr.jpg", fr);

    //mix the point with origin picture
    mix = mixP(fr, image, 2);
    cvShowImage("mix", mix);
	cvSaveImage("./Resource/mix.jpg", mix);

    cvWaitKey(0);

    return 0;
}

double* img2double(IplImage* img) {
	CvMat* im = cvCreateMat(img->height, img->width, CV_64FC1);
	cvConvert(img, im);
	double* d = im->data.db;
	return d;
}

IplImage* double2img(double* b, IplImage* i) {
	CvMat im = cvMat(i->height, i->width, CV_64FC1, b);
	IplImage* img = cvCreateImage(cvGetSize(i), i->depth, 1);
	cvConvert(&im, img);
	return img;
}

IplImage* getIntensity(IplImage* img) {
    IplImage* r = cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage* g = cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage* b = cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage* i = cvCreateImage(cvGetSize(img), img->depth, 1);
    
    CvMat* rmat = cvCreateMat(img->height, img->width, CV_64FC1);
    CvMat* gmat = cvCreateMat(img->height, img->width, CV_64FC1);
    CvMat* bmat = cvCreateMat(img->height, img->width, CV_64FC1);
    CvMat* imat = cvCreateMat(img->height, img->width, CV_64FC1);

    cvSplit(img, b, g, r, NULL);
    cvConvert(b, bmat);
    cvConvert(g, gmat);
    cvConvert(r, rmat);

    cvAddWeighted(bmat, 1.0 / 3.0, rmat, 1.0 / 3.0, 0.0, imat);
    cvAddWeighted(imat, 1.0 / 1.0, gmat, 1.0 / 3.0, 0.0, imat);
    cvConvert(imat, i);

    return i;
}

IplImage* applyFilter(IplImage* img, int para) {
    IplImage* result = cvCreateImage(cvGetSize(img), img->depth, 1);
    
    double prewitt_x[] = {-1,  0,  1, -1, 0, 1, -1, 0, 1};
    double prewitt_y[] = {-1, -1, -1,  0, 0, 0,  1, 1, 1};
    double sobel_x[]   = {-1,  0,  1, -2, 0, 2, -1, 0, 1};
    double sobel_y[]   = {-1, -2, -1,  0, 0, 0,  1, 2, 1};

    CvMat px = cvMat(3, 3, CV_64FC1, prewitt_x);
    CvMat py = cvMat(3, 3, CV_64FC1, prewitt_y);
    CvMat sx = cvMat(3, 3, CV_64FC1, sobel_x);
    CvMat sy = cvMat(3, 3, CV_64FC1, sobel_y);

    switch(para) {
        case 1: cvFilter2D(img, result, &px, cvPoint(-1, -1)); break;
        case 2: cvFilter2D(img, result, &py, cvPoint(-1, -1)); break;
	case 3: cvFilter2D(img, result, &sx, cvPoint(-1, -1)); break;
	case 4: cvFilter2D(img, result, &sy, cvPoint(-1, -1)); break;
    }

    return result;
}

IplImage* getEigenValue(IplImage* ix, IplImage* iy, int wsize, int para) {
    double* x = img2double(ix);
    double* y = img2double(iy);
    
    double* l = (double*)malloc(ix->height * ix->width * sizeof(double));
    double* s = (double*)malloc(ix->height * ix->width * sizeof(double));
    double* r = (double*)malloc(ix->height * ix->width * sizeof(double));
    
    for(int i = wsize / 2; i < (ix->height - wsize / 2); i ++)
    for(int j = wsize / 2; j < (ix->width  - wsize / 2); j ++) {
	//compute A B C, A = Ix * Ix, B = Iy * Iy, C = Ix * Iy
 	double A = 0;
	double B = 0;
	double C = 0;
        for(int ii = i - wsize / 2; ii <= (i + wsize / 2); ii ++)
	for(int jj = j - wsize / 2; jj <= (j + wsize / 2); jj ++) {
    	    double xx = x[ii * ix->width + jj];
	    double yy = y[ii * ix->width + jj];
	    A += xx * xx;
	    B += yy * yy;
	    C += xx * yy;
	}
	double p = A + B;
	double q = A * B - C * C;
	double delta = p * p - 4 * q;
	if(delta >= 0) {
	    l[i * ix->width + j] = (p + sqrt(delta)) / 2;
	    s[i * ix->width + j] = (p - sqrt(delta)) / 2;
	    double rr = q * q - 0.04 * p * p;
	    double threhold = 100000000000008;
	    if(rr > threhold) {
		r[i * ix->width + j] = 255;
	    }
	}
    }

    IplImage* le = double2img(l, ix);
    IplImage* se = double2img(s, ix);
    IplImage* re = double2img(r, ix);
    
    switch(para) {
	case 1: return le; break;
        case 2: return se; break;
        case 3: return re; break;
    }
}

IplImage* filterR(IplImage* img, int wsize) {
    double* result = (double*)malloc(img->height * img->width * sizeof(double));    
    double* r = img2double(img);
    
    //find local maxima of R
    for(int i = wsize / 2; i < (img->height - wsize / 2); i += wsize)
    for(int j = wsize / 2; j < (img->width  - wsize / 2); j += wsize) {
	double origin = r[i * img->width + j];
	for(int ii = i - wsize/2; ii <= (i + wsize / 2); ii ++)
        for(int jj = j - wsize/2; jj <= (j + wsize / 2); jj ++)
	    if(origin < r[ii * img->width + j]) origin = 0;	    
	if(origin == 0) result[i * img->width + j ] = 0;
	else result[i * img->width + j] = 255;
    }

    IplImage* fr = double2img(result, img);
    return fr;
}

IplImage* mixP(IplImage* point, IplImage* img, int psize) {
    IplImage* r = cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage* g = cvCreateImage(cvGetSize(img), img->depth, 1);
    IplImage* b = cvCreateImage(cvGetSize(img), img->depth, 1);
    
    cvSplit(img, b, g, r ,NULL);

    double* rarray = img2double(r);
    double* garray = img2double(g);
    double* barray = img2double(b);
    
    double* parray = img2double(point);
   
    for(int i = psize; i < img->height - psize; i ++)
    for(int j = psize; j < img->width  - psize; j ++) {	    
        if(parray[i * img->width + j] != 0) {
	    for(int ii = i - psize; ii <= i + psize; ii ++)
	    for(int jj = j - psize; jj <= j + psize; jj ++) {	
		int tmp = ii * img->width + jj;
	 	rarray[tmp] = 255;
		garray[tmp] = 0;
		barray[tmp] = 0;
            }
	 }
    }
    
    IplImage* rr = double2img(rarray, img);
    IplImage* gg = double2img(garray, img);
    IplImage* bb = double2img(barray, img);
    
    IplImage* result = cvCreateImage(cvGetSize(img), img->depth, 3);
    cvMerge(bb, gg, rr, NULL, result);

    return result;
}
