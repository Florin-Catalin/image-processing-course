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

#pragma region Algo
bool is_low_pass_filter(Mat_<int> *filter)
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

void compute_for_low_pass(Mat_<int> &filter, float *scalingCoeff, float *additionFactor)
{
	float sum = 0;
	for (int i = 0; i < filter.rows; i++)
	{
		for (int j = 0; j < filter.cols; j++)
		{
			sum += filter.at<int>(i, j);
		}
	}
	*scalingCoeff = sum;
	*additionFactor = 0;
}

void compute_for_high_pass(Mat_<int> &filter, float *scalingCoeff, float *additionFactor)
{
	float sumPos = 0, sumNeg = 0;
	for (int i = 0; i < filter.rows; i++)
	{
		for (int j = 0; j < filter.cols; j++)
		{
			float f = filter.at<int>(i, j);
			sumPos = (f > 0) ? sumPos + f : sumPos;
			sumNeg = (f < 0) ? sumNeg + f : sumNeg;
		}
	}
	*scalingCoeff = 2 * max(sumPos, abs(sumNeg));
	*additionFactor = 128;
}

void convolution(Mat_<int> &filter, Mat_<uchar> &img, Mat_<int> &output) {

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
			output(i, j) = sum;// / scalingCoeff + additionFactor;
		}
	}
}

void SobelConvolution(Mat_<uchar>& img, Mat_<int>& gradX, Mat_<int>& gradY)
{
	int sobelXFilterData[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
	Mat_<int> sobelXFilter(3, 3, sobelXFilterData);

	int sobelYFilterData[9] = { 1, 2, 1, 0, 0, 0, -1, -2, -1 };
	Mat_<int> sobelYFilter(3, 3, sobelYFilterData);

	convolution(sobelXFilter, img, gradX);
	convolution(sobelYFilter, img, gradY);
}

void ComputeMag(Mat_<int>& gradX, Mat_<int>& gradY, Mat_<float>& mag)
{
	int height = gradX.rows;
	int width = gradX.cols;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			mag.at<float>(i, j) = sqrt(pow(gradX(i, j), 2) + pow(gradY(i, j), 2));
		}
	}
}

void ComputeOrientation(Mat_<int>& gradX, Mat_<int>& gradY, Mat_<float>& orientation)
{
	int height = gradX.rows;
	int width = gradX.cols;
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			orientation.at<float>(i, j) = atan2(gradY(i, j), gradX(i, j));
			orientation.at<float>(i, j) = (orientation.at<float>(i, j) < 0) ? orientation.at<float>(i, j) + CV_PI : orientation.at<float>(i, j);
		}
	}
}

int GetGradientDirection(float value)
{
	float quant = CV_PI / 8.0f;
	if (3 * quant < value && value < 5 * quant) return 0;
	if (quant < value && value < 3 * quant) return 1;
	if (5 * quant < value && value < 7 * quant) return 3;
	if ((0 < value && value < quant) || (7 * quant < value && value < 8 * quant)) return 2;
	return -1;
}

void NonMaximaSuppression(Mat_<uchar>& mag, Mat_<float>& orientation, Mat_<uchar>& suppressed)
{
	int height = mag.rows;
	int width = mag.cols;
	for(int i = 1; i < height-1; i++)
	{
		for(int j = 1; j < width-1; j++)
		{
			int gradient_dir = GetGradientDirection(orientation.at<float>(i, j));
			//std::cout << gradient_dir << std::endl;
			switch(gradient_dir)
			{
			case 0:
				suppressed.at<uchar>(i, j) = ((mag.at<uchar>(i - 1, j) < mag.at<uchar>(i, j)) && (mag.at<uchar>(i, j) > mag.at<uchar>(i + 1, j))) ? mag.at<uchar>(i, j) : 0;
				break;
			case 1:
				suppressed.at<uchar>(i, j) = ((mag.at<uchar>(i + 1, j - 1) < mag.at<uchar>(i, j)) && (mag.at<uchar>(i, j) > mag.at<uchar>(i - 1, j + 1))) ? mag.at<uchar>(i, j) : 0;
				break;
			case 2:
				suppressed.at<uchar>(i, j) = ((mag.at<uchar>(i, j - 1) < mag.at<uchar>(i, j)) && (mag.at<uchar>(i, j) > mag.at<uchar>(i, j + 1))) ? mag.at<uchar>(i, j) : 0;
				break;
			case 3:
				suppressed.at<uchar>(i, j) = ((mag.at<uchar>(i - 1, j - 1) < mag.at<uchar>(i, j)) && (mag.at<uchar>(i, j) > mag.at<uchar>(i + 1, j + 1))) ? mag.at<uchar>(i, j) : 0;
				break;
			default:
				suppressed.at<uchar>(i, j) = mag.at<uchar>(i, j);
				//std::cout << "erroooooooooooooooooooooooooooooooor" << std::endl;;
				break;
			}
		}
	}
}


int* Histogram(Mat *img)
{
	int *hist = new int[255 + 1]{ 0 };
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

int GetTHigh(int *hist, int NonEdgePixels)
{
	int sum = 0;
	int i = 1;
	while(sum < NonEdgePixels)
	{
		sum += hist[i];
		i++;
	}
	return i;
}

void Partition(Mat_<uchar>& magnorm, Mat_<uchar>& thresholded, int tHigh, int tLow)
{
	thresholded = magnorm.clone();
	for(int i = 1; i < magnorm.rows - 1; i++)
	{
		for(int j = 1; j < magnorm.cols - 1; j++)
		{
			if(magnorm.at<uchar>(i, j) < tLow)
			{
				thresholded.at<uchar>(i, j) = 0;
			}
			else if(magnorm.at<uchar>(i, j) < tHigh)
			{
				thresholded.at<uchar>(i, j) = 128;
			}
			else
			{
				thresholded.at<uchar>(i, j) = 255;
			}
		}
	}
}

void EdgeExtension(Mat_<uchar>& thresholded, Mat_<uchar>& pureEdges)
{
	int  h = thresholded.rows, w = thresholded.cols;
	Mat labels = Mat::zeros(h, w, CV_32SC1);
	pureEdges = Mat::zeros(h, w, CV_8UC1);
	int di[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
	int dj[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };

	for (int i = 1; i < h - 1; i++)
	{
		for (int j = 1; j < w - 1; j++)
		{
			if (thresholded.at<uchar>(i, j) == 255 && labels.at<int>(i, j) == 0)
			{
				std::queue<Point2i> Q;

				labels.at<int>(i, j) = 1;
				Q.push({ i, j });
				while (!Q.empty())
				{
					Point2i q = Q.front();
					Q.pop();
					for (int k = 0; k < 8; k++)
					{
						int ki = q.x + di[k], kj = q.y + dj[k];
						if (thresholded.at<uchar>(ki, kj) >= 128 &&
							labels.at<int>(ki, kj) == 0)
						{
							pureEdges.at<uchar>(ki, kj) = 255;
							labels.at<int>(ki, kj) = 1;
							Q.push({ ki, kj });
						}
					}
				}
			}
		}
	}
}

Mat EdgeDetectionProcess(Mat_<uchar>& img, double p, double k)
{
	int gaussianFilterData[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
	Mat_<int> gaussianFilter(3, 3, gaussianFilterData);

	Mat_<int> afterGaussian;
	convolution(gaussianFilter, img, afterGaussian);

	Mat_<uchar> blurred;
	cv::Size size(0, 0);
	GaussianBlur(img, blurred, size, 0.8);

	imshow("img", img);
	imshow("blur", blurred);
	//imshow("afterG", afterGaussian);

	Mat_<int> gradX;
	Mat_<int> gradY;

	SobelConvolution(blurred, gradX, gradY);

	//imshow("gradX", gradX);
	//imshow("gradY", gradY);

	Mat_<float> mag = Mat::zeros(gradX.rows, gradX.cols, CV_32FC1);
	ComputeMag(gradX, gradY, mag);

	Mat_<float> orientation = Mat::zeros(gradX.rows, gradX.cols, CV_32FC1);
	ComputeOrientation(gradX, gradY, orientation);

	Mat_<uchar> magnorm = Mat::zeros(gradX.rows, gradX.cols, CV_8UC1);
	

	normalize(mag, magnorm, 0, 255, NORM_MINMAX, CV_8UC1);

	imshow("magnorm", magnorm);


	Mat_<uchar> suppressed = Mat::zeros(gradX.rows, gradX.cols, CV_8UC1);
	NonMaximaSuppression(magnorm, orientation, suppressed);

	imshow("suppressed", suppressed);

	int *hist = Histogram(&suppressed);

	int NonEdgePixels = (1 - p) * ((magnorm.rows - 2) * (magnorm.cols - 2) - hist[0]);
	int THigh = GetTHigh(hist, NonEdgePixels);
	int TLow = k * THigh;

	std::cout << THigh << "," << TLow << std::endl;
	
	Mat_<uchar> thresholded;
	Partition(suppressed, thresholded, THigh, TLow);

	imshow("thresholded", thresholded);

	Mat_<uchar> pureEdges;

	EdgeExtension(thresholded, pureEdges);

	imshow("pureEdges", pureEdges);

	return magnorm;
}

#pragma endregion

#pragma region Runners
void EdgeDetectionRunner(double p, double k)
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat dst = EdgeDetectionProcess(img, p, k);
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
		printf(" 1 - EdgeDetection\n\n");
		printf("Option: ");
		//scanf("%d",&op);
		op = 1;
		switch (op)
		{
		case 1:
			EdgeDetectionRunner(0.155, 0.4);
			//EdgeDetectionRunner(0.1, 0.4);
			break;
			default:
				system("cls");
				break;
		}
	}
	while (op!=0);
	return 0;
}