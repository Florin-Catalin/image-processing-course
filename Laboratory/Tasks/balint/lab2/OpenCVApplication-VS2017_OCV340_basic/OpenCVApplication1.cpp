// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

#define MIN2(a,b) (a < b ? a:b)
#define MAX2(a,b) (a > b ? a:b)
#define MIN3(a,b,c) (MIN2(MIN2(a,b),c))
#define MAX3(a,b,c) (MAX2(MAX2(a,b),c))

void testOpenImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image",src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName)==0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName,"bmp");
	while(fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(),src);
		if (waitKey()==27) //ESC pressed
			break;
	}
}

void separateRGB()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int height = src.rows, width = src.cols;
		Mat r(height, width, CV_8UC1);
		Mat g(height, width, CV_8UC1);
		Mat b(height, width, CV_8UC1);

		uchar *lpR = r.data;
		uchar *lpG = g.data;
		uchar *lpB = b.data;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				Vec3b pixel = src.at<Vec3b>(i, j);
				r.at<uchar>(i, j) = pixel[2];
				g.at<uchar>(i, j) = pixel[1];
				b.at<uchar>(i, j) = pixel[0];
			}
		}

		imshow("original", src);
		imshow("red", r);
		imshow("green", g);
		imshow("blue", b);

		waitKey(0);
	}
}

void colorToGrayscale()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int h = src.rows, w = src.cols;
		Mat dst(h, w, CV_8UC1);

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				Vec3b pixel = src.at<Vec3b>(i, j);
				dst.at<uchar>(i, j) = (pixel[0] + pixel[1] + pixel[2]) / 3;
			}
		}

		imshow("original", src);
		imshow("grayscale", dst);

		waitKey(0);
	}
}

void grayscaleToBw()
{
	printf("Input a threshold:\n>");
	int threshold = 0;
	scanf("%d", &threshold);
	
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int h = src.rows, w = src.cols;
		Mat dst(h, w, CV_8UC1);

		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				dst.at<uchar>(i, j) = (src.at<uchar>(i, j) < threshold) ? 0 : 255;
			}
		}

		imshow("original", src);
		imshow("bw", dst);

		waitKey(0);
	}
}

Vec3b rgbPixToHsvPix(Vec3b pix)
{
	float r = (float)pix[2]/255, g = (float)pix[1]/255, b = (float)pix[0]/255, max = MAX3(r, g, b), min = MIN3(r, g, b), c = max - min;
	float h = 0, s = 0, v = 0;

	//Value
	v = max;

	//Saturation
	s = (v != 0) ? c / v : 0;

	//Hue
	if (c != 0)
	{
		if (max == r) h = 60 * (g - b) / c;
		if (max == g) h = 120 + 60 * (b - r) / c;
		if (max == b) h = 240 + 60 * (r - g) / c;
	}
	else
	{
		h = 0;
	}

	if (h < 0)
	{
		h += 360;
	}
	
	return Vec3b((uchar)(h * 255 / 360), (uchar)(s * 255), (uchar)(v * 255));
}

void rgbToHsv()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int h = src.rows, w = src.cols;
		Mat hsv(h, w, CV_8UC3);
		Mat hue(h, w, CV_8UC1);
		Mat sat(h, w, CV_8UC1);
		Mat val(h, w, CV_8UC1);


		for (int i = 0; i < h; i++)
		{
			for (int j = 0; j < w; j++)
			{
				Vec3b opix = src.at<Vec3b>(i, j);
				Vec3b pix = rgbPixToHsvPix(opix);
				hsv.at<Vec3b>(i, j) = pix;
				hue.at<uchar>(i, j) = pix[0];
				sat.at<uchar>(i, j) = pix[1];
				val.at<uchar>(i, j) = pix[2];
			}
		}

		imshow("rgb", src);
		imshow("hsv", hsv);
		imshow("hue", hue);
		imshow("sat", sat);
		imshow("val", val);

		waitKey(0);
	}
}

void detectRed()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		int h = src.rows, w = src.cols;
		Mat hsvImg(h, w, CV_8UC3);
		Mat hsvChannels[3];
		Mat mask;

		cvtColor(src, hsvImg, CV_BGR2HSV);
		split(hsvImg, hsvChannels);

		inRange(hsvChannels[0], 0, 5, mask);

		imshow("mask", mask);
		waitKey(0);
	}
}

int main()
{
	int op;
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Separate RGB\n");
		printf(" 2 - Convert to grayscale\n");
		printf(" 3 - Grayscale to BW\n");
		printf(" 4 - RGB to H, S, V\n");
		printf(" 5 - Detect red\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				separateRGB();
				break;
			case 2:
				colorToGrayscale();
				break;
			case 3:
				grayscaleToBw();
				break;
			case 4:
				rgbToHsv();
				break;
			case 5:
				detectRed();
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}