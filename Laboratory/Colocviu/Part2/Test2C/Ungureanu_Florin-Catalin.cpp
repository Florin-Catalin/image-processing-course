#include "include/opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
#define MAX_PATH
int MIN, MAX;
int r;

bool isInside(int i, int j, int rows, int cols) {
	return(i >= 0 && i < rows && j >= 0 && j < cols);
}
int clam(int x)
{
	if (x > MAX)
		x = MAX;
	if (x < MIN)
		x = MIN;
	return x;
}
int mean(Mat img) {

	float average = 0;
	int M = img.rows * img.cols;

	for (int i = 0; i < img.rows; i++)
		for (int j = 0; j < img.cols; j++)
			average += img.at<uchar>(i, j);

	average /= M;

	return average;
}

// flag = 0  -> clamming 
// flag = 1  -> mean
Mat convolution(Mat src, Mat kernel , int flag ) {

	float S = 0;
	float k = 0;

	Mat dst = Mat(src.size(), CV_8UC1);

	k = kernel.cols / 2;
	if (k < 2) k = kernel.rows / 2;
	dst.convertTo(dst, CV_32FC1);

	for (int i = k; i < src.rows - k; i++) {
		for (int j = k; j < src.cols - k; j++) {

			S = 0.0;
			for (int ki = 0; ki < kernel.rows; ki++)
				for (int kj = 0; kj < kernel.cols; kj++) {
					if ( !flag )
					S += clam(src.at<float>(i,j) )* kernel.at<float>(ki, kj);
					else if (flag && isInside(ki,kj,i,j))
				 S += mean(src)* kernel.at<float>(ki, kj);
				}
			dst.at<float>(i, j) = S;
		}
	}
	dst.convertTo(dst, CV_8UC1);
	return dst;
}

void centering_transform(Mat img){
	//expects floating point image
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++){
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}
Mat generic_frequency_domain_filter(Mat src){
	//convert input image to float image
	Mat srcf;
	src.convertTo(srcf, CV_32FC1);
	//centering transformation
	centering_transform(srcf);
	//perform forward transform with complex image output
	Mat fourier;
	dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
	//split into real and imaginary channels
	Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	split(fourier, channels); // channels[0] = Re(DFT(I)), channels[1] = Im(DFT(I))
	//calculate magnitude and phase in floating point images mag and phi
	Mat mag, phi;
	magnitude(channels[0], channels[1], mag);
	phase(channels[0], channels[1], phi);
	//display the phase and magnitude images here
	// ......
	//insert filtering operations on Fourier coefficients here
	// ......
	//store in real part in channels[0] and imaginary part in channels[1]
	// ......
	//perform inverse transform and put results in dstf
	Mat dst, dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	//inverse centering transformation
	centering_transform(dstf);
	//normalize the result and put in the destination image
	normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	//Note: normalizing distorts the resut while enhancing the image display in the range [0,255].
	//For exact results (see Practical work 3) the normalization should be replaced with convertion:
	//dstf.convertTo(dst, CV_8UC1);
	return dst;
}

int min(int x, int y){
	if (x > y)
		return y;
	else
		return x;
}
int max(int x, int y){
	if (x < y)
		return y;
	else
		return x;
}

Mat create_kernel_matrix(Mat src)
{
	Mat_<float> kernel_mat(src.rows, src.cols);

	int xc = src.cols/2;
	int yc = src.rows/2;
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			kernel_mat(i, j) = (float) min(1, pow(i - xc, 2) / pow(r, 2) + pow(j - yc, 2) / pow(r, 2));
		}

	imshow("kernel_matrix", kernel_mat);
	waitKey(0);

	return kernel_mat;
}

void problem1()
{
	Mat img = imread("Images/cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("original image", img);
	Mat km = img.clone();

	km = create_kernel_matrix(img);
	imshow("kernel matrix", km);

	
	imshow("smth", generic_frequency_domain_filter(km));



	waitKey(0);
}



Mat kernel_matrix_w( int w  )
{
	Mat_<float> kernel_mat(3, w);

	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < w; j++)
		{
			kernel_mat(i, j) = -1; 
		}
	kernel_mat(3, w / 2) = w + 1;

	imshow("kernel_matrix", kernel_mat);
	waitKey(0);

	return kernel_mat;
}
void problem2( int w )
{
	Mat img = imread("Images/cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("original image", img);

	Mat res = img.clone();
	res = convolution(img, kernel_matrix_w(w), 0);
	imshow("aaa", res); 
	
}



void problem3( int w )
{
	Mat img = imread("Images/cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	imshow("original image", img);

	Mat res = img.clone();
	res = convolution(img, kernel_matrix_w(w), 1);

	
}

void compareTime() {
	double t1 = (double)getTickCount(); 
	// implementation one
	double t2 = (double)getTickCount(); 
	//second one
	t2 = ((double)getTickCount() - t2) / getTickFrequency();
	
	printf("Time = %.3f [ms]\n", (t1-t2) * 1000);

}
int comparePixel(Mat_<uchar> img1, Mat_<uchar> img2)
{
	int M = img1.rows * img1.cols;
	int dif = 0;
	for (int i = 0; i < img1.rows; i++)
		for (int j = 0; j < img1.cols; j++)
		{
			dif += abs(img1(i, j) -img2(i, j));
		}



	return dif / M;
}

int main(){
	// problem 1
	
	/*
	printf("Give 'r' parameter ");
	scanf("%d",&r);
	problem1();
	
	*/
	int w;
	printf("Give 'w' parameter");
	scanf("%d", &w);
	//problem 2
	/*
	
		printf("MIN= ");
		scanf("%d", &MIN);
		printf("MAX= ");
		scanf("%d", &MAX);
		problem2(w);
		*/
	return 0;
}