// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"

using namespace std;
using namespace cv;


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

#pragma region FilterUtils
bool is_low_pass_filter(Mat_<int> *filter)
{
	for(int i = 0; i < filter->rows; i++)
	{
		for(int j = 0; j < filter->cols; j++)
		{
			if (filter->at<int>(i, j) < 0) return false;
		}
	}
	return true;
}

void compute_for_low_pass(Mat_<int> &filter, int *scalingCoeff, int *additionFactor)
{
	int sum = 0;
	for(int i = 0; i < filter.rows; i++)
	{
		for(int j = 0; j < filter.cols; j++)
		{
			sum += filter(i, j);
		}
	}
	*scalingCoeff = sum;
	*additionFactor = 0;
}
void compute_for_high_pass(Mat_<int> &filter, int *scalingCoeff, int *additionFactor)
{
	int sumPos = 0, sumNeg = 0;
	for(int i = 0; i < filter.rows; i++)
	{
		for(int j = 0; j < filter.cols; j++)
		{
			int f = filter(i, j);
			sumPos = (f > 0) ? sumPos + f : sumPos;
			sumNeg = (f < 0) ? sumNeg + f : sumNeg;
		}
	}
	*scalingCoeff = 2 * max(sumPos, abs(sumNeg));
	*additionFactor = 128;
}
#pragma endregion

#pragma region Filter
void convolution(Mat_<int> &filter, Mat_<uchar> &img, Mat_<uchar> &output) {

    output.create(img.size());
    memcpy(output.data, img.data, img.rows * img.cols * sizeof(uchar));

    int scalingCoeff = 1;
    int additionFactor = 0;

    //TODO: decide if the filter is low pass or high pass and compute the scaling coefficient and the addition factor
    // low pass if all elements >= 0
    // high pass has elements < 0
	
	bool is_low = is_low_pass_filter(&filter);

    // compute scaling coefficient and addition factor for low pass and high pass
    // low pass: additionFactor = 0, scalingCoeff = sum of all elements
    // high pass: formula 9.20

	if(is_low)
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
	for(int i = filter_halfsize.x; i < img.rows - filter_halfsize.x; i++)
	{
		for(int j = filter_halfsize.y; j < img.cols - filter_halfsize.y; j++)
		{
			int sum = 0;
			Point2i filter_corner = Point2i(i - filter_halfsize.x, j - filter_halfsize.y);
			for(int u = 0; u < filter.rows; u++)
			{
				for(int v = 0; v < filter.cols; v++)
				{
					sum += img(filter_corner.x + u, filter_corner.y + v) * filter(u, v);
				}
			}
			output(i, j) = sum / scalingCoeff + additionFactor;
		}
	}
}


/*  in the frequency domain, the process of convolution simplifies to multiplication => faster than in the spatial domain
    the output is simply given by F(u,v)×G(u,v) where F(u,v) and G(u,v) are the Fourier transforms of their respective functions
    The frequency-domain representation of a signal carries information about the signal's magnitude and phase at each frequency*/

/*
The algorithm for filtering in the frequency domain is:
    a) Perform the image centering transform on the original image (9.15)
    b) Perform the DFT transform
    c) Alter the Fourier coefficients according to the required filtering
    d) Perform the IDFT transform
    e) Perform the image centering transform again (this undoes the first centering transform)
 */

void centering_transform(Mat img){
//expects floating point image
    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
        }
    }
}

Mat generic_frequency_domain_filter(Mat src, int tp, int R2)
{
    int height = src.rows;
    int width = src.cols;

    Mat srcf;
    src.convertTo(srcf, CV_32FC1);
    // Centering transformation
    centering_transform(srcf);

    //perform forward transform with complex image output
    Mat fourier;
    dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

    //split into real and imaginary channels fourier(i, j) = Re(i, j) + i * Im(i, j)
    Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
    split(fourier, channels);  // channels[0] = Re (real part), channels[1] = Im (imaginary part)

    //calculate magnitude and phase in floating point images mag and phi
    // http://www3.ncc.edu/faculty/ens/schoenf/elt115/complex.html
    // from cartesian to polar coordinates

    Mat mag, phi;
    magnitude(channels[0], channels[1], mag);
    phase(channels[0], channels[1], phi);


    // TODO: Display here the log of magnitude (Add 1 to the magnitude to avoid log(0)) (see image 9.4e))
    // do not forget to normalize
	
	Mat lg;
	log(mag + 1, lg);
    normalize(lg, lg, 0, 1, CV_MINMAX);
	imshow("lg", lg);


    // TODO: Insert filtering operations here ( channels[0] = Re(DFT(I), channels[1] = Im(DFT(I) )
	for (int k = 0; k <= 1; k++)
	{
		for (int u = 0; u < src.rows; u++)
		{
			for (int v = 0; v < src.cols; v++)
			{
				if (tp == 1)
				{
					channels[k].at<float>(u, v) = (pow((src.rows / 2 - u), 2) + pow((src.cols / 2 - v), 2) < R2) ? channels[k].at<float>(u, v) : 0;
				}
				else if(tp == 2)
				{
					channels[k].at<float>(u, v) = (pow((src.rows / 2 - u), 2) + pow((src.cols / 2 - v), 2) > R2) ? channels[k].at<float>(u, v) : 0;
				}
				else if(tp == 3)
				{
					channels[k].at<float>(u, v) = channels[k].at<float>(u, v) * exp(-(pow((height / 2 - u), 2) + pow((width / 2 - v), 2)) / R2);
				}
				else
				{
					channels[k].at<float>(u, v) = channels[k].at<float>(u, v) * (1 - exp(-(pow((height / 2 - u), 2) + pow((width / 2 - v), 2)) / R2));
				}
			}
		}
	}


    //perform inverse transform and put results in dstf
    Mat dst, dstf;
    merge(channels, 2, fourier);
    dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT);

    // Inverse Centering transformation
    centering_transform(dstf);

    //normalize the result and put in the destination image
    normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);

    return dst;
}


#pragma endregion 

int main()
{
	int op;
	// PART 1: convolution in the spatial domain
    Mat_<uchar> img = imread("d:\\univ\\code\\y3se2\\ip\\labs\\lab8\\OpenCVApplication-VS2017_OCV340_basic\\Images\\cameraman.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    Mat_<uchar> outputImage;

    // LOW PASS
    // mean filter 5x5
    int meanFilterData5x5[25];
    fill_n(meanFilterData5x5, 25, 1);
    Mat_<int> meanFilter5x5(5, 5, meanFilterData5x5);

    // mean filter 3x3
    Mat_<int> meanFilter3x3(3, 3, meanFilterData5x5);

    // gaussian filter
    int gaussianFilterData[9] = {1, 2, 1, 2, 4, 2, 1, 2, 1};
    Mat_<int> gaussianFilter(3, 3, gaussianFilterData);

    // HIGH PASS
    // laplace filter 3x3
    int laplaceFilterData[9] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    Mat_<int> laplaceFilter(3, 3, laplaceFilterData);

    int highpassFilterData[9] = {-1, -1, -1, -1, 9, -1, -1, -1, -1};
    Mat_<int> highpassFilter(3, 3, highpassFilterData);

    //TODO: convolution with the mean filter 5 x 5
    //TODO: convolution with the mean filter 3 x 3
    //TODO: convolution with the gaussian filter
    //TODO: convolution with the laplacian filter
    //TODO: convolution with the highpass filter


    // PART 2: convolution in the frequency domain
    // use the generic_frequency_domain_filter() function

    // TODO: convolution with the ideal low pass filter (formula 9.16) take R^2 = 20
    // TODO: convolution with the ideal high pass filter (formula 9.17) take R^2 = 20
    // TODO: convolution with the Gaussian low pass filter (formula 9.18) take A = 10
    // TODO: convolution with the Gaussian high pass filter (formula 9.19) take A = 10
	do
	{
		destroyAllWindows();
		printf("Menu:\n");
		printf(" 0 - Exit\n\n");
		printf("1 - convolution with the mean filter 5 x 5\n");
		printf("2 - convolution with the mean filter 3 x 3\n");
		printf("3 - convolution with the gaussian filter\n");
		printf("4 - convolution with the laplacian filter\n");
		printf("5 - convolution with the highpass filter\n");
		printf("6 - convolution with the ideal low pass filter (formula 9.16) take R^2 = 20\n");
		printf("7 - convolution with the ideal high pass filter (formula 9.17) take R^2 = 20\n");
		printf("8 - convolution with the Gaussian low pass filter (formula 9.18) take A = 10\n");
		printf("9 - convolution with the Gaussian high pass filter (formula 9.19) take A = 10\n");

		printf("Option: ");
		scanf("%d",&op);
		getchar();
		int r = 400, a = 100;
		switch (op)
		{
		case 1:
			convolution(meanFilter5x5, img, outputImage);
			break;
		case 2:
			convolution(meanFilter3x3, img, outputImage);
			break;
		case 3:
			convolution(gaussianFilter, img, outputImage);
			break;
		case 4:
			convolution(laplaceFilter, img, outputImage);
			break;
		case 5:
			convolution(highpassFilter, img, outputImage);
			break;
		case 6:
			outputImage = generic_frequency_domain_filter(img, 1, r);
			break;
		case 7:
			outputImage = generic_frequency_domain_filter(img, 2, r);
			break;
		case 8:
			outputImage = generic_frequency_domain_filter(img, 3, a);
			break;
		case 9:
			outputImage = generic_frequency_domain_filter(img, 4, a);
			break;

			default:
				system("cls");
				break;
		}
		imshow("original", img);
		imshow("output", outputImage);
		waitKey(0);
	}
	while (op!=0);
	return 0;
}