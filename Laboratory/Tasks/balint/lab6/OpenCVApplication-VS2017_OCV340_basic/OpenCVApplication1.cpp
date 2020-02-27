// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

#pragma region Utils
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
#pragma endregion

#pragma region Constants
int di[8] = {1, 1, 0, -1, -1, -1, 0, 1};
int dj[8] = {0, -1, -1, -1, 0, 1, 1, 1};

int di4[4] = {1, 0, -1, 0};
int dj4[4] = {0, -1, 0, 1};

#pragma endregion

#pragma region Ops
Mat Dilate(Mat *img)
{
	int h = img->rows, w = img->cols;
	Mat dst(h, w, CV_8UC1);
	dst = img->clone();

	for (int i = 1; i < h-1; i++)
	{
		for (int j = 1; j < w-1; j++)
		{
			if (img->at<uchar>(i, j) == 0)
			{
				for (int k = 0; k < 8; k++)
				{
					int ki = i + di[k];
					int kj = j + dj[k];
					dst.at<uchar>(ki, kj) = 0;
				}
			}
		}
	}

	return dst;
}

Mat Dilate4(Mat *img)
{
	int h = img->rows, w = img->cols;
	Mat dst(h, w, CV_8UC1);
	dst = img->clone();

	for (int i = 1; i < h-1; i++)
	{
		for (int j = 1; j < w-1; j++)
		{
			if (img->at<uchar>(i, j) == 0)
			{
				for (int k = 0; k < 4; k++)
				{
					int ki = i + di4[k];
					int kj = j + dj4[k];
					dst.at<uchar>(ki, kj) = 0;
				}
			}
		}
	}

	return dst;
}

Mat Erode(Mat *img)
{
	int h = img->rows, w = img->cols;
	Mat dst(h, w, CV_8UC1);
	dst = img->clone();

	for (int i = 1; i < h-1; i++)
	{
		for (int j = 1; j < w-1; j++)
		{
			if (img->at<uchar>(i, j) == 0)
			{
				for (int k = 0; k < 8; k++)
				{
					int ki = i + di[k];
					int kj = j + dj[k];
					if(img->at<uchar>(ki, kj) == 255)
						dst.at<uchar>(i, j) = 255;
				}
			}
		}
	}

	return dst;
}

Mat Opening(Mat *img)
{
	return Dilate(&Erode(img));
}

Mat Closing(Mat *img)
{
	return Erode(&Dilate(img));
}

Mat And(Mat *a, Mat *b)
{
	int h = (a->rows > b->rows) ? a->rows : b->rows, w = (a->cols > b->cols) ? a->cols : b->cols;
	Mat dst(h, w, CV_8UC1);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = (a->at<uchar>(i, j) == 0 &&
				b->at<uchar>(i, j) == 0) ? 0 : 255;
		}
	}

	return dst;
}

Mat Or(Mat *a, Mat *b)
{
	int h = (a->rows > b->rows) ? a->rows : b->rows, w = (a->cols > b->cols) ? a->cols : b->cols;
	Mat dst(h, w, CV_8UC1);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = (a->at<uchar>(i, j) == 0 ||
				b->at<uchar>(i, j) == 0) ? 0 : 255;
		}
	}

	return dst;
}

Mat Not(Mat *a)
{
	int h = a->rows, w = a->cols;
	Mat dst(h, w, CV_8UC1);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = 255 - a->at<uchar>(i, j);
		}
	}
	
	return dst;
}

Mat Minus(Mat *b, Mat *a)
{
	return And(&Not(a), b);
}

bool Equal(Mat *a, Mat *b)
{
	int h = a->rows, w = a->cols;

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (a->at<uchar>(i, j) != b->at<uchar>(i, j)) return false;
		}
	}
	return true;
}

Point2i StartPoint(Mat *ac)
{
	int h = ac->rows, w = ac->cols;

	for (int i = 1; i < h; i++)
	{
		for (int j = 1; j < w; j++)
		{
			if (ac->at<uchar>(i, j) == 0 &&
				ac->at<uchar>(i, j - 1) == 255 &&
				ac->at<uchar>(i - 1, j) == 255) return Point2i(i, j);
		}
	}
	return Point2i(-1, -1);
}

Mat Boundary(Mat *a)
{
	return Minus(a, &Erode(a));
}

Mat RegionFill(Mat *a)
{
	int h = a->rows, w = a->cols;
	Mat notA = Not(a);
	Point2i startP = StartPoint(&notA);
	Mat x = Mat::zeros(h, w, CV_8UC1);
	x = Not(&x);

	x.at<uchar>(startP.x, startP.y) = 0;

	Mat newX = And(&Dilate4(&x), &notA);
	while (!Equal(&x, &newX))
	{
		x = newX;
		newX = And(&Dilate4(&x), &notA);
	}
	
	return Or(&newX, a);
}
#pragma endregion

#pragma region Runners
void Runner(Mat (*op)(Mat* src), int times)
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat res = img;
		for (int i = 0; i < times; i++)
		{
			res = op(&res);
		}
		imshow("img", img);
		imshow("res", res);
		waitKey(0);
	}
}
#pragma endregion

int main()
{
	int op;
	int times;
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 0 - Exit\n\n");
		printf(" 1 - Dilation <times>\n\n");
		printf(" 2 - Erosion <times>\n\n");
		printf(" 3 - Opening 1\n\n");
		printf(" 4 - Closing 1\n\n");
		printf(" 5 - Boundary 1\n\n");
		printf(" 6 - RegionFill 1\n\n");
		printf("Option: ");
		scanf("%d %d",&op,&times);
		switch (op)
		{
			case 1:
				Runner(Dilate, times);
				break;
			case 2:
				Runner(Erode, times);
				break;
			case 3:
				Runner(Opening, 1);
				break;
			case 4:
				Runner(Closing, 1);
				break;
			case 5:
				Runner(Boundary, 1);
				break;
			case 6:
				Runner(RegionFill, 1);
				break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}