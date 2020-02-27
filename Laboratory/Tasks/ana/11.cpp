// OpenCVApplication.cpp : Defines the entry point for the console application.
// 

#include "stdafx.h"
#include "common.h"
#include <queue>

int n = 2;


void L1CreateColorImage()
{
	Mat_<Vec3b> img(256,256,Vec3b(0,0,0));

	for(int i = 0; i < 256;i++)
		for (int j = 0; j < 256;j++)
		{
			if (i<=127 && j<=127)
			{
				img(i, j) = Vec3b(255,255,255);
			}
			else
				if (i<=127 && j>127)
				{
					img(i, j) = Vec3b(0,0,255);
			    }
			else
				if (i>127 && j<=127)
				{
					img(i,j)= Vec3b(0,255,0);
			}
			else 
				if (i>127 && j > 127)
				{
					img(i,j)= Vec3b(0,255,255);
			    }
		}

	   imshow("image colored", img);
	   waitKey();
}


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
				uchar neg = 255 - val;
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

void L1AdditiveFactor()
{
	int factor = 0;
    scanf("%d",&factor);

	if (factor < 0)
	{
		factor = (uchar)0;
	}
	else
	{
		if (factor > 255)
			factor = 255;
	}
	
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				int newVal = val + factor;
				dst.at<uchar>(i, j) = newVal;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("newVal image", dst);
		waitKey();
	}
}

void L1MultiplicativeFactor()
{
	float factor = 0;
	scanf("%f", &factor);



	if (factor < 0)
	{
		factor = (uchar)0;
	}
	else
	{
		if (factor > 255)
			factor = 255;
	}

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		double t = (double)getTickCount(); // Get the current time [s]

		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = Mat(height, width, CV_8UC1);
		// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
		// Varianta ineficienta (lenta)
		for (int i = 0; i<height; i++)
		{
			for (int j = 0; j<width; j++)
			{
				uchar val = src.at<uchar>(i, j);
				int newVal = val * factor;
				dst.at<uchar>(i, j) = newVal;
			}
		}

		// Get the current time again and compute the time difference [s]
		t = ((double)getTickCount() - t) / getTickFrequency();
		// Print (in the console window) the processing time in [ms] 
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("input image", src);
		imshow("newVal image", dst);
		imwrite("images/New.bmp",dst);
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
		int w = (int) src.step; // no dword alignment is done !!!
		for (int i = 0; i<height; i++)
			for (int j = 0; j < width; j++) {
				uchar val = lpSrc[i*w + j];
				lpDst[i*w + j] = 255 - val;
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
		double k = 0.4;
		int pH = 50;
		int pL = (int) k*pH;
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

void GeometricalFeaturesComputation(int event, int x, int y, int flags, void* param)
{
	//More examples: http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
	
	Mat* src = (Mat*)param;
	Vec3b color;
	if (event == CV_EVENT_LBUTTONDBLCLK)
	{
		printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
			x, y,
			(int)(*src).at<Vec3b>(y, x)[2],
			(int)(*src).at<Vec3b>(y, x)[1],
			(int)(*src).at<Vec3b>(y, x)[0]);
		
		color = src->at<Vec3b>(y, x);

		// compute the area
		int area = 0;
		for (int i = 0; i<(*src).rows; i++)
			for (int j = 0; j < (*src).cols; j++)
			{
				if ((*src).at<Vec3b>(i, j) == color)
					area++;
			}

		int sumRows = 0;
		int sumCols = 0;

		int cX, cY;
		
		for (int i = 0; i < (*src).rows; i++)

			for (int j = 0; j < (*src).cols; j++) {

				if ((*src).at<Vec3b>(i, j) == color)
				{
					sumRows += i;
					sumCols += j;

				}
			}


		cX = sumRows / area;
		cY = sumCols / area;

		//angle of elongation axis

		//axa de alungire

		int upper_sum=0, bottom_sum = 0;
		
		for (int i = 0; i < (*src).rows; i++)

			for (int j = 0; j < (*src).cols; j++) {

					if ((*src).at<Vec3b>(i, j) == color)
				{

					upper_sum = upper_sum + (i - cX) * (j - cY);
					bottom_sum = bottom_sum + (j - cY) * (j - cY) - (i - cX) * (i - cX);


				}

			}

		float fi = atan2(2 * upper_sum, bottom_sum) / 2;
		if (fi < 0)
			fi += CV_PI;
		fi = (fi * 180) / CV_PI;

		// perimeter
		int perimeterPixels = 0;
		float perimeter = 0;

		for (int i = 0; i < (*src).rows; i++)

			for (int j = 0; j < (*src).cols; j++)
			{
				if (((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i + 1, j) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i - 1, j) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i, j + 1) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i, j - 1) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i - 1, j - 1) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i - 1, j + 1) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i + 1, j - 1) != color) ||
					((*src).at<Vec3b>(i, j) == color && (*src).at<Vec3b>(i + 1, j + 1) != color)
					)

					perimeterPixels++;
				{

				}
			}

		// compute thiness ratio

		perimeter = perimeterPixels * CV_PI / 4;

		float thiness = (4 * CV_PI * area) / (perimeter*perimeter);

		float aspect;

		int rMin = (*src).rows;
		int rMax = 0;
		int cMin = (*src).cols;
		int cMax = 0;
		


		for (int i = 0; i < (*src).rows; i++)

			for (int j = 0; j < (*src).cols; j++)
			{
				if ((*src).at<Vec3b>(i, j) == color)
				{
					if (i < rMin) rMin = i;
					if (i > rMax) rMax = i;
					if (j < cMin) cMin = j;
					if (j > cMax) cMax = j;

				}
			}
		printf("rMax %d , rMin %d, cMax %d , cMin %d \n", rMax, rMin, cMax, cMin);
		aspect =(float) (cMax - cMin + 1) / (rMax - rMin + 1);


		printf("AREA: %d \n", area);
		printf("CENTER OF MASS: (%d , %d) \n", cX,cY);
		printf("THE ANGLE OF ELONGATION :%f \n", fi);
		printf("PERIMETER: %f \n", perimeter);
		printf("THINESS RATIO: %f \n", thiness);
		printf("ASPECT RATIO: %f", aspect);

		// draw the elongation lines

		float newFi = (fi * CV_PI) / 180;

		    float rowA = cX + tan(newFi)* (cMin - cY);
			float colA = cMin;
			float rowB = cX + tan(newFi) * (cMax - cY);
			float colB = cMax;

			Mat dst = src->clone();
			line(dst,Point(colA,rowA),Point(colB,rowB),Scalar(0,0,0));

			imshow("drawing elongation axis", dst);
			waitKey();


	       // horizontal projection

			Mat image_1((*src).rows,(*src).cols,CV_8UC3,Scalar(256,256,256));

			int countRows = 0;
			for (int i = 0; i < (*src).rows; i++)
			{
				countRows = 0;
				for (int j = 0; j < (*src).cols; j++)
				{
					if ((*src).at<Vec3b>(i, j) == color)
					{
						image_1.at<Vec3b>(i, countRows) = color;
						countRows++;
					}
					
				}
				
			}
			imshow("projection 1",image_1);

			// vertical projection 

			Mat image_2((*src).rows, (*src).cols, CV_8UC3, Scalar(256, 256, 256));

			int countCols = 0;
			for (int j = 0; j < (*src).cols; j++)
			{
				countCols = 0;
				for (int i = 0; i < (*src).rows; i++)
				{
					if ((*src).at<Vec3b>(i,j) == color)
					{
						image_2.at<Vec3b>(countCols,j) = color;
						countCols++;
					}
				}
			}

			imshow("projection 2", image_2);			
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

/* Histogram display function - display a histogram using bars (simlilar to L3 / PI)
Input:
name - destination (output) window name
hist - pointer to the vector containing the histogram values
hist_cols - no. of bins (elements) in the histogram = histogram image width
hist_height - height of the histogram image
Call example:
showHistogram ("MyHist", hist_dir, 255, 200);
*/
void showHistogram(const std::string& name, int* hist, const int  hist_cols, const int hist_height)
{
	Mat imgHist(hist_height, hist_cols, CV_8UC3, CV_RGB(255, 255, 255)); // constructs a white image

	//computes histogram maximum
	int max_hist = 0;
	for (int i = 0; i<hist_cols; i++)
	if (hist[i] > max_hist)
		max_hist = hist[i];
	double scale = 1.0;
	scale = (double)hist_height / max_hist;
	int baseline = hist_height - 1;

	for (int x = 0; x < hist_cols; x++) {
		Point p1 = Point(x, baseline);
		Point p2 = Point(x, baseline - cvRound(hist[x] * scale));
		line(imgHist, p1, p2, CV_RGB(255, 0, 255)); // histogram bins colored in magenta
	}

	imshow(name, imgHist);
}

void L1Inverse()
{
	float matrix[9] = { 2.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
	Mat M(3, 3, CV_32FC1,matrix);
	std::cout << M.inv();
	getchar();
	getchar();

}

void rgb_channels()
{
	Mat img = imread("Images/flowers_24bits.bmp", CV_LOAD_IMAGE_COLOR);
	Mat img1(img.rows, img.cols, CV_8UC3);
	Mat img2(img.rows, img.cols, CV_8UC3);
	Mat img3(img.rows, img.cols, CV_8UC3);

	for(int i=0;i<img.rows;i++)
		for (int j = 0; j < img.cols; j++)
		{
			img1.at<Vec3b>(i, j) = Vec3b(img.at<Vec3b>(i, j)[0],0,0);
			img2.at<Vec3b>(i, j) = Vec3b(0,img.at<Vec3b>(i, j)[1],0);
			img3.at<Vec3b>(i, j) = Vec3b(0,0,img.at<Vec3b>(i, j)[2]);
		}

	 imshow("blue", img1);
	 imshow("green", img2);
	 imshow("red", img3);

	 waitKey(0);
}

void color_to_grayscale()
{
	Mat img = imread("Images/flowers_24bits.bmp", CV_LOAD_IMAGE_COLOR);
	Mat img_gray(img.rows, img.cols, CV_8UC1);

	for (int i = 0; i<img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			img_gray.at<uchar>(i, j) = (img.at<Vec3b>(i, j)[0], img.at<Vec3b>(i, j)[1], img.at<Vec3b>(i, j)[2]) / 3;
		}

	imshow("init", img);
	imshow("final", img_gray);

	waitKey(0);

}

void grayscale_to_black_white()
{
	Mat img = imread("Images/cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img1(img.rows, img.cols, CV_8UC3);

	int treshold;
	scanf("%d", &treshold);

	for (int i = 0; i<img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			if (img.at<uchar>(i, j) > treshold) //black
			{
				img1.at<Vec3b>(i, j) = Vec3b(0,0,0);

			}

			else  //white
			{
				img1.at<Vec3b>(i, j) = Vec3b(255,255,255);
			}
		}

	imshow("init",img);
	imshow("final",img1);

	waitKey();
}
	

void rgb_to_hsv()
{
	Mat img = imread("Images/flowers_24bits.bmp", CV_LOAD_IMAGE_COLOR);
	Mat img1(img.rows, img.cols, CV_8UC1);
	Mat img2(img.rows, img.cols, CV_8UC1);
	Mat img3(img.rows, img.cols, CV_8UC1);
	

	for (int i = 0; i<img.rows; i++)
		for (int j = 0; j < img.cols; j++)
		{
			float b = img.at<Vec3b>(i, j)[0] / 255.f;
			float g = img.at<Vec3b>(i, j)[1] / 255.f;
			float r = img.at<Vec3b>(i, j)[2] / 255.f;

			float M = max(r,max(b,g));
			float m = min(r,min(b,g));
			float C = M - m;

			float V = M; // value
			float S;
			float H;

			//saturation
			if (C)
			{
				S = C / V;
			}
			else S = 0;

			//hue
			if (C)
			{
				if (M == r) H = 60 * (g-b) / C;
				if (M == g) H = 120 + 60 * (b-r) / C;
				if(M==b) H = 240 + 60 * (r-g) / C;
			}

			else H = 0;

			if (H < 0)
			{
				H = H + 360;
			}

			float H_norm = H * 255 / 360.f;
			float S_norm = S * 255;
			float V_norm = V * 255;
			
			img1.at<uchar>(i, j) = H_norm;
			img2.at<uchar>(i, j) = S_norm;
			img3.at<uchar>(i, j) = V_norm;
		}

	imshow("H", img1);
	imshow("S", img2);
	imshow("V", img3);

	waitKey(0);
}

void testGeometricalFeatures()
{
	Mat src;
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname,CV_LOAD_IMAGE_COLOR);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", GeometricalFeaturesComputation, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
}

void bfs_labelling()
{
	Mat src;

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		int label = 0;
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		imshow("initial image", src);

		int i_index[8] = { -1, 0, 1, 0, -1, -1, 1, 1 };
        int j_index[8] = { 0, -1, 0, 1, -1, 1, -1, 1 };
        uchar neighbor[8];

        Mat labels = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));

      for (int i = 0; i < src.rows; i++) {

	   for (int j = 0; j < src.cols; j++) {

		if ((src.at<uchar>(i, j) == 0) && (labels.at<int>(i, j) == 0))
		{

			label++;

			std::queue<Point2i> Q;

			labels.at<int>(i, j) = label;

			Q.push(Point2i(i, j));

			while (!Q.empty())
			{
				Point2i q = Q.front();
				Q.pop();

				// check neighbours

				for (int k = 0; k < 8; k++)
					neighbor[k] = src.at<uchar>(q.x + i_index[k], q.y + j_index[k]);

				for (int k = 0; k < 8; k++) {

					if (neighbor[k] == 0 && labels.at<int>(q.x + i_index[k], q.y + j_index[k]) == 0)
					{

						labels.at<int>(q.x + i_index[k], q.y + j_index[k]) = label;

						Q.push(Point2i(q.x + i_index[k], q.y + j_index[k]));

					}
				}
			}
		}
	}

}

printf("\n%d\n", label);

Mat dst(src.rows, src.cols, CV_8UC3, Scalar(255, 255, 255));

Vec3b *colors = new Vec3b[label + 1];

for (int k = 1; k <= label; k++)
colors[k] = Vec3b(std::rand() % 255, std::rand() % 255, std::rand() % 255);

for (int i = 0; i < dst.rows; i++) {

	for (int j = 0; j < dst.cols; j++) {

		if (labels.at<int>(i, j) != 0)
		{
			dst.at<Vec3b>(i, j) = Vec3b(colors[labels.at<int>(i, j)]);
		}

	}
}

imshow("final", dst);

	}
}

int min_func(std::vector<int> val)
{
	int min = val.at(0);
	for (int i = 0; i < val.size(); i++)
	{
		if (val.at(i) < min)
			min = val.at(i);
	}

	return min;
}

void equivalence_classes_labelling()
{
	Mat src;

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		int i_index[4] = { -1,-1,-1,0 };
		int j_index[4] = { -1,0,1,-1 };
		uchar neighbor[4];

		int label = 0;
		int x;

		Mat labels = Mat(src.rows, src.cols, CV_32SC1, Scalar(0));

		std::vector<std::vector<int>> edges;


		for (int i = 0; i < src.rows; i++) {

			for (int j = 0; j < src.cols; j++)
			{
				if (src.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0)
				{
					std::vector<int> L;

					for (int k = 0; k < 4; k++)
						neighbor[k] = src.at<uchar>(i + i_index[k], j + j_index[k]);

					for (int k = 0; k < 4; k++)
					{
						if (labels.at<int>(i + i_index[k], j + j_index[k]) > 0)
						{
							L.push_back(labels.at<int>(i + i_index[k], j + j_index[k]));
						}
					}

					if (L.size() == 0)
					{
						label++;
						edges.resize(label + 1);
						labels.at<int>(i, j) = label;
					}
					else
					{
						x = min_func(L);
						labels.at<int>(i, j) = x;
						for (int p = 0; p < L.size(); p++)
						{
							if (x != L.at(p))
							{
								edges[x].push_back(L.at(p));
								edges[L.at(p)].push_back(x);
							}
						}

					}

				}
			}
		}



		int newLabel = 0;
		int *newLabels = new int[label + 1]{0};

		for (int i = 1; i <= label; i++)
		{
			if (newLabels[i] == 0)
			{
				newLabel++;

				std::queue<int> Q;

				newLabels[i] = newLabel;

				Q.push(i);
				while (Q.size() != 0)
				{
					int x = Q.front();
					Q.pop();
					for (int p = 0; p < edges[x].size(); p++)
					{
						if (newLabels[edges[x][p]] == 0) {
							newLabels[edges[x][p]] = newLabel;
							Q.push(edges[x][p]);
						}
					}

				}
			}
		}



		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++)
			{
				labels.at<int>(i, j) = newLabels[labels.at<int>(i, j)];
			}

		printf("\n%d\n", label);
		



		Mat dst(src.rows, src.cols, CV_8UC3, Scalar(255, 255, 255));

		// color

		Vec3b *colors = new Vec3b[label + 1];

		for (int k = 1; k <= label; k++)
			colors[k] = Vec3b(std::rand() % 255, std::rand() % 255, std::rand() % 255);

		for (int i = 0; i < src.rows; i++) {

			for (int j = 0; j < src.cols; j++) {

				if (labels.at<int>(i, j) != 0)
				{
					dst.at<Vec3b>(i, j) = colors[labels.at<int>(i, j)];
				}

			}
		}

		imshow("initial image", src);
		imshow("result", dst); 
		waitKey();
	}

}

void border_tracing()
{
	Mat src;

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		int a = 0;
		int dir = 7;
		int dirstart;
		int directie[500];
		std::vector<Point2i> vec;
		int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
		int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

		Mat dest(src.rows, src.cols, CV_8UC1,Scalar(255));

		int ok = 0;
		for (int i = 0; i < src.rows && ok==0; i++)
			for (int j = 0; j < src.cols && ok==0; j++)
				if (src.at<uchar>(i, j) == 0) {
					Point2i p(j, i);
					vec.push_back(p);
					printf("The first point is: P(%d,%d)\n", i,j);
					ok = 1;
				}
	
		do {

			if (dir % 2 == 1)
				dirstart = (dir + 6) % 8;
			else
				dirstart = (dir + 7) % 8;

			int dir_start = dirstart;
			
			for (int k = 0; k < 8; k++) {
				if (src.at<uchar>(vec.back().y + di[dir_start], vec.back().x + dj[dir_start]) == 0) 
				{
					dest.at<uchar>(vec.back().y + di[dir_start], vec.back().x + dj[dir_start]) = 0;
					
					Point2i p(vec.back().x + dj[dir_start], vec.back().y + di[dir_start]);
					
					vec.push_back(p);
					
					break;
				}
				else
				dir_start = (dir_start + 1) % 8;
			}

			dir = dir_start;
			directie[a] = dir;
			a++;

		} while ( ! ((vec.size()>2)  && (vec.at(1) == vec.at(vec.size()-1)) && (vec.at(0) == vec.at(vec.size()-2))) );


	

        //compute 
		int DC[500];
		for (int i = 0; i < a - 2; i++)
		{
			DC[i] = (directie[i + 1] - directie[i] + 8) % 8;
		}

		// print AC
		printf("AC: \n\n");
		for (int i = 0; i < a - 2; i++)
		{
			printf("%d  ", directie[i]);
		}
		
		//print DC
		printf("DC: \n\n\n");
		for (int i = 0; i < a - 2; i++)
		{
			printf("%d  ", DC[i]);
		}

		imshow("img", src);
		imshow("Contur", dest);
		waitKey();


	}
}

void border_reconstruction()
{

	Mat dest = imread("Images/gray_background.bmp", CV_LOAD_IMAGE_GRAYSCALE);	// Read the image
	FILE *p;
	p = fopen("Images/reconstruct.txt","r");
	
	int y0, x0, n;

	fscanf(p,"%d",&x0);
	fscanf(p,"%d",&y0);
	fscanf(p,"%d",&n);

	printf("%d %d %d: \n\n\n", y0,x0,n);

	int array[3159];
	int count = 0;
	for (int i = 0; i < 3149; i++)
	{
		fscanf(p,"%d",&array[i]);
		count++;
	}

	printf("count: %d", count);

	for (int i = 0; i < 3149; i++)
	{
		printf("%d  ",array[i]);
	}

	int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
	int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

	dest.at<uchar>(x0, y0)= 0;
	int cx0, cy0;
	cx0 = x0;
	cy0 = y0;
	for (int i = 0; i < n; i++)
	{
		dest.at<uchar>(cx0 + di[array[i]], cy0 + dj[array[i]] ) = 0;
		cx0 = cx0 + di[array[i]];
		cy0 = cy0 + dj[array[i]];
	}


	imshow("Contur", dest);
	waitKey(); 
}

bool inBounds(int i, int j, int rows, int cols) {
	return(i >= 0 && i < rows && j >= 0 && j < cols);
}

void auxdilation(Mat *src, Mat *dst)
{
	*dst = Mat(src->rows, src->cols, CV_8UC1, Scalar(255, 255, 255));
	for (size_t i = 0; i < src->rows; i++)
	{
		for (size_t j = 0; j < src->cols; j++)
		{
			if (src->at<uchar>(i, j) == 0) {
				if (inBounds(i, j - 1, src->rows, src->cols) == true)
				dst->at<uchar>(i, j - 1) = 0;
				if (inBounds(i, j + 1, src->rows, src->cols) == true)
				dst->at<uchar>(i, j + 1) = 0;
				if (inBounds(i+1, j, src->rows, src->cols) == true)
				dst->at<uchar>(i + 1, j) = 0;
				if (inBounds(i-1, j, src->rows, src->cols) == true)
				dst->at<uchar>(i - 1, j) = 0;
				if (inBounds(i, j, src->rows, src->cols) == true)
				dst->at<uchar>(i, j) = 0;
			}
		}
	}
}

void dilation(int n)
{
	char fname[MAX_PATH];
	Mat src, dst, aux1, aux2;
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		aux1 = src.clone();
		for (int i = 0; i < n; i++) {
			auxdilation(&aux1, &aux2);
			dst = aux2.clone();
			aux1 = aux2.clone();
		}
		imshow("input image", src);
		imshow("dilated image", dst);
		waitKey();
	}
}


void auxErosion(Mat *src, Mat *dst)
{
	*dst = src->clone();
	for (size_t i = 0; i < src->rows; i++)
	{
		for (size_t j = 0; j < src->cols; j++)
		{
			if (src->at<uchar>(i, j) == 255) {
				if (inBounds(i, j - 1, src->rows, src->cols) == true)
					dst->at<uchar>(i, j - 1) = 255;
				if (inBounds(i, j + 1, src->rows, src->cols) == true)
					dst->at<uchar>(i, j + 1) = 255;
				if (inBounds(i + 1, j, src->rows, src->cols) == true)
					dst->at<uchar>(i + 1, j) = 255;
				if (inBounds(i - 1, j, src->rows, src->cols) == true)
					dst->at<uchar>(i - 1, j) = 255;
				if (inBounds(i, j, src->rows, src->cols) == true)
					dst->at<uchar>(i, j) = 255;
			}
		}
	}
}

void erosion(int n)
{
	char fname[MAX_PATH];
	Mat src, dst, aux1, aux2;
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		aux1 = src.clone();
		for (int i = 0; i < n; i++) {
			auxErosion(&aux1, &aux2);
			dst = aux2.clone();
			aux1 = aux2.clone();
		}
		imshow("input image", src);
		imshow("erosion image", dst);
		waitKey();
	}
}

void opening()
{
	
	Mat aux, fin, src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		auxErosion(&src, &aux);
		auxdilation(&aux, &fin);

		imshow("input image", src);
		imshow("opening image", fin);
		waitKey();
	}
}

void closure()
{

	Mat aux, fin, src;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		auxdilation(&src, &aux);
		auxErosion(&aux, &fin);

		imshow("input image", src);
		imshow("closure image", fin);
		waitKey();
	}

}

void boundary_extraction()
{
	Mat src, dest;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		auxErosion(&src, &dest);
		for(int i=0;i<src.rows;i++)
			for (int j = 0; j < src.cols; j++) {
				dest.at<uchar>(i, j) = 255 - abs(src.at<uchar>(i,j) - dest.at<uchar>(i,j));
			}

		imshow("input image", src);
		imshow("closure image", dest);
		waitKey();
	}
}

bool equals(Mat m1, Mat m2) {
	
	for (int i = 0; i < m1.rows; i++)
	{
		for (int j = 0; j < m1.cols; j++)
		{
			if (m1.at<uchar>(i, j) != m2.at<uchar>(i, j)) 
				return false;
		}
	}
	return true;
}

void intersect(Mat m1, Mat m2, Mat *dst) {

	*dst = Mat(m1.rows, m1.cols, CV_8UC1, Scalar(255));
	
	for (int i = 0; i < m1.rows; i++)
	{
		for (int j = 0; j < m1.cols; j++)
		{
			if (m1.at<uchar>(i, j) == 0 && m2.at<uchar>(i, j) == 0) {
				dst->at<uchar>(i, j) = 0;
			}
		}
	}

}

void region_filling() {

	char fname[MAX_PATH];

	Mat src;
	Mat X0, X1;
	Mat aux, borders;

	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		
		X0 = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		X1 = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		
		borders = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));

		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				
				if (src.at<uchar>(i, j) == 0)
					borders.at<uchar>(i, j) = 255;
				else
					borders.at<uchar>(i, j) = 0;
				
			}
		}

		int x = src.rows / 2;
		int y = src.cols/ 2;

		X1.at<uchar>(x, y) = 0;
		do {
			X1.copyTo(X0);
			auxdilation(&X1, &aux);
			intersect(aux, borders, &X1);
		} while (!equals(X0,X1));


		imshow("Initial image ", src);
		imshow("Final image ", X0);
		waitKey();
	
	}
}

void compute_histogram()
{
		Mat src;
		int v[256];
		char fname[MAX_PATH];

		while (openFileDlg(fname)) {

			src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

			for (int k = 0; k < 256; k++) 
				v[k] = 0;

			for (int k = 0; k < 256; k++) 
			{
				for (int i = 0; i < src.rows; i++) {
					for (int j = 0; j < src.cols; j++) {
						if (src.at<uchar>(i, j) == k) v[k]++;
					}
				}
			}

			imshow("src", src);
			showHistogram("dst", v, 256, 256);

			//compute the mean

			float mean = 0;
			int dist = 0;
			int  totalPixels = src.cols * src.rows;

			for (int k = 0; k < 256; k++) {

				if (v[k] != 0) {
					mean += k * v[k];
				}
			}
			mean = mean / totalPixels;
			printf("Mean =%f ", mean);

			// compute the standard deviation
			float dev = 0;

			for (int k = 0; k < 256; k++) {

				if (v[k] != 0) {
					dev += (k - mean)*(k - mean) * v[k];
				}
			}

			dev = sqrt(dev / totalPixels);
			printf("Deviation =%f", dev);


			waitKey();
		}
}

void tresholding_algorithm()
{

	Mat src;
	int v[256];
	char fname[MAX_PATH];

	while (openFileDlg(fname)) {

		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		for (int k = 0; k < 256; k++)
			v[k] = 0;
		for (int k = 0; k < 256; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k) v[k]++;

				}
			}
		}

		int iMax = 0, iMin = 0;
		int Tfinal = 0, Tintermediar = 0;
		int m1 = 0, m2 = 0, N1 = 0, N2 = 0;

		for (int k = 0; k < 256; k++) {
			if (v[k] != 0) {
				iMin = k;
				break;
			}
		}

		for (int k = 255; k >0; k--) {
			if (v[k] != 0) {
				iMax = k;
				break;
			}
		}

		printf("iMin: %d \n", iMin);
		printf("iMax: %d \n", iMax);

		Tfinal = (iMin + iMax) / 2;

		do {
			    N1 = 0;
				N2 = 0;
				m1 = 0;
				m2 = 0;

			    Tintermediar = Tfinal;

				for (int k = iMin; k < Tintermediar; k++)
				{
						m1 += k * v[k];
						N1 += v[k];
				}

				for (int k = Tintermediar; k <= iMax; k++) 
				{
					m2 += k * v[k];
					N2 += v[k];
				}
			
				Tfinal = (m1 / N1 + m2 / N2) / 2;

		} while (abs(Tfinal - Tintermediar) > 1);

		printf("TFinal: %d \n",Tfinal);

		Mat dst = Mat(src.size(), CV_8UC1);

		for (int i = 0; i < src.rows; i++)

			for (int j = 0; j < src.cols; j++) {

				if (src.at<uchar>(i, j) <= Tfinal) 
					dst.at<uchar>(i, j) = 0;
				else dst.at<uchar>(i, j) = 255;

			}

		imshow("src", src);
		imshow("dst", dst);

		showHistogram("histDst", v, 256, 256);
		waitKey();
	}
}

void brightness_change()  
{
	Mat src;
	int v[256];
	int a[256];
	char fname[MAX_PATH];

	int coeff;
	printf("Introduce the brightness coeff: \n");
	scanf("%d",&coeff);

	while (openFileDlg(fname)) {

		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = src.clone();

		for (int k = 0; k < 256; k++) 
			v[k] = 0;
		for (int k = 0; k < 256; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k)
						v[k]++;
				}
			}
		}

		for (int k = 0; k < 256; k++)
			a[k] = 0;
		for (int k = 0; k < 256; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k)
						a[k+coeff]++;
				}
			}
		}

		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dst.at<uchar>(i, j) = src.at<uchar>(i, j) + coeff;
			}
		}
			
	
		showHistogram("initial", v, 256, 256);
		showHistogram("final", a, 256, 256);
		imshow("initial image",src);
		imshow("final image",dst);
		waitKey();
	}
}

void contrast_change() 
{
	int GMax, GMin;
	printf("Introduce the  GMax coeff: \n");
	scanf("%d", &GMax);
	printf("Introduce the GMin coeff: \n");
	scanf("%d", &GMin);

	Mat src;
	int v[256];
	int a[256];
	char fname[MAX_PATH];
	int iMax, iMin;
	int newG;
	Mat dst;

	while (openFileDlg(fname)) {

		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));

		for (int k = 0; k < 256; k++)
			v[k] = 0;
		for (int k = 0; k < 256; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (src.at<uchar>(i, j) == k)
						v[k]++;
				}
			}
		}

		for (int k = 0; k < 256; k++) {
			if (v[k] != 0) {
				iMin = k;
				break;
			}
		}

		for (int k = 255; k >0; k--) {
			if (v[k] != 0) {
				iMax = k;
				break;
			}
		}

		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				dst.at<uchar>(i, j) = GMin + (src.at<uchar>(i, j) - iMin)*((float)(GMax - GMin) / (iMax - iMin));
			}
		}

		for (int k = 0; k < 256; k++)
			a[k] = 0;
		for (int k = 0; k < 256; k++) {
			for (int i = 0; i < src.rows; i++) {
				for (int j = 0; j < src.cols; j++) {
					if (dst.at<uchar>(i, j) == k)
						a[k]++;
				}
			}
		}

		showHistogram("initial_histo", v, 256, 256);
		showHistogram("final_histo", a, 256, 256);
		imshow("intial",src);
		imshow("final", dst);
		waitKey();
	}
}


void gamma_correction() {
	char fname[MAX_PATH];
	
	float gamma;
	printf("Introduce the gamma value : \n");
	scanf("%d", &gamma);
	
	while (openFileDlg(fname))
	{
	
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();
		int L = 255;
		int h[256] = { 0 };
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				h[src.at<uchar>(i, j)]++;
			}
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = L * pow((dst.at<uchar>(i, j) / (float)L), gamma);
				if (dst.at<uchar>(i, j) > 255) {
					dst.at<uchar>(i, j) = 255;
				}
				if (dst.at<uchar>(i, j) < 0) {
					dst.at<uchar>(i, j) = 0;
				}
			}
		}
		int h1[256] = { 0 };
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				h1[dst.at<uchar>(i, j)]++;
			}
		}
		showHistogram("Histogram", h, 256, 500);
		showHistogram("Histogram2", h1, 256, 500);
		imshow("input image", src);
		imshow("output image", dst);
		waitKey();
	}
}

void histogram_equalization() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		float m = width * height * 1.0;
		Mat dst = src.clone();
		int L = 255;
		int h[256] = { 0 };
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				h[src.at<uchar>(i, j)]++;
			}
		}
		float hn[256] = { 0 };
		for (int i = 0; i < 256; i++) {
			hn[i] = h[i] / m;
		}
		float p[256] = { 0.0 };
		p[0] = hn[0];
		for (int i = 1; i < 256; i++) {
			p[i] = p[i - 1] + hn[i];
		}

		int h1[256] = { 0 };
		for (int i = 0; i < 256; i++) {
			h1[(int)(255 * p[i])] = h[i];
		}
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				dst.at<uchar>(i, j) = 255 * p[src.at<uchar>(i, j)];
			}
		}
		showHistogram("Histogram", h, 256, 500);
		showHistogram("Histogram2", h1, 256, 500);
		imshow("input image", src);
		imshow("output image", dst);
		waitKey();
	}
}

void mean_filter_3()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		int dx[9] = { -1,-1,-1,0,0,0,1,1,1 };
		int dy[9] = { -1,0,1,-1,0,1,-1,0,1 };
		int kernel_value[9] = { 1,1,1,1,1,1,1,1,1 };
		int kernel_dim = 3;
		int height = src.rows;
		int width = src.cols;
		int sum;

		for (int i = 1; i < src.rows-1; i++)
		{
			for (int j = 1; j < src.cols-1; j++)
			{
				// current pixel is (i,j)
				sum = 0;
				for (int k = 0; k <= 8; k++)
				{
					sum += kernel_value[k] * src.at<uchar>(i+dy[k], j+dx[k]);
				}
				dst.at<uchar>(i, j) = sum / 9;
			}
		}

		imshow("init image",src);
		imshow("final image", dst);
	
	}
}

void mean_filter_5()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		int dx[25] = { -2,-2,-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2 };
		int dy[25] = { -2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2,-2,-1,0,1,2 };
		int kernel_value[25] = { 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 };
		int sum;

		for (int i = 2; i < src.rows - 2; i++)
		{
			for (int j = 2; j < src.cols - 2; j++)
			{
				// current pixel is (i,j)
				sum = 0;
				for (int k = 0; k <= 24; k++)
				{
					sum += kernel_value[k] * src.at<uchar>(i + dy[k], j + dx[k]);
				}
				dst.at<uchar>(i, j) = sum / 25;
			}
		}

		imshow("init image", src);
		imshow("final image", dst);

	}
}

void gaussian_filter()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		int dx[9] = { -1,-1,-1,0,0,0,1,1,1 };
		int dy[9] = { -1,0,1,-1,0,1,-1,0,1 };
		int kernel_value[9] = { 1,2,1,2,4,2,1,2,1};
		int kernel_dim = 3;
		int height = src.rows;
		int width = src.cols;
		int sum;

		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				// current pixel is (i,j)
				sum = 0;
				for (int k = 0; k <= 8; k++)
				{
					sum += kernel_value[k] * src.at<uchar>(i + dy[k], j + dx[k]);
				}
				dst.at<uchar>(i, j) = sum / 16;
			}
		}

		imshow("init image", src);
		imshow("final image", dst);

	}
}

void laplace_filter()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		int dx[9] = { -1,-1,-1,0,0,0,1,1,1 };
		int dy[9] = { -1,0,1,-1,0,1,-1,0,1 };
		int kernel_value[9] = { 0,-1,0,-1,4,-1,0,-1,0 };
		int kernel_dim = 3;
		int height = src.rows;
		int width = src.cols;
		int sum;

		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				// current pixel is (i,j)
				sum = 0;
				for (int k = 0; k <= 8; k++)
				{
					sum += kernel_value[k] * src.at<uchar>(i + dy[k], j + dx[k]);
				}
				if (sum < 0) sum = 0;
				if (sum > 255) sum = 255;			
				dst.at<uchar>(i, j) = sum;
			}
		}

		imshow("init image", src);
		imshow("final image", dst);
	}
}

void high_pass_filter()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		int dx[9] = { -1,-1,-1,0,0,0,1,1,1 };
		int dy[9] = { -1,0,1,-1,0,1,-1,0,1 };
		int kernel_value[9] = { 0,-1,0,-1,5,-1,0,-1,0 };
		int kernel_dim = 3;
		int height = src.rows;
		int width = src.cols;
		int sum;

		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				// current pixel is (i,j)
				sum = 0;
				for (int k = 0; k <= 8; k++)
				{
					sum += kernel_value[k] * src.at<uchar>(i + dy[k], j + dx[k]);
				}
				if (sum < 0) sum = 0;
				if (sum > 255) sum = 255;
				dst.at<uchar>(i, j) = sum;
			}
		}

		imshow("init image", src);
		imshow("final image", dst);

	}
}

void centering_transform(Mat m)
{
	for (int i = 0; i <= m.rows - 1; i++)
	{
		for (int j = 0; j <= m.cols - 1; j++)
		{
			m.at<float>(i, j) = ((i + j) & 1) ? -m.at<float>(i, j) : m.at<float>(i,j);
		}
	}
}

void magnitude_lag()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat srcf;
		src.convertTo(srcf,CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier,DFT_COMPLEX_OUTPUT);

		Mat channels[] = {Mat::zeros(src.size(),CV_32F), Mat::zeros(src.size(),CV_32F)};
		split(fourier, channels);

		Mat mag;
		Mat dst;
		magnitude(channels[0],channels[1],mag);

		for (int i = 0; i <= mag.rows - 1; i++)
		{
			for (int j = 0; j <= mag.cols - 1; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}

		normalize(mag,dst,0,255,NORM_MINMAX,CV_8UC1);
		imshow("init image", src);
		imshow("final image",dst);
	}
}

void ideal_low_pass_filter()
{
	char fname[MAX_PATH];

	float radius;
	printf("Introduce the radius value : \n");
	scanf("%f", &radius);

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(),CV_32F), Mat::zeros(src.size(),CV_32F) };
		split(fourier, channels);

		// filtering operations
		for (int i = 0; i < src.rows; i++)
			for (int j = 0; j < src.cols; j++)
			{
				if ( (pow(src.rows/2 -i,2) + pow(src.cols/2-j,2) ) > pow(radius,2)  )
				{
					channels[0].at<float>(i, j) = 0;
					channels[1].at<float>(i, j) = 0;
				}
	       }

		//perform the inverse transform
		Mat dest, dstf;
		merge(channels,2,fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		
		centering_transform(dstf);

		normalize(dstf, dest,0,255,NORM_MINMAX, CV_8UC1 );

		imshow("init image", src);
		imshow("final image", dest);
	}
}

void ideal_high_pass_filter()
{
	char fname[MAX_PATH];

	float radius;
	printf("Introduce the radius value : \n");
	scanf("%f", &radius);

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(),CV_32F), Mat::zeros(src.size(),CV_32F) };
		split(fourier, channels);

		Mat mag;
		Mat dst, phi;
		magnitude(channels[0], channels[1], mag);
		phase(channels[0], channels[1], phi);

		for (int i = 0; i <= mag.rows - 1; i++)
		{
			for (int j = 0; j <= mag.cols - 1; j++)
			{
				mag.at<float>(i, j) = log(mag.at<float>(i, j) + 1);
			}
		}

		normalize(mag, dst, 0, 255, NORM_MINMAX, CV_8UC1);

		// filtering operations
		for (int i = 0; i <= src.rows - 1; i++)
			for (int j = 0; j <= src.cols - 1; j++)
			{
				if (!((pow(src.rows / 2 - i, 2) + pow(src.cols / 2 - j, 2)) > pow(radius, 2)))
				{
					channels[0].at<float>(i, j) = 0;
					channels[1].at<float>(i, j) = 0;
				}
			}

		//perform the inverse transform
		Mat dest, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

		centering_transform(dstf);

		normalize(dstf, dest, 0, 255, NORM_MINMAX, CV_8UC1);

		imshow("init image", src);
		imshow("final image", dest);
	}
}

void median_filter()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));
		int dx[9] = { -1,-1,-1,0,0,0,1,1,1 };
		int dy[9] = { -1,0,1,-1,0,1,-1,0,1 };
		int kernel_dim = 3;
		int height = src.rows;
		int width = src.cols;
		int sum;

		for (int i = 1; i < src.rows - 1; i++)
		{
			for (int j = 1; j < src.cols - 1; j++)
			{
				// current pixel is (i,j)

				std::vector<int> val;
				for (int k = 0; k <= 8; k++)
				{
					val.push_back(src.at<uchar>(i + dy[k], j + dx[k]));
				}
				std::sort(val.begin(), val.end());

				dst.at<uchar>(i, j) = val[4];

			}
		}


		imshow("init image", src);
		imshow("final image", dst);
	}
}

Mat convolution(Mat src, Mat kernel, float m) {
	
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

					S += (1 / m) * src.at<byte>(i + ki - k, j + kj - k) * kernel.at<float>(ki, kj);
				}
			dst.at<float>(i, j) = S;
		}
	}
	dst.convertTo(dst, CV_8UC1);
	return dst;

}

void gaussian_filter_1x2() {

	Mat src, dst;
	char fname[MAX_PATH];
	float data[100];

	float sigma;
	printf("Introduce the sigma value : \n");
	scanf("%f", &sigma);

	int n;
	n = 6 * sigma;
	if (n % 2 == 0) n += 1;

	printf("The size of the kernel is: %d ",n );

	Mat kernel = Mat(n, n, CV_32F);
	float sum = 0;
	while (openFileDlg(fname)) {

		double t = (double)getTickCount();

		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		for (int ki = 0; ki < n; ki++) {
			for (int kj = 0; kj < n; kj++) {
				float intermed = -((ki - n / 2) * (ki - n / 2) + (kj - n / 2)*(kj - n / 2)) / (2 * sigma*sigma);
				kernel.at<float>(ki, kj) = (1 / (2 * PI*sigma*sigma)) * exp(intermed);
				sum += kernel.at<float>(ki, kj);
			}
		}

		dst = convolution(src, kernel, sum);

		t = ((double)getTickCount() - t) / getTickFrequency();

		printf("Time = %.3f [ms]\n", t*1000);
		
		imshow("src", src);
		imshow("dst", dst);

		waitKey();
	}

}

Mat apply_convolution(Mat *src, Mat *k, float sum) {
	Mat dst = Mat(src->rows, src->cols, CV_8UC1);
	int dim1 = k->rows;
	int dim2 = k->cols;
	for (int i = dim1 / 2; i <= src->rows - dim1 / 2 - 1; i++) {
		for (int j = dim2 / 2; j <= src->cols - dim2 / 2 - 1; j++) {
			int val = 0;
			for (int ki = -dim1 / 2; ki <= dim1 / 2; ki++) {
				for (int m = -dim2 / 2; m <= dim2 / 2; m++) {
					val += (src->at<uchar>(i + ki, j + m) * k->at<float>(ki + dim1 / 2, m + dim2 / 2));
				}
			}

			dst.at<uchar>(i, j) = (float)val / sum;
		}
	}
	return dst;
}

Mat apply_convolution_float(Mat *src, Mat *k, float sum) {
	Mat dst = Mat(src->rows, src->cols, CV_32FC1);
	int dim1 = k->rows;
	int dim2 = k->cols;
	for (int i = dim1 / 2; i <= src->rows - dim1 / 2 - 1; i++) {
		for (int j = dim2 / 2; j <= src->cols - dim2 / 2 - 1; j++) {
			int val = 0;
			for (int ki = -dim1 / 2; ki <= dim1 / 2; ki++) {
				for (int m = -dim2 / 2; m <= dim2 / 2; m++) {
					val += (src->at<uchar>(i + ki, j + m) * k->at<float>(ki + dim1 / 2, m + dim2 / 2));
				}
			}

			dst.at<float>(i, j) = (float)val / sum;
		}
	}
	return dst;
}


void gaussian_filter_2x1() {
	char fname[MAX_PATH];
	Mat src, dst, intermediate;
	
	float sigma;
	printf("Introduce the sigma value : \n");
	scanf("%f", &sigma);
	
	int n = sigma * 6;
	if (n % 2 == 0) n += 1;

	Mat kernel1 = Mat(n, 1, CV_32FC1);
	Mat kernel2 = Mat(1, n, CV_32FC1);
	
	int x0 = n / 2;
	int y0 = n / 2;

	while (openFileDlg(fname))
	{
		src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		dst = Mat(src.rows, src.cols, CV_8UC1, Scalar(255));

		double t = (double)getTickCount();

		float sumx = 0;
		float sumy = 0;

		for (int i = 0; i < kernel1.rows; i++)
		{
			float exponent = ((i - x0)*(i - x0)) / (2 * sigma*sigma);
			kernel1.at<float>(i, 0) = 1 / (2 * PI*sigma*sigma)*exp(-exponent);
			sumx += kernel1.at<float>(i, 0);
		}

		for (int i = 0; i < kernel2.cols; i++)
		{
			float exponent = ((i - y0)*(i - y0)) / (2 * sigma*sigma);
			kernel2.at<float>(0, i) = 1 / (2 * PI*sigma*sigma)*exp(-exponent);
			sumy += kernel2.at<float>(0, i);
		}

		dst = apply_convolution(&apply_convolution(&src, &kernel1, sumx), &kernel2, sumy);


		
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("Time = %.3f [ms]\n", t * 1000);
		
		imshow("src", src);
		imshow("dst", dst);
		
		waitKey();
	}
}

void gaussianLPF() {
	int r;
	printf("Enter the value of the radius: \n");
	scanf("%d", &r);

	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier, dst2;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);
		Mat mag, phi, dst;
		int height = fourier.rows;
		int width = fourier.cols;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				channels[0].at<float>(i, j) *= (exp(-(((height / 2 - i) * (height / 2 - i) + (width / 2 - j) * (width / 2 - j)) / (r*r))));
				channels[1].at<float>(i, j) *= (exp(-(((height / 2 - i) * (height / 2 - i) + (width / 2 - j) * (width / 2 - j)) / (r*r))));
			}
		}
		merge(channels, 2, fourier);
		dft(fourier, dst2, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dst2);
		normalize(dst2, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("input image", src);
		imshow("output image", dst);
		waitKey();
	}
}

void gaussianHPF() {

	int r;
	printf("Enter the value of the radius: \n");
	scanf("%d", &r);
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);
		Mat fourier, dst2;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);
		Mat mag, phi, dst;
		int height = fourier.rows;
		int width = fourier.cols;
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				channels[0].at<float>(i, j) *= (1 - exp(-(((height / 2 - i) * (height / 2 - i) + (width / 2 - j) * (width / 2 - j)) / (r*r))));
				channels[1].at<float>(i, j) *= (1 - exp(-(((height / 2 - i) * (height / 2 - i) + (width / 2 - j) * (width / 2 - j)) / (r*r))));
			}
		}
		merge(channels, 2, fourier);
		dft(fourier, dst2, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
		centering_transform(dst2);
		normalize(dst2, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		imshow("input image", src);
		imshow("output image", dst);
		waitKey();
	}
}


Mat apply_convolution_float(Mat *src, Mat *p) {
	Mat dst = Mat(src->rows, src->cols, CV_32FC1);
	for (int i = 1; i <src->rows-1; i++) {
		for (int j = 1; j < src->cols-1; j++) {
			float val = 0;
			for (int ki = -1; ki <= 1; ki++) {
				for (int m = -1; m <= 1; m++) {
					val += (src->at<uchar>(i + ki, j + m) * p->at<float>(ki+1 , m+1));
				}
			}
			dst.at<float>(i, j) = val ;
		}
	}
	return dst;
}


void canny_gradient() {
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat Gx, Gy;
		
		Mat Sx = (Mat_<float>(3,3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		Mat Sy = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	

		Gx = apply_convolution_float(&src,&Sx);
		Gy = apply_convolution_float(&src,&Sy);

		Mat G = Mat(src.rows, src.cols, CV_32FC1);
		Mat NG = Mat(src.rows, src.cols, CV_32FC1);
		Mat fi = Mat(src.rows, src.cols, CV_32FC1);

		for(int i=0;i<src.rows;i++)
			for (int j = 0; j < src.cols; j++) {
				G.at<float>(i, j) = sqrt(pow(Gx.at<float>(i,j),2) + pow(Gy.at<float>(i,j),2));
				NG.at<float>(i, j) = G.at<float>(i,j)/( 4 * sqrt(2)); //normalize
				fi.at<float>(i, j) = atan2(Gy.at<float>(i,j), Gx.at<float>(i,j));
			}

		Mat NG_show, fi_show;
		NG.convertTo(NG_show,CV_8UC1);
		fi.convertTo(fi_show, CV_8UC1);

		imshow("normalized gradient matrix",NG_show);
		waitKey();
	}
}

int determine_area(float fi) {

	if ((fi >= -PI / 8.f && fi <= PI / 8.f) || (fi <= -7.f * PI / 8 || fi >= 7.f * PI / 8))
		return 0;
	else if ((fi >= PI / 8.f && fi <= 3.f * PI / 8.f) || (fi <= -5.f * PI / 8.f && fi >= -7.f * PI / 8.f))
		return 1;
	else if ((fi >= 3.f * PI / 8.f && fi <= 5.f * PI / 8.f) || (fi <= -3.f * PI / 8.f && fi >= -5.f * PI / 8.f))
		return 2;
	else if ((fi >= 5.f * PI / 8.f && fi <= 7.f * PI / 8.f) || (fi >= -3.f * PI / 8.f && fi <= -PI / 8.f))
		return 3;

}

void canny_non_maxima() {
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat Gx, Gy;

		Mat Sx = (Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		Mat Sy = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);


		Gx = apply_convolution_float(&src, &Sx);
		Gy = apply_convolution_float(&src, &Sy);

		Mat G = Mat(src.rows, src.cols, CV_32FC1);
		Mat NG = Mat(src.rows, src.cols, CV_32FC1);
		Mat fi = Mat(src.rows, src.cols, CV_32FC1);

		for (int i = 0; i<src.rows; i++)
			for (int j = 0; j < src.cols; j++) {
				G.at<float>(i, j) = sqrt(pow(Gx.at<float>(i, j), 2) + pow(Gy.at<float>(i, j), 2));
				NG.at<float>(i, j) = G.at<float>(i, j) / (4 * sqrt(2)); //normalize
				fi.at<float>(i, j) = atan2(Gy.at<float>(i, j), Gx.at<float>(i, j));  // Gy /Gx
			}

		Mat NG_show;
		NG.convertTo(NG_show, CV_8UC1);

		imshow("normalized gradient matrix", NG_show);

		//non -maxima

		Mat newNG = NG.clone();

		for (int i = 1; i<NG.rows - 1; i++)
		{
			for (int j = 1; j<NG.cols - 1; j++)
			{
				float angle = fi.at<float>(i, j);
				int area = determine_area(angle);

				float pixel = abs(NG.at<float>(i, j));

				switch (area) {
				case 0:
					if (pixel >= abs(NG.at<float>(i, j - 1)) && pixel >= abs(NG.at<float>(i, j + 1)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				case 1:
					if (pixel >= abs(NG.at<float>(i - 1, j + 1)) && pixel >= abs(NG.at<float>(i + 1, j - 1)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				case 2:
					if (pixel >= abs(NG.at<float>(i + 1, j)) && pixel >= abs(NG.at<float>(i - 1, j)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				case 3:
					if (pixel >= abs(NG.at<float>(i - 1, j - 1)) && pixel >= abs(NG.at<float>(i + 1, j + 1)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				}
			}
		}

		Mat newNG_show;
		newNG.convertTo(newNG_show, CV_8UC1);

		imshow("non_maxima", newNG_show);
		waitKey();
	}
}

void canny_adative_tresholding() {
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat Gx, Gy;

		Mat Sx = (Mat_<float>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
		Mat Sy = (Mat_<float>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);


		Gx = apply_convolution_float(&src, &Sx);
		Gy = apply_convolution_float(&src, &Sy);

		Mat G = Mat(src.rows, src.cols, CV_32FC1);
		Mat NG = Mat(src.rows, src.cols, CV_32FC1);
		Mat fi = Mat(src.rows, src.cols, CV_32FC1);

		for (int i = 0; i<src.rows; i++)
			for (int j = 0; j < src.cols; j++) {
				G.at<float>(i, j) = sqrt(pow(Gx.at<float>(i, j), 2) + pow(Gy.at<float>(i, j), 2));
				NG.at<float>(i, j) = G.at<float>(i, j) / (4 * sqrt(2)); //normalize
				fi.at<float>(i, j) = atan2(Gy.at<float>(i, j), Gx.at<float>(i, j));  // Gy /Gx
			}

		Mat NG_show;
		NG.convertTo(NG_show, CV_8UC1);

		imshow("normalized gradient matrix", NG_show);

		//non -maxima

		Mat newNG = NG.clone();

		for (int i = 1; i<NG.rows - 1; i++)
		{
			for (int j = 1; j<NG.cols - 1; j++)
			{
				float angle = fi.at<float>(i, j);
				int area = determine_area(angle);

				float pixel = abs(NG.at<float>(i, j));

				switch (area) {
				case 0:
					if (pixel >= abs(NG.at<float>(i, j - 1)) && pixel >= abs(NG.at<float>(i, j + 1)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				case 1:
					if (pixel >= abs(NG.at<float>(i - 1, j + 1)) && pixel >= abs(NG.at<float>(i + 1, j - 1)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				case 2:
					if (pixel >= abs(NG.at<float>(i + 1, j)) && pixel >= abs(NG.at<float>(i - 1, j)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				case 3:
					if (pixel >= abs(NG.at<float>(i - 1, j - 1)) && pixel >= abs(NG.at<float>(i + 1, j + 1)))
						newNG.at<float>(i, j) = pixel;
					else
						newNG.at<float>(i, j) = 0;
					break;
				}
			}
		}

		Mat newNG_show;
		newNG.convertTo(newNG_show, CV_8UC1);

		imshow("non_maxima", newNG_show);

		// compute histogram

		int v[256];
		
		for (int k = 0; k < 256; k++)
			v[k] = 0;

			for (int i = 1; i < src.rows-1; i++) {
				for (int j = 1; j < src.cols-1; j++) {
					v[newNG_show.at<uchar>(i, j)] ++;
				}
			}

		showHistogram("histogram", v, 256, 256);

		int noEdgePixels;
		int noNonZeroGradientPixels;
		float procent = 0.1f;

		noNonZeroGradientPixels = (newNG_show.rows - 2) * (newNG_show.cols - 2) - v[0];
		noEdgePixels = procent * noNonZeroGradientPixels;

		int ok = 0;
		int computed_sum = 0;
		int treshold_high;
		for (int i = 255; i >= 0 && ok == 0; i--) {
			if (computed_sum < noEdgePixels)
				computed_sum += v[i];
			else {
				ok = 1;
				treshold_high = i;
			}
		}

		int treshold_low;
		float k_procent = 0.4f;
		treshold_low = k_procent * treshold_high;
		
		printf("treshold high %d", treshold_high);

		// binarizare

		for (int i = 1; i < newNG_show.rows - 1; i++) {
			for (int j = 1; j < newNG_show.cols - 1; j++)
			{
				if (newNG_show.at<uchar>(i, j) >= treshold_high) {
				    newNG_show.at<uchar>(i, j) = 255;
			    }
				else if (newNG_show.at<uchar>(i, j) < treshold_high && newNG_show.at<uchar>(i, j) > treshold_low) {
					newNG_show.at<uchar>(i, j) = 128;
				}
				else if (newNG_show.at<uchar>(i, j) = 255 <= treshold_low) {
					newNG_show.at<uchar>(i, j) = 0;
				}
			}
		}
			
		// bfs algorithm


		int i_index[8] = { -1, 0, 1, 0, -1, -1, 1, 1 };
		int j_index[8] = { 0, -1, 0, 1, -1, 1, -1, 1 };
		uchar neighbor[8];

		for (int i = 1; i < newNG_show.rows- 1; i++) {
			for (int j = 1; j < newNG_show.cols - 1; j++) {
				if (newNG_show.at<uchar>(i, j) == 255) {
					std::queue<Point2i> Q;
					Q.push(Point2i(i, j));
					while (!Q.empty())
					{
						Point2i q = Q.front();
						Q.pop();

						// check neighbours
						for (int k = 0; k < 8; k++)
							neighbor[k] = newNG_show.at<uchar>(q.x + i_index[k], q.y + j_index[k]);
					
						for (int k = 0; k < 8; k++) {
							if (neighbor[k] == 128)
							{
								newNG_show.at<uchar>(q.x + i_index[k], q.y + j_index[k]) = 255;
								Q.push(Point2i(q.x + i_index[k], q.y + j_index[k]));

   						  }
						}
					}
				}

			}
		}

	    // change weak points non-edge points
		for (int i = 1; i < newNG_show.rows - 1; i++) {
			for (int j = 1; j < newNG_show.cols - 1; j++) {
				if (newNG_show.at<uchar>(i,j) == 128) {
					newNG_show.at<uchar>(i, j) = 0;
				}
			}
		}

		imshow("final Canny", newNG_show);
		waitKey();
	}

}




int main()
	{
		int op;
		do
		{
			system("cls");
			destroyAllWindows();
			printf("Menu:\n");
			printf(" 1 - Open image\n");
			printf(" 2 - Open BMP images from folder\n");
			printf(" 3 - Image negative - diblook style\n");
			printf(" 4 - BGR->HSV\n");
			printf(" 5 - Resize image\n");
			printf(" 6 - Canny edge detection\n");
			printf(" 7 - Edges in a video sequence\n");
			printf(" 8 - Snap frame from live video\n");
			printf(" 9 - Mouse callback demo\n");
			printf(" 10 - L1 - Additive factor(grayscale images)\n");
			printf(" 11 - L1 - Multiplicative factor(grayscale images)\n");
			printf(" 12 - L1 - Create color image\n");
			printf(" 13 - L1 - Inverse matrix\n");
			printf(" 14 - L2 - RGB channels\n");
			printf(" 15 - L2 - Conevrsion color - grayscale\n");
			printf(" 16 - L2 - Conversion Grayscale - Black and White\n");
			printf(" 17 - L2 - RGB to HSV\n");
			printf(" 18 - L4 - Geometrical Features. \n");
			printf(" 19 - L5 - Labelling (BFS) \n");
			printf(" 20 - L5 - Labelling (equivalence classes) \n");
			printf(" 21 - L6 - Border tracing algorithm \n");
			printf(" 22 - L6 - Border recunstruction \n");
			printf(" 23 - L7 - Dilation \n");
			printf(" 24 - L7 - Erosion \n");
			printf(" 25 - L7 - Opening \n" );
			printf(" 26 - L7 - Closure \n");
			printf(" 27 - L7 - Boundary Extraction \n");
			printf(" 28 - L7 - Region Filling \n");
			printf(" 29 - L8 - Histogram, mean and deviation  \n");
			printf(" 30 - L8 - Basic Global tresholding algorithm  \n");
			printf(" 31 - L8 - Brightness Change \n");
			printf(" 32 - L8 - Contrast Change \n");
			printf(" 33 - L8 - Gamma Correction \n");
			printf(" 34 - L8 - Histogram Equation \n");
			printf(" 35 - L9 - Mean filter (3x3) \n");
			printf(" 36 - L9 - Mean filter (5x5) \n");
			printf(" 37 - L9 - Gausian filter \n");
			printf(" 38 - L9 - Laplace filter \n");
			printf(" 39 - L9 - High-pass filter \n");
			printf(" 40 - L9 - Magnitude lag of the centered Fourier spectrum \n");
			printf(" 41 - L9 - Ideal low-pass filter \n");
			printf(" 42 - L9 - Ideal high-pass filter \n");
			printf(" 43 - L9 - Gaussian-cut LPF \n");
			printf(" 44 - L9 - Gaussian-cut HPF \n");
			printf(" 45 - L10 - Median filetr (omega variable) \n");
			printf(" 46 - L10 - Gaussian filter 1x2D (sigma variable) \n");
			printf(" 47 - L10 - Gaussian filter 2x1D (sigma variable) \n");
			printf(" 48 - L11 - Canny gradient \n");
			printf(" 49 - L11 - Canny non-maxima \n");
			printf(" 50 - L11 - Canny adative_tresholding \ n");
			printf(" 0 - Exit\n\n");
			printf("Option: ");
			scanf("%d", &op);
			switch (op)
			{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testParcurgereSimplaDiblookStyle(); 
				break;
			case 4:
				//testColor2Gray();
				testBGR2HSV();
				break;
			case 5:
				testResize();
				break;
			case 6:
				testCanny();
				break;
			case 7:
				testVideoSequence();
				break;
			case 8:
				testSnap();
				break;
			case 9:
				testMouseClick();
				break;
			case 10:
				L1AdditiveFactor();
				break;
			case 11:
				L1MultiplicativeFactor();
				break;
			case 12:
				L1CreateColorImage();
				break;
			case 13:
				L1Inverse();
				break;
			case 14:
				rgb_channels();
				break;
			case 15:
				color_to_grayscale();
				break;
			case 16:
				grayscale_to_black_white();
				break;
			case 17:
				rgb_to_hsv();
				break;
			case 18:
				testGeometricalFeatures();
				break;
			case 19:
				bfs_labelling();
				break;
			case 20:
				equivalence_classes_labelling();
				break;
			case 21:
				border_tracing();
				break;
			case 22:
				border_reconstruction();
				break;
			case 23:
				dilation(n);
				break;
			case 24:	
				erosion(n);
				break;
			case 25:
				opening();
				break;
			case 26:
				closure();
				break;
			case 27:
				boundary_extraction();
				break;
			case 28:
				region_filling();
				break;
			case 29:
				compute_histogram();
				break;
			case 30:
				tresholding_algorithm();
				break;
			case 31:
				brightness_change();
				break;
			case 32:
				contrast_change();
				break;
			case 33: 
				gamma_correction();
				break;
			case 34:
				histogram_equalization();
				break;
			case 35:
				mean_filter_3();
				break;
			case 36:
				mean_filter_5();
				break;
			case 37:
				gaussian_filter();
				break;
			case 38:
				laplace_filter();
				break;
			case 39:
				high_pass_filter();
				break;
			case 40:
				magnitude_lag();
				break;
			case 41:
				ideal_low_pass_filter();
				break;
			case 42:
				ideal_high_pass_filter();
				break;
			case 43:
				gaussianLPF();
				break;
			case 44:
				gaussianHPF();
				break;
			case 45:
				median_filter();
				break;
			case 46:
				gaussian_filter_1x2();
				break;
			case 47:
				gaussian_filter_2x1();
				break;
			case 48: 
				canny_gradient();
				break;
			case 49:
				canny_non_maxima();
				break;
			case 50:
				canny_adative_tresholding();
				break;

			}
		} while (op != 0);
		return 0;
	}