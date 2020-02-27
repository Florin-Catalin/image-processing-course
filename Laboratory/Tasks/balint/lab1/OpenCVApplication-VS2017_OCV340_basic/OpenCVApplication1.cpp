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
		imshow("hsv image", hsvImg);

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
		imshow("gauss", gauss);
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

void negativeImage() {
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		double t = (double) getTickCount();
		for (int i = 0; i < img.rows; i++) 
		{
			for (int j = 0; j < img.cols; j++) 
			{
				img.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);
			}
		}
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("time: %.3f [ms]\n", t * 1000);
		imshow("negative image", img);
		waitKey(0);
	}
	
}

int openNewFile(char *fname)
{
	char filename[MAX_PATH];
	OPENFILENAME ofn;

	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	//ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFilter = "Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrTitle = "Select a File, yo!";
	ofn.Flags = OFN_DONTADDTORECENT;
	if (GetOpenFileName(&ofn))
	{
		std::cout << "You chose the file \"" << filename << "\"\n";
	}
	else
	{
		// All this stuff below is to tell you exactly how you messed up above. 
		// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
		switch (CommDlgExtendedError())
		{
		case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
		case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
		case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
		case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
		case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
		case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
		case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
		case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
		case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
		case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
		case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
		case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
		case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
		case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
		case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
		default: std::cout << "You cancelled.\n";
		}
	}
	strcpy(fname, ofn.lpstrFile);
	return strcmp(fname, "");
}

void writeToNewFile(Mat *img)
{
	char fname[MAX_PATH];
	openNewFile(fname);
	imwrite(fname, *img);
}

void changeGrayscaleAdditiveFactor() 
{
	const uchar GREY_ADD = 100;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;

		int step = (int)src.step;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int val = GREY_ADD + lpSrc[i * step + j];
				val = (val > 255) ? 255 : val;
				val = (val < 0) ? 0 : val;
				lpDst[i * step + j] = val;
			}
		}
		imshow("original", src);
		imshow("added grey", dst);
		waitKey(0);
	}
}

void changeGrayscaleMultiplicativeFactor() 
{
	const double GREY_MULT = 1.5;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = src.rows;
		int width = src.cols;
		Mat dst = src.clone();

		uchar *lpSrc = src.data;
		uchar *lpDst = dst.data;

		int step = (int)src.step;

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				int val = (int)(GREY_MULT * lpSrc[i * step + j]);
				val = (val > 255) ? 255 : val;
				val = (val < 0) ? 0 : val;
				lpDst[i * step + j] = val;
			}
		}
		//printf("%d\n", imwrite("d:/univ/code/y3se2/ip/labs/lab1/OpenCVApplication-VS2017_OCV340_basic/Images/greyscale.bmp", dst));
		imshow("original", src);
		imshow("mult grey", dst);
		writeToNewFile(&dst);
		waitKey(0);
	}
}

void fillImage(Mat *img, int fromI, int toI, int fromJ, int toJ, Vec3b bgr)
{
	for (int i = fromI; i < toI; i++)
	{
		for (int j = fromJ; j < toJ; j++)
		{
			img->at<Vec3b>(i, j) = bgr;
		}
	}
}

void newImageInFourParts()
{
	int height = 256;
	int width = 256;
	int halfheight = height / 2;
	int halfwidth = width / 2;
	Vec3b white(255, 255, 255);
	Vec3b red(0, 0, 255);
	Vec3b green(0, 255, 0);
	Vec3b yellow(0, 255, 255);

	Mat_<Vec3b> img(height, width, CV_8UC3);
	
	fillImage(&img, 0, halfheight, 0, halfwidth, white);
	fillImage(&img, halfheight, height, 0, halfwidth, red);
	fillImage(&img, 0, halfheight, halfwidth, width, green);
	fillImage(&img, halfheight, height, halfwidth, width, yellow);

	imshow("created", img);
	waitKey(0);
}

void matrixInverse()
{
	float vals[9] = { 1.0, 2.5, 3.6,
					8.5, 9.34, 89.315,
					9.3, 1.005, 0.3 };
	Mat M(3, 3, CV_32FC1, vals);
	std::cout << M << std::endl;
	Mat MI = M.inv();
	std::cout << MI << std::endl;
}

void horizontalFlipGrayscale()
{
	char fname[MAX_PATH];
	int height = 0, width = 0, halfheight = 0;

	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		height = img.rows;
		halfheight = height / 2;
		width = img.cols;
		Mat dst = Mat(height, width, CV_8UC1);

		uchar *lpSrc = img.data;
		uchar *lpDst = dst.data;
		int step = (int)img.step;
		for (int j = 0; j < width; j++)
		{
			for (int i = 0; i < halfheight; i++)
			{
				lpDst[i * step + j] = lpSrc[(height - i - 1) * step + j];
				lpDst[(height - i - 1) * step + j] = lpSrc[i * step + j];
			}
			lpDst[halfheight * step + j] = lpSrc[halfheight * step + j];
		}
		imshow("original", img);
		imshow("horizontal flip", dst);
		waitKey(0);
	}
}

/*
void horizontalFlip()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		Mat dst = Mat(img.rows, img.cols, CV_8UC3);
		flip(img, dst, 0);
		imshow("original", img);
		imshow("flipped", dst);
		waitKey(0);
	}
}
*/

void verticalFlipColor()
{
	char fname[MAX_PATH];
	int height = 0, width = 0, halfwidth = 0;

	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_COLOR);
		height = img.rows;
		width = img.cols;
		halfwidth = width / 2;
		Mat dst = Mat(height, width, CV_8UC3);

		//Vec3b *lpSrc = (Vec3b*) img.data;
		uchar *lpSrc = img.data;
		//Vec3b *lpDst = (Vec3b*) dst.data;
		uchar *lpDst = dst.data;
		int step = (int)img.step[0];
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < halfwidth; j++)
			{
				//lpDst[i * step + j] = lpSrc[i * step + (width - j - 1)];
				//lpDst[i * step + (width - j - 1)] = lpSrc[i * step + j];
				dst.at<Vec3b>(i, j) = img.at<Vec3b>(i, width - j - 1);
				dst.at<Vec3b>(i, width - j - 1) = img.at<Vec3b>(i, j);
			}
			//lpDst[i * step + halfwidth] = lpSrc[i * step + halfwidth];
			dst.at<Vec3b>(i, halfwidth) = img.at<Vec3b>(i, halfwidth);
		}
		imshow("original", img);
		imshow("horizontal flip", dst);
		waitKey(0);
	}
}

void centerCrop()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_UNCHANGED);
		int halfcols = img.cols / 2;
		int halfrows = img.rows / 2;
		Rect cropRect(halfcols - halfcols / 2, halfrows - halfrows / 2, halfcols, halfrows);
		Mat dst(img, cropRect);
		
		imshow("original", img);
		imshow("cropped", dst);
		waitKey(0);
	}
}

void imageResize()
{
	const int NEW_HEIGHT = 1000;
	const int NEW_WIDTH = 1000;
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<Vec3b> img = imread(fname, CV_LOAD_IMAGE_COLOR);
		//Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int height = img.rows;
		int width = img.cols;
		float heightRatio = (float)height / NEW_HEIGHT;
		float widthRatio = (float)width / NEW_WIDTH;
		Mat dst = Mat(NEW_HEIGHT, NEW_WIDTH, CV_8UC3);
		//Mat dst = Mat(NEW_HEIGHT, NEW_WIDTH, CV_8UC1);

		for (int i = 0; i < NEW_HEIGHT; i++)
		{
			for (int j = 0; j < NEW_WIDTH; j++)
			{
				//dst.at<uchar>(i, j) = img.at<uchar>((int) i * heightRatio, (int) j * widthRatio);
				dst.at<Vec3b>(i, j) = img.at<Vec3b>((int) i * heightRatio, (int) j * widthRatio);
			}
		}
		imshow("original", img);
		imshow("resized", dst);
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
		printf(" 1 - Open image\n");
		printf(" 2 - Open BMP images from folder\n");
		printf(" 3 - Image negative - diblook style\n");
		printf(" 4 - BGR->HSV\n");
		printf(" 5 - Resize image\n");
		printf(" 6 - Canny edge detection\n");
		printf(" 7 - Edges in a video sequence\n");
		printf(" 8 - Snap frame from live video\n");
		printf(" 9 - Mouse callback demo\n");
		printf(" 10 - Negative image\n");
		printf(" 11 - Grayscale additive factor\n");
		printf(" 12 - Grayscale multiplicative factor\n");
		printf(" 13 - Color image broken into 4 parts\n");
		printf(" 14 - Matrix inverse\n");
		printf(" 15 - Horizontal flip\n");
		printf(" 16 - Vertical flip\n");
		printf(" 17 - Center crop\n");
		printf(" 18 - Resize image\n");
		printf(" 0 - Exit\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				testOpenImage();
				break;
			case 2:
				testOpenImagesFld();
				break;
			case 3:
				testParcurgereSimplaDiblookStyle(); //diblook style
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
				negativeImage();
				break;
			case 11:
				changeGrayscaleAdditiveFactor();
				break;
			case 12:
				changeGrayscaleMultiplicativeFactor();
				break;
			case 13:
				newImageInFourParts();
				break;
			case 14:
				matrixInverse();
				break;
			case 15:
				horizontalFlipGrayscale();
				break;
			case 16:
				verticalFlipColor();
				break;
			case 17:
				centerCrop();
				break;
			case 18:
				imageResize();
				break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}