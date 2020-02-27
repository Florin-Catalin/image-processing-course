// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

using namespace std;

#pragma region Utils
void testOpenImage()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat src;
		src = imread(fname);
		imshow("image", src);
		waitKey();
	}
}

void testOpenImagesFld()
{
	char folderName[MAX_PATH];
	if (openFolderDlg(folderName) == 0)
		return;
	char fname[MAX_PATH];
	FileGetter fg(folderName, "bmp");
	while (fg.getNextAbsFile(fname))
	{
		Mat src;
		src = imread(fname);
		imshow(fg.getFoundFileName(), src);
		if (waitKey() == 27) //ESC pressed
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

#pragma region Algo
Mat MeanFilter(Mat *src, int size)
{
	int h = src->rows, w = src->cols;
	int halfsize = size / 2;
	Mat dst = src->clone();

	for (int i = halfsize; i < h - halfsize; i++)
	{
		for (int j = halfsize; j < w - halfsize; j++)
		{
			int corner_i = i - halfsize;
			int corner_j = j - halfsize;
			vector<uchar> neigh;
			for (int ki = 0; ki < size; ki++)
			{
				for (int kj = 0; kj < size; kj++)
				{
					neigh.push_back(src->at<uchar>(corner_i + ki, corner_j + kj));
				}
			}
			sort(neigh.begin(), neigh.end());
			dst.at<uchar>(i, j) = neigh.at(size * size / 2);
		}
	}
	return dst;
}

bool is_low_pass_filter(Mat_<float> *filter)
{
	for (int i = 0; i < filter->rows; i++)
	{
		for (int j = 0; j < filter->cols; j++)
		{
			if (filter->at<int>(i, j) < 0) return false;
		}
	}
	return true;
}

void compute_for_low_pass(Mat_<float> &filter, float *scalingCoeff, float *additionFactor)
{
	float sum = 0;
	for (int i = 0; i < filter.rows; i++)
	{
		for (int j = 0; j < filter.cols; j++)
		{
			sum += filter(i, j);
		}
	}
	*scalingCoeff = sum;
	*additionFactor = 0;
}

void compute_for_high_pass(Mat_<float> &filter, float *scalingCoeff, float *additionFactor)
{
	float sumPos = 0, sumNeg = 0;
	for (int i = 0; i < filter.rows; i++)
	{
		for (int j = 0; j < filter.cols; j++)
		{
			float f = filter(i, j);
			sumPos = (f > 0) ? sumPos + f : sumPos;
			sumNeg = (f < 0) ? sumNeg + f : sumNeg;
		}
	}
	*scalingCoeff = 2 * max(sumPos, abs(sumNeg));
	*additionFactor = 128;
}

void convolution(Mat_<float> &filter, Mat_<uchar> &img, Mat_<uchar> &output) {

	output.create(img.size());
	memcpy(output.data, img.data, img.rows * img.cols * sizeof(uchar));

	float scalingCoeff = 1;
	float additionFactor = 0;

	//TODO: decide if the filter is low pass or high pass and compute the scaling coefficient and the addition factor
	// low pass if all elements >= 0
	// high pass has elements < 0

	bool is_low = is_low_pass_filter(&filter);

	// compute scaling coefficient and addition factor for low pass and high pass
	// low pass: additionFactor = 0, scalingCoeff = sum of all elements
	// high pass: formula 9.20

	if (is_low)
	{
		compute_for_low_pass(filter, &scalingCoeff, &additionFactor);
	}
	else
	{
		compute_for_high_pass(filter, &scalingCoeff, &additionFactor);
	}

	// TODO: implement convolution operation (formula 9.2)
	// do not forget to divide with the scaling factor and add the addition factor in order to have values between [0, 255]
	output = img.clone();
	Point2i filter_halfsize = Point2i(filter.rows / 2, filter.cols / 2);
	for (int i = filter_halfsize.x; i < img.rows - filter_halfsize.x; i++)
	{
		for (int j = filter_halfsize.y; j < img.cols - filter_halfsize.y; j++)
		{
			int sum = 0;
			Point2i filter_corner = Point2i(i - filter_halfsize.x, j - filter_halfsize.y);
			for (int u = 0; u < filter.rows; u++)
			{
				for (int v = 0; v < filter.cols; v++)
				{
					sum += img(filter_corner.x + u, filter_corner.y + v) * filter(u, v);
				}
			}
			output(i, j) = sum / scalingCoeff + additionFactor;
		}
	}
}

Mat_<float> Construct2DGaussianFilter(int size)
{
	double stdev = size / 6.0;
	Mat_<float> filter(size, size, CV_32FC1);
	int center = size / 2;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			filter.at<float>(i, j) = 1 / (2 * CV_PI * pow(stdev, 2)) * exp(-(pow(i - center, 2) + pow(j - center, 2)) / (2 * pow(stdev, 2)));
		}
	}
	return filter;
}

Mat_<float> Construct1DGaussianFilterY(int size)
{
	double stdev = size / 6.0;
	Mat_<float> filter(size, 1, CV_32FC1);
	int center = size / 2;
	for (int i = 0; i < size; i++)
	{
		filter.at<float>(i, 0) = 1 / (sqrt(2 * CV_PI) * stdev) * exp(-(pow(i - center, 2)) / (2 * pow(stdev, 2)));
	}
	return filter;
}

Mat_<float> Construct1DGaussianFilterX(int size)
{
	double stdev = size / 6.0;
	Mat_<float> filter(1, size, CV_32FC1);
	int center = size / 2;
	for (int i = 0; i < size; i++)
	{
		filter.at<float>(0, i) = 1 / (sqrt(2 * CV_PI) * stdev) * exp(-(pow(i - center, 2)) / (2 * pow(stdev, 2)));
	}
	return filter;
}
#pragma endregion

#pragma region Runners
void RunnerMeanFilter(int size)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("img", img);
		Mat dst = MeanFilter(&img, size);
		imshow("dst", dst);
		waitKey(0);
	}
}

void Runner2DGaussianFilter(int size)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("img", img);
		Mat_<float> gaussianFilter = Construct2DGaussianFilter(size);
		Mat_<uchar> dst;
		convolution(gaussianFilter, img, dst);
		imshow("dst", dst);
		waitKey(0);
	}
}

void Runner1DGaussianFilter(int size)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("img", img);
		Mat_<float> gaussianFilterX = Construct1DGaussianFilterX(size);
		Mat_<float> gaussianFilterY = Construct1DGaussianFilterY(size);
		Mat_<uchar> dst;
		Mat_<uchar> interm;
		convolution(gaussianFilterY, img, interm);
		convolution(gaussianFilterX, interm, dst);
		imshow("dst", dst);
		waitKey(0);
	}
}
#pragma endregion

int main()
{
	int op, size;
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 0 - Exit\n\n");
		printf(" 1 - Mean filter\n\n");
		printf(" 2 - Gaussian 2D filter\n\n");
		printf(" 3 - Gaussian 1D filter\n\n");
		printf("Option Size: ");
		scanf("%d %d", &op, &size);
		switch (op)
		{
		case 1:
			RunnerMeanFilter(size);
			break;
		case 2:
			Runner2DGaussianFilter(size);
			break;
		case 3:
			Runner1DGaussianFilter(size);
			break;
		default:
			system("cls");
			break;
		}
	} while (op != 0);
	return 0;
}