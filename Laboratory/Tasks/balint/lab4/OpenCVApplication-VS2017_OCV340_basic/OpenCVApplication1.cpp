// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <queue>
#include <random>

using namespace std;

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

Mat generateLabelMatrix(Mat *img) 
{
	int  h = img->rows, w = img->cols;
	int label = 0;
	Mat labels = Mat::zeros(h, w, CV_32SC1);
	int di[8] = { -1, -1, 0, 1, 1, 1, 0, -1};
	int dj[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (img->at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0)
			{
				label++;
				queue<Point2i> Q;

				labels.at<int>(i, j) = label;
				Q.push({ i, j });
				while (!Q.empty())
				{
					Point2i q = Q.front();
					Q.pop();
					for (int k = 0; k < 8; k++)
					{
						int ki = q.x + di[k], kj = q.y + dj[k];
						if(img->at<uchar>(ki, kj) == 0 &&
							labels.at<int>(ki, kj) == 0)
						{
							labels.at<int>(ki, kj) = label;
							Q.push({ ki, kj });
						}
					}
				}
			}
		}
	}


	return labels;
}

Mat assignColorToLabels(Mat *labels) 
{
	default_random_engine gen;
	uniform_int_distribution<int> d(0, 255);
	int h = labels->rows, w = labels->cols;
	Vec3b colors[255];
	Mat dst(h, w, CV_8UC3);

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			uchar label = labels->at<int>(i, j);
			if (label == 0) 
			{
				dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
				continue;
			}
			if (colors[label][0] == 0 || colors[label][1] == 0 || colors[label][2] == 0)
			{
				colors[label][0] = d(gen);
				colors[label][1] = d(gen);
				colors[label][2] = d(gen);
			}
			dst.at<Vec3b>(i, j) = colors[label];
		}
	}
	return dst;
}

void bfsLabel()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("src", src);
		Mat labels = generateLabelMatrix(&src);
		imshow("labels", labels);
		Mat dst = assignColorToLabels(&labels);
		imshow("dst", dst);
		waitKey(0);
	}
}

int minel(vector<int> vct)
{
	int min = INT_MAX;
	for each (int y in vct)
	{
		min = (y < min) ? y : min;
	}
	return min;
}

Mat generateLabelTwoPass(Mat *src)
{
	int label = 0;
	int h = src->rows, w = src->cols;
	Mat labels = Mat::zeros(h, w, CV_32SC1);
	vector<vector<int>> edges;
	int di[4] = { 0, -1, -1, -1 };
	int dj[4] = { -1, -1, 0, 1 };

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			if (src->at<uchar>(i, j) == 0 &&
				labels.at<int>(i, j) == 0)
			{
				vector<int> L;
				for (int k = 0; k < 4; k++)
				{
					int ki = i + di[k];
					int kj = j + dj[k];
					if (labels.at<int>(ki, kj) > 0)
					{
						L.push_back(labels.at<int>(ki, kj));
					}
				}
				if (L.size() == 0)
				{
					label++;
					labels.at<int>(i, j) = label;
					edges.resize(label + 1);
				}
				else
				{
					int x = *min_element(begin(L), end(L));
					//int x = minel(L);
					labels.at<int>(i, j) = x;
					for each (int y in L)
					{
						if (y != x)
						{
							edges.at(x).push_back(y);
							edges.at(y).push_back(x);
						}
					}
				}
			}
		}
	}

	imshow("labels1", labels);

	int newlabel = 0;
	vector<int> newlabels(label + 1, 0);
	for (int i = 1; i <= label; i++)
	{
		if (newlabels.at(i) == 0)
		{
			newlabel++;
			queue<int> Q;
			newlabels.at(i) = newlabel;
			Q.push(i);
			while (!Q.empty())
			{
				int x = Q.front();
				Q.pop();
				for each (int y in edges.at(x))
				{
					if (newlabels.at(y) == 0)
					{
						newlabels.at(y) = newlabel;
						Q.push(y);
					}
				}
			}
		}
	}

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			labels.at<int>(i, j) = newlabels.at(labels.at<int>(i, j));
		}
	}
	return labels;
}

void twoPassLabel()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("src", src);
		Mat labels = generateLabelTwoPass(&src);
		imshow("labels2", labels);
		Mat dst = assignColorToLabels(&labels);
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
		printf(" 1 - BFS\n\n");
		printf(" 2 - 2Pass\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
			case 1:
				bfsLabel();
				break;
			case 2:
				twoPassLabel();
				break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}