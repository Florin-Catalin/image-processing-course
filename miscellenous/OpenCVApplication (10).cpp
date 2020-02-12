// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"


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

void testImageOpenAndSave()
{
	Mat src, dst;

	src = imread("Images/Lena_24bits.bmp", CV_LOAD_IMAGE_COLOR);	// Read the image

	if (!src.data)	// Check for invalid input
	{
		printf("Could not open or find the image\n");
		return;
	}

	// Get the image resolution
	Size src_size = Size(src.cols, src.rows);

	// Display window
	const char* WIN_SRC = "Src"; //window for the source image
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Dst"; //window for the destination (processed) image
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, src_size.width + 10, 0);

	cvtColor(src, dst, CV_BGR2GRAY); //converts the source image to a grayscale one

	imwrite("Images/Lena_24bits_gray.bmp", dst); //writes the destination to file

	imshow(WIN_SRC, src);
	imshow(WIN_DST, dst);

	printf("Press any key to continue ...\n");
	waitKey(0);
}

void testNegativeImage()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]
		
		Mat src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height,width,CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				uchar val = src.at<uchar>(i,j);
				uchar neg = MAX_PATH-val;
				dst.at<uchar>(i,j) = neg;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image",src);
		imshow("negative image",dst);
		waitKey();
	}
}

void testParcurgereSimplaDiblookStyle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		double t = (double)getTickCount(); // Get the current time [s]

		// the fastest approach using the “diblook style”
		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;
		int w = src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
			}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("negative image", dst);
		waitKey();
	}
}

void testColor2Gray()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src = imread(fname);

		int height = src.rows;
		int width = src.cols;

		Mat dst = Mat(height,width,CV_8UC1);
		
		// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i=0; i<height; i++)
		{
			for (int j=0; j<width; j++)
			{
				Vec3b v3 = src.at<Vec3b>(i,j);
				uchar b = v3[0];
				uchar g = v3[1];
				uchar r = v3[2];
				dst.at<uchar>(i,j) = (r+g+b)/3;
			}
		}
		
		imshow("input image",src);
		imshow("gray image",dst);
		waitKey();
	}
}

void testBGR2HSV()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname);
		int height = src.rows;
		int width = src.cols;

		// Componentele d eculoare ale modelului HSV
		Mat H = Mat(height, width, CV_8UC1);
		Mat S = Mat(height, width, CV_8UC1);
		Mat V = Mat(height, width, CV_8UC1);

		// definire pointeri la matricele (8 biti/pixeli) folosite la afisarea componentelor individuale H,S,V
		uchar* lpH = H.data;
		uchar* lpS = S.data;
		uchar* lpV = V.data;

		Mat hsvImg;
		cvtColor(src, hsvImg, CV_BGR2HSV);

		// definire pointer la matricea (24 biti/pixeli) a imaginii HSV
		uchar* hsvDataPtr = hsvImg.data;

		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				int hi = i*width * 3 + j * 3;
				int gi = i*width + j;

				lpH[gi] = hsvDataPtr[hi] * 510 / 360;		// lpH = 0 .. 255
				lpS[gi] = hsvDataPtr[hi + 1];			// lpS = 0 .. 255
				lpV[gi] = hsvDataPtr[hi + 2];			// lpV = 0 .. 255
			}
		}

		imshow("input image", src);
		imshow("H", H);
		imshow("S", S);
		imshow("V", V);

		waitKey();
	}
}

void testResize()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		Mat dst1,dst2;
		//without interpolation
		resizeImg(src,dst1,320,false);
		//with interpolation
		resizeImg(src,dst2,320,true);
		imshow("input image",src);
		imshow("resized image (without interpolation)",dst1);
		imshow("resized image (with interpolation)",dst2);
		waitKey();
	}
}

void testCanny()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src,dst,gauss;
		src = imread(fname,CV_LOAD_IMAGE_GRAYSCALE);
		int k = 0.4;
		int pH = 50;
		int pL = k*pH;
		GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
		Canny(gauss,dst,pL,pH,3);
		imshow("input image",src);
		imshow("canny",dst);
		waitKey();
	}
}

void testVideoSequence()
{
	VideoCapture cap("Videos/rubic.avi"); // off-line video from file
	//VideoCapture cap(0);	// live video from web cam
	if (!cap.isOpened()) {
		printf("Cannot open video capture device.\n");
		waitKey(0);
		return;
	}
		
	Mat edges;
	Mat frame;
	char c;

	while (cap.read(frame))
	{
		Mat grayFrame;
		cvtColor(frame, grayFrame, CV_BGR2GRAY);
		Canny(grayFrame,edges,40,100,3);
		imshow("source", frame);
		imshow("gray", grayFrame);
		imshow("edges", edges);
		c = cvWaitKey(0);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished\n"); 
			break;  //ESC pressed
		};
	}
}


void testSnap()
{
	VideoCapture cap(0); // open the deafult camera (i.e. the built in web cam)
	if (!cap.isOpened()) // openenig the video device failed
	{
		printf("Cannot open video capture device.\n");
		return;
	}

	Mat frame;
	char numberStr[256];
	char fileName[256];
	
	// video resolution
	Size capS = Size((int)cap.get(CV_CAP_PROP_FRAME_WIDTH),
		(int)cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	// Display window
	const char* WIN_SRC = "Src"; //window for the source frame
	namedWindow(WIN_SRC, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_SRC, 0, 0);

	const char* WIN_DST = "Snapped"; //window for showing the snapped frame
	namedWindow(WIN_DST, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(WIN_DST, capS.width + 10, 0);

	char c;
	int frameNum = -1;
	int frameCount = 0;

	for (;;)
	{
		cap >> frame; // get a new frame from camera
		if (frame.empty())
		{
			printf("End of the video file\n");
			break;
		}

		++frameNum;
		
		imshow(WIN_SRC, frame);

		c = cvWaitKey(10);  // waits a key press to advance to the next frame
		if (c == 27) {
			// press ESC to exit
			printf("ESC pressed - capture finished");
			break;  //ESC pressed
		}
		if (c == 115){ //'s' pressed - snapp the image to a file
			frameCount++;
			fileName[0] = NULL;
			sprintf(numberStr, "%d", frameCount);
			strcat(fileName, "Images/A");
			strcat(fileName, numberStr);
			strcat(fileName, ".bmp");
			bool bSuccess = imwrite(fileName, frame);
			if (!bSuccess) 
			{
				printf("Error writing the snapped image\n");
			}
			else
				imshow(WIN_DST, frame);
		}
	}

}

void MyCallBackFunc(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDOWN)
		{
			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);
		}
}

void testMouseClick()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", MyCallBackFunc, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

Mat_<uchar> readImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src;
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		return src;
	}
}

std::vector<Point2f> lab1ReadInput()
{
	char path[MAX_PATH];
	while (openFileDlg(path)){

		FILE *f = fopen(path, "r");
		int points;
		fscanf(f, "%d", &points);
		std::vector<Point2f> result;
		for (int i = 0; i < points; i++)
		{
			float x, y;
			fscanf(f, "%f%f", &x, &y);
			Point2f point;
			point.x = x;
			point.y = y;
			result.push_back(point);
		}
		return result;
	}
}

std::vector<Point2f> lab1Normalize(std::vector<Point2f> points)
{
	int minX = 0, minY = 0;
	for (int i = 0; i < points.size(); i++)
	{
		if (minX > points.at(i).x)
		{
			minX = points.at(i).x;
		}
		if (minY > points.at(i).y)
		{
			minY = points.at(i).y;
		}
	}
	if (minX < 0 || minY < 0)
	{
		for (int i = 0; i < points.size(); i++)
		{
			points.at(i).x -= minX;
			points.at(i).y -= minY;
		}
	}
	return points;
}

void lab1PlotPoints(std::vector<Point2f> points)
{
	Mat_<uchar> img(500, 500);
	img.setTo(Scalar(255, 255, 255));
	for (int i = 0; i < points.size(); i++)
	{
		int x = points.at(i).x, y = points.at(i).y;
		circle(img, Point(x, y), 5, Scalar(0, 0, 0));
	}
	imshow("image", img);
	waitKey(0);
}

Point2f lab1CalculateParameters1(std::vector<Point2f> points)
{
	Point2f result = Point2f(0, 0);
	float sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
	float n = points.size();
	for (int i = 0; i < points.size(); i++)
	{
		sumXY += points.at(i).x*points.at(i).y;
		sumY += points.at(i).y;
		sumX += points.at(i).x;
		sumX2 += pow(points.at(i).x, 2);
	}
	result.y = (n*sumXY - sumX*sumY)/(n*sumX2 - pow(sumX, 2));
	result.x = (1 / n)*(sumY - result.y*sumX);
	return result;
}

Point2f lab1CalculateParameters2(std::vector<Point2f> points)
{
	Point2f params;
	float n = points.size();
	float sumX = 0, sumY = 0, sumXY = 0, sumY2X2 = 0;
	for (int i = 0; i < n; i++)
	{
		sumXY += points.at(i).x*points.at(i).y;
		sumY += points.at(i).y;
		sumX += points.at(i).x;
		sumY2X2 += pow(points.at(i).y, 2) - pow(points.at(i).x, 2);
	}
	params.x = -(1 / (float)2)*atan2(2 * sumXY-(2/n)*sumX*sumY, sumY2X2+(1/n)*pow(sumX,2)-(1/n)*pow(sumY, 2));
	params.y = (1 / n)*(cos(params.x)*sumX + sin(params.x)*sumY);
	return params;
}

void lab1ShowLine1(std::vector<Point2f> points, Point2f parameters)
{
	int x0, y0, x1, y1;
	x0 = 0;
	x1 = 500;
	y0 = parameters.x;
	y1 = parameters.x + x1*parameters.y;
	Mat_<uchar> img(500, 500);
	img.setTo(Scalar(255, 255, 255));
	for (int i = 0; i < points.size(); i++)
	{
		int x = points.at(i).x, y = points.at(i).y;
		circle(img, Point(x, y), 5, Scalar(0, 0, 0));
	}
	line(img, Point(x0, y0), Point(x1, y1), Scalar(155, 0, 0), 3);
	imshow("image", img);
	waitKey(0);
}

Point2d lab2CalculateParameters(int points)
{
	float p = 0.99, q = 0.3, s = 2, n = points;
	int N = log(1 - p) / log(1 - pow(q, s));
	int T = q * n;
	return Point2d(T, N);
}

Point2d lab2CalculateParameters()
{
	Mat_<uchar> image = readImage();
	std::vector<Point2d> points;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (image(i, j) == 0)
			{
				points.push_back(Point2d(i, j));
			}
		}
	}
	float p = 0.99, q = 0.3, s = 2, n = points.size();
	int N = log(1 - p) / log(1 - pow(q, s));
	int T = q * n;
	return Point2d(T, N);
}

float distance(Point2d point, int a, int b, int c)
{
	float aux = abs(a * point.x + b * point.y + c);
	float a2 = pow(a, 2);
	float b2 = pow(b, 2);
	return aux / (sqrt(a2 + b2));
}

void lab2RANSAC()
{
	Mat_<uchar> image = readImage();
	std::vector<Point2d> points;
	for (int i = 0; i < image.rows; i++)
	{
		for (int j = 0; j < image.cols; j++)
		{
			if (image(i, j) == 0)
			{
				points.push_back(Point2d(j, i));
			}
		}
	}
	Point2d parameters = lab2CalculateParameters(points.size());
	int N = parameters.y, T = parameters.x;
	float t = 10;

	int fit = 0, max = 0, lineA, lineB, lineC;
	for (int i = 0; i < N && fit < T; i++)
	{
		fit = 0;
		Point2d p1, p2;
		p1 = points.at(rand() % points.size());
		p2 = points.at(rand() % points.size());
		int a = p1.y - p2.y, b = p2.x - p1.x, c = p1.x * p2.y - p2.x * p1.y;
		for (Point2d p : points)
		{
			if (distance(p, a, b, c) < t)
			{
				fit++;
			}
		}
		if (fit > max)
		{
			max = fit;
			lineA = a;
			lineB = b;
			lineC = c;
		}
	}

	int Y1 = (-lineC) / lineB;
	int Y2 = ((-lineC) - (lineA*image.cols)) / lineB;
	line(image, Point(0, Y1), Point(image.cols, Y2), Scalar(0, 0, 0));
	imshow("RANSAC", image);
	waitKey();
}

int main()
{
	int op;
	do
	{
		system("cls");
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 1 - Calculate parameters\n");
		printf(" 2 - RANSAC\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
			{
				Point2d parameters = lab2CalculateParameters();
				std::cout << "T = " << parameters.x << " N = " << parameters.y;
				getchar();
				getchar();
				break;
			}
			case 2:
			{
				lab2RANSAC();
				break;
			}
		}
	}
	while (op!=0);
	return 0;
}