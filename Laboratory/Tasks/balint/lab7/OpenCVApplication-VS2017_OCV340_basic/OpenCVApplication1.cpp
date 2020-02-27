// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

#define HIGHEST_INTENSITY 255
#define LOWEST_INTENSITY 0

using namespace std;

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
#pragma endregion

#pragma region Functions
double Mean(Mat *img, int tlow = 0, int thigh = HIGHEST_INTENSITY + 1)
{
	int h = img->rows, w = img->cols, size = 0;
	double sum = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			int value = img->at<uchar>(i, j);
			if (value > tlow && value < thigh)
			{
				sum += value;
				size++;
			}
		}
	}
	return sum / size;
}

double StandardDeviation(Mat *img, float mean)
{
	int h = img->rows, w = img->cols, size = h * w;
	double sum = 0;
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			sum += pow((img->at<uchar>(i, j) - mean), 2);
		}
	}
	return sqrt(sum / size);
}

int* Histogram(Mat *img)
{
	int *hist = new int[HIGHEST_INTENSITY + 1]{ 0 };
	int h = img->rows, w = img->cols;
	
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			hist[img->at<uchar>(i, j)]++;
		}
	}

	return hist;
}

Point2i IMinMax(Mat *img)
{
	int h = img->rows, w = img->cols, min = INT_MAX, max = INT_MIN;
	
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			uchar value = img->at<uchar>(i, j);
			min = (min > value) ? value : min;
			max = (max < value) ? value : max;
		}
	}

	return Point2i(min, max);
}

float HistogramMean(int *hist, int tlow, int thigh)
{
	int n = 0;
	float sum = 0;
	for (int i = tlow; i < thigh; i++)
	{
		n += hist[i];
		sum += i * hist[i];
	}
	return sum / n;
}

float AutomaticThreshold(Mat *img, int *hist, float error)
{
	Point2i iminmax = IMinMax(img);
	cout << iminmax << endl;
	float told = (iminmax.x + iminmax.y) / 2, tnew = 0;

	float meanG1 = HistogramMean(hist, 0, told);
	float meanG2 = HistogramMean(hist, told, HIGHEST_INTENSITY);

	tnew = (meanG1 + meanG2) / 2;

	while (abs(tnew - told) > error)
	{
		told = tnew;
		meanG1 = HistogramMean(hist, 0, told);
		meanG2 = HistogramMean(hist, told, HIGHEST_INTENSITY);
		tnew = (meanG1 + meanG2) / 2;
	}

	return tnew;
}

Mat ThresholdImage(Mat *img, int threshold)
{
	int h = img->rows, w = img->cols;
	Mat dst = img->clone();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = (img->at<uchar>(i, j) < threshold) ? 0 : HIGHEST_INTENSITY;
		}
	}

	return dst;
}

Point2i HistogramMinMax(int *hist)
{
	int min = 0, max = HIGHEST_INTENSITY;

	for (int i = 0; i < HIGHEST_INTENSITY + 1; i++)
	{
		if (hist[i] > 0)
		{
			min = i;
			break;
		}
	}
	for (int i = HIGHEST_INTENSITY; i >= 0; i++)
	{
		if (hist[i] > 0)
		{
			max = i;
			break;
		}
	}

	return Point2i(min, max);
}

Mat StretchHistogram(Mat *img, int *hist, int goutMin, int goutMax)
{
	Point2i histMinMax = HistogramMinMax(hist);
	int h = img->rows, w = img->cols;
	Mat dst = img->clone();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = goutMin + (img->at<uchar>(i, j) - histMinMax.x) * (goutMax - goutMin) / (histMinMax.y - histMinMax.x);
		}
	}

	return dst;
}

uchar Clamp(float x)
{
	x = (x < LOWEST_INTENSITY) ? LOWEST_INTENSITY : x;
	x = (x > HIGHEST_INTENSITY) ? HIGHEST_INTENSITY : x;
	return x;
}

Mat GammaCorrect(Mat *img, float gamma)
{
	int h = img->rows, w = img->cols;
	Mat dst = img->clone();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = Clamp(HIGHEST_INTENSITY * pow((img->at<uchar>(i, j) / 255.0), gamma));
		}
	}

	return dst;
}

Mat Brightness(Mat *img, uchar diff)
{
	int h = img->rows, w = img->cols;
	Mat dst = img->clone();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			dst.at<uchar>(i, j) = Clamp(img->at<uchar>(i, j) + diff);
		}
	}

	return dst;
}

float* CPDF(int *hist, float size)
{
	int nHist[HIGHEST_INTENSITY + 1] = {0};
	float *cpdf = new float[HIGHEST_INTENSITY + 1];
	nHist[0] = hist[0];
	for (int i = 0; i < HIGHEST_INTENSITY; i++)
	{
		if(i == 0)
		{
			nHist[i] = hist[i];
		}
		else
		{
			nHist[i] = hist[i] + nHist[i - 1];
		}
		cpdf[i] = nHist[i] / size;
	}
	return cpdf;
}

Mat EqualizedHistogram(Mat *img, float *cpdf)
{
	int h = img->rows, w = img->cols;
	Mat dst = img->clone();

	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			float cpdf1 = cpdf[img->at<uchar>(i, j)];
			float newi = HIGHEST_INTENSITY * cpdf1;
			dst.at<uchar>(i, j) = newi;
		}
	}

	return dst;

}
#pragma endregion

#pragma region Runners
void RunnerMSTD()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		float mean = Mean(&img);
		float stdev = StandardDeviation(&img, mean);
		cout << "Mean: " << mean << endl;
		cout << "StdDev: " << stdev << endl;
		waitKey(0);
	}
}

void RunnerHist()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int *hist = Histogram(&img);
		showHistogram("Hist", hist, 255, 200);
		if (hist)
			delete hist;
		waitKey(0);
	}
}

void RunnerAutThres(float error)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int *hist = Histogram(&img);
		imshow("img", img);
		showHistogram("Hist", hist, 255, 200);
		int threshold = AutomaticThreshold(&img, hist, error);
		cout << "Threshold: " << threshold << endl;
		cout << "Error: " << error << endl;
		Mat thrImg = ThresholdImage(&img, threshold);
		imshow("thrImg", thrImg);
		if (hist)
			delete hist;
		waitKey(0);
	}
}

void RunnerStretch(int goutMin, int goutMax)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int *hist = Histogram(&img);
		imshow("img", img);
		showHistogram("Hist", hist, 255, 200);
		Mat dst = StretchHistogram(&img, hist, goutMin, goutMax);
		imshow("dst", dst);
		int *newHist = Histogram(&dst);
		showHistogram("newHist", newHist, 255, 200);
		if (newHist)
			delete newHist;
		if (hist)
			delete hist;
		waitKey(0);
	}
}

void RunnerGamma(float gamma)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("img", img);
		Mat dst = GammaCorrect(&img, gamma);
		imshow("dst", dst);
		waitKey(0);
	}
}

void RunnerBrightness(uchar diff)
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		imshow("img", img);
		Mat dst = Brightness(&img, diff);
		imshow("dst", dst);
		waitKey(0);
	}
}

void RunnerEqualized()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int *hist = Histogram(&img);
		imshow("img", img);
		showHistogram("Hist", hist, 255, 200);
		float *cpdf = CPDF(hist, img.rows * img.cols);
		Mat dst = EqualizedHistogram(&img, cpdf);
		int *nHist = Histogram(&dst);
		showHistogram("nHist", nHist, 255, 200);
		imshow("dst", dst);
		if (cpdf)
			delete cpdf;
		if (hist)
			delete hist;
		waitKey(0);
	}
}


#pragma endregion

int main()
{
	int op;
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 0 - Exit\n\n");
		printf(" 1 - Mean & Standard deviation\n\n");
		printf(" 2 - Histogram\n\n");
		printf(" 3 - Threshold\n\n");
		printf(" 4 - Stretch\n\n");
		printf(" 5 - Gamma\n\n");
		printf(" 6 - Brightness\n\n");
		printf(" 7 - EqualizedHist\n\n");
		printf("Option: ");
		scanf("%d",&op);
		switch (op)
		{
		case 1:
			RunnerMSTD();
			break;
		case 2:
			RunnerHist();
			break;
		case 3:
			RunnerAutThres(0.1);
			break;
		case 4:
			RunnerStretch(0, 250);
			break;
		case 5:
			RunnerGamma(4.5);
			break;
		case 6:
			RunnerBrightness(50);
			break;
		case 7:
			RunnerEqualized();
			break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}