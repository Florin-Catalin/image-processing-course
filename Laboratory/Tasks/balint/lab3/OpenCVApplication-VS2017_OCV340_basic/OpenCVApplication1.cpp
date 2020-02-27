// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

#define MY_WINDOW_NAME "MyWindow"

int op;

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

unsigned int calculateArea(Vec3b color, Mat *src)
{
	unsigned int area = 0;
	int h = src->rows, w = src->cols;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (color == src->at<Vec3b>(i, j)) area++;
		}
	}

	return area;
}

Point calculateCenterOfMass(unsigned int area, Vec3b color, Mat *src)
{
	int r = 0, c = 0;
	int h = src->rows, w = src->cols;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (color == src->at<Vec3b>(i, j))
			{
				r += i;
				c += j;
			}
		}
	}

	return Point((int)(r / area), (int)(c / area));
}

float calculateAngleOfElongation(Point centerMass, Vec3b color, Mat *src)
{
	float nominator = 0, denominator1 = 0, denominator2 = 0, fi = 0, degree = 0;
	int h = src->rows, w = src->cols;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (color == src->at<Vec3b>(i, j))
			{
				float rsub = i - centerMass.x, csub = j - centerMass.y;
				nominator += rsub * csub;
				denominator1 += csub * csub;
				denominator2 += rsub * rsub;
			}
		}
	}

	fi = atan2(2 * nominator, denominator1 - denominator2) / 2;
	return fi;
}

bool isOnPerimeter(Point point, Point minxy, Point maxxy, Vec3b pointColor, Mat *src)
{
	if (minxy.y < point.y)
	{
		for (int ix = point.x - 1; ix <= point.x + 1; ix++)
		{
			if (minxy.x < ix && maxxy.x > ix && src->at<Vec3b>(ix, point.y - 1) != pointColor) return true;
		}
	}
	if (maxxy.x > point.x)
	{
		for (int iy = point.y - 1; iy <= point.y + 1; iy++)
		{
			if (minxy.y < iy && maxxy.y > iy && src->at<Vec3b>(point.x + 1, iy) != pointColor) return true;
		}
	}
	if (maxxy.y > point.y)
	{
		for (int ix = point.x - 1; ix <= point.x + 1; ix++)
		{
			if (minxy.x < ix && maxxy.x > ix && src->at<Vec3b>(ix, point.y + 1) != pointColor) return true;
		}
	}
	if (minxy.x < point.x)
	{
		for (int iy = point.y - 1; iy <= point.y + 1; iy++)
		{
			if (minxy.y < iy && maxxy.y > iy && src->at<Vec3b>(point.x - 1, iy) != pointColor) return true;
		}
	}
	return false;
}

std::vector<Point> getContourPoints(Vec3b color, Mat *src, unsigned int *perimeter)
{
	int h = src->rows, w = src->cols;
	std::vector<Point> contourPoints;

	*perimeter = 0;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (color == src->at<Vec3b>(i, j) &&
				isOnPerimeter(Point(i, j), Point(0, 0), Point(h, w), color, src))
			{
				(*perimeter)++;
				contourPoints.push_back(Point(i, j));
			}
		}
	}
	*perimeter = (*perimeter) * CV_PI / 4;
	return contourPoints;
}

float calculateThinness(unsigned int area, unsigned int perimeter)
{
	return 4 * CV_PI * area / (perimeter * perimeter);
}

float calculateAspectRatio(Vec3b color, Mat *src)
{
	int h = src->rows, w = src->cols, cmin = INT_MAX, rmin = INT_MAX, cmax = INT_MIN, rmax = INT_MIN;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (color == src->at<Vec3b>(i, j))
			{
				cmin = (j < cmin) ? j : cmin;
				rmin = (i < rmin) ? i : rmin;
				cmax = (j > cmax) ? j : cmax;
				rmax = (i > rmax) ? i : rmax;
			}
		}
	}

	return (cmax - cmin + 1) / (float)(rmax - rmin + 1);
}

void drawContour(std::vector<Point> contour, Mat *src)
{
	Mat img = src->clone();
	int h = img.rows, w = img.cols;
	Vec3b black(0, 0, 0);

	for (int i = 0; i < contour.size(); i++)
	{
		img.at<Vec3b>(contour.at(i).x, contour.at(i).y) = black;
	}
	imshow("contour", img);
}

void drawCenterOfMass(Point centerOfMass, Mat *src)
{
	Mat img = src->clone();
	int h = img.rows, w = img.cols;
	Vec3b black(0, 0, 0);

	img.at<Vec3b>(centerOfMass.x, centerOfMass.y) = black;

	imshow("center of mass", img);
}

void drawAxisOfElongation(float axisOfElongation, Point centerMass, Mat *src)
{
	Mat img = src->clone();
	int h = img.rows, w = img.cols;
	int stepsize = 100;
	int xstart = centerMass.x - stepsize;
	int xend = centerMass.x + stepsize;

	Point start(axisOfElongation * (xstart - centerMass.x) + centerMass.y, xstart);
	Point end(axisOfElongation * (xend - centerMass.x) + centerMass.y, xend);

	line(img, start, end, Vec3b(0, 0, 0), 10);
	imshow("axis of elongation", img);
}

void calculateDetails(int x, int y, Mat *src)
{
	Vec3b color = (*src).at<Vec3b>(y, x);
	unsigned int area = calculateArea(color, src);
	printf("Area:\t\t\t%lu\n", area);
	Point centerMass = calculateCenterOfMass(area, color, src);
	std::cout << "Center of mass " << centerMass << std::endl;
	float fi = calculateAngleOfElongation(centerMass, color, src);
	float degree = fi * 180 / CV_PI;
	degree = (degree > 0) ? degree : degree + 180;
	std::cout << "Angle of elongation " << degree << std::endl;
	unsigned int perimeter = 0;
	std::vector<Point> contourPoints = getContourPoints(color, src, &perimeter);
	std::cout << "Perimeter " << perimeter << std::endl;
	float thinness = calculateThinness(area, perimeter);
	std::cout << "Thinness " << thinness << std::endl;
	float aspectRatio = calculateAspectRatio(color, src);
	std::cout << "Aspect ratio " << aspectRatio << std::endl;
	drawAxisOfElongation(fi, centerMass, src);
	drawContour(contourPoints, src);
	drawCenterOfMass(centerMass, src);
	waitKey(0);
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
			calculateDetails(x, y, src);
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

void getImageParams()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		namedWindow(MY_WINDOW_NAME, 1);
		setMouseCallback(MY_WINDOW_NAME, MyCallBackFunc, &src);

		imshow(MY_WINDOW_NAME, src);

		waitKey(0);
	}
}

int main()
{
	Mat src;
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 0 - Exit\n\n");
		printf(" 1 - Parameters\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
			case 2:
				getImageParams();
				break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}