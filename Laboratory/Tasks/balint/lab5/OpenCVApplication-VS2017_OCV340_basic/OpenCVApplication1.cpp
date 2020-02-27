// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

typedef struct _border
{
	vector<Point2i> borderPoints;
	vector<int> chainCodes;
}BORDER;

typedef struct _reconstruct
{
	Point2i startPoint;
	vector<int> chainCode;
}RECONSTRUCT;

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

Point2i GetTraceStart(Mat *img)
{
	int h = img->rows, w = img->cols;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (img->at<uchar>(i, j) == 0)
			{
				cout << "i=" << i << endl;
				cout << "j=" << j << endl;
				return Point2i(j, i);
			}
		}
	}
	return Point2i(-1, -1);
}

Point2i dirPixel(Point2i start, int dir)
{
	switch (dir)
	{
	case 0: return Point2i(start.x + 1, start.y);
	case 1: return Point2i(start.x + 1, start.y - 1);
	case 2: return Point2i(start.x, start.y - 1);
	case 3: return Point2i(start.x - 1, start.y - 1);
	case 4: return Point2i(start.x - 1, start.y);
	case 5: return Point2i(start.x - 1, start.y + 1);
	case 6: return Point2i(start.x, start.y + 1);
	case 7: return Point2i(start.x + 1, start.y + 1);
	default: return Point2i(-1, -1);
	}
}

BORDER BorderTracing(Mat *img, Point2i startPoint)
{
	BORDER border;
	
	int h = img->rows, w = img->cols;
	int dir = 7;
	Point2i nextPoint(-1, -1);
	Point2i prevPoint = startPoint;

	border.borderPoints.push_back(startPoint);

	do
	{
		if (dir % 2 == 0)
		{
			dir = (dir + 7) % 8;
		}
		else
		{
			dir = (dir + 6) % 8;
		}
		nextPoint = dirPixel(prevPoint, dir);
		while (img->at<uchar>(nextPoint.y, nextPoint.x) != 0)
		{
			dir = (dir + 1) % 8;
			nextPoint = dirPixel(prevPoint, dir);
		}
		border.borderPoints.push_back(nextPoint);
		border.chainCodes.push_back(dir);
		prevPoint = nextPoint;
	} while (border.borderPoints.size() < 4 ||
		border.borderPoints.at(0) != border.borderPoints.at(border.borderPoints.size() - 2) ||
		border.borderPoints.at(1) != border.borderPoints.at(border.borderPoints.size() - 1));

	// Removing 2 leftover points.
	border.borderPoints.pop_back();
	border.borderPoints.pop_back();
	border.chainCodes.pop_back();
	border.chainCodes.pop_back();

	return border;
}

vector<int> chainCodeToDerivativeChainCode(vector<int> chainCode)
{
	vector<int> derivative;
	size_t chainSize = chainCode.size();

	for (size_t i = 1; i < chainSize; i++)
	{
		int derivPoint = (chainCode.at(i) - chainCode.at(i - 1) + 8) % 8;
		derivative.push_back(derivPoint);
	}

	return derivative;
}

Mat ReconstructBorder(Mat *src, RECONSTRUCT *recon, uchar borderCol)
{
	Mat img = Mat(src->rows, src->cols, CV_8UC1);
	img = src->clone();
	Point2i prevPoint = recon->startPoint;
	img.at<uchar>(prevPoint.y, prevPoint.x) = borderCol;

	for (auto i : recon->chainCode)
	{
		Point2i nextPoint = dirPixel(prevPoint, i);
		img.at<uchar>(nextPoint.y, nextPoint.x) = borderCol;
		prevPoint = nextPoint;
	}
	return img;
}

void borderRunner()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		Point2i start = GetTraceStart(&img);
		cout << start.x << ',' << start.y << endl;

		BORDER border = BorderTracing(&img, start);

		cout << border.chainCodes.size() << endl;
		for (auto i : border.chainCodes)
			cout << i << ", ";
		cout << endl;
		vector<int> derivative = chainCodeToDerivativeChainCode(border.chainCodes);
		for (auto i : derivative)
			cout << i << ", ";

		RECONSTRUCT recon = { start, border.chainCodes };

		Mat overlay = Mat::zeros(img.rows, img.cols, CV_8UC1);
		Mat dst = ReconstructBorder(&overlay, &recon, 255);

		imshow("img", img);
		imshow("dst", dst);
		waitKey(0);
	}
}

RECONSTRUCT readFromFile(char *fname)
{
	RECONSTRUCT recon;

	ifstream stream(fname, ios::in);

	stream >> recon.startPoint.x;
	stream >> recon.startPoint.y;

	int n = 0, rd;
	stream >> n;
	for (int i = 0; i < n; i++)
	{
		stream >> rd;
		recon.chainCode.push_back(rd);
	}

	return recon;
}

void fromFile()
{
	char fname[MAX_PATH];
	char fname2[MAX_PATH];
	while (openFileDlg(fname))
	{
		 RECONSTRUCT recon = readFromFile(fname);
		 
		 openFileDlg(fname2);
		 Mat backg = imread(fname2, CV_LOAD_IMAGE_GRAYSCALE);
		 Mat dst = ReconstructBorder(&backg, &recon, 0);

		 imshow("dst", dst);
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
		printf(" 0 - Exit\n\n");
		printf(" 1 - Borders\n\n");
		printf(" 2 - Reconstruct\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				borderRunner();
				break;
			case 2:
				fromFile();
				break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}