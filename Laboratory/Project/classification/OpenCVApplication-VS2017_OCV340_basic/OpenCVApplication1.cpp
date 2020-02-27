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

void loadTrainTestLabel(std::string &pathName, std::vector<Mat> &trainCells, std::vector<Mat> &testCells, std::vector<int> &trainLabels, std::vector<int> &testLabels){

    int SZ = 20;
    Mat img = imread(pathName,CV_LOAD_IMAGE_GRAYSCALE);
    int ImgCount = 0;
    for(int i = 0; i < img.rows; i = i + SZ)
    {
        for(int j = 0; j < img.cols; j = j + SZ)
        {
            Mat digitImg = (img.colRange(j,j+SZ).rowRange(i,i+SZ)).clone();
            if(j < int(0.9*img.cols))
            {
                trainCells.push_back(digitImg);
            }
            else
            {
                testCells.push_back(digitImg);
            }
            ImgCount++;
        }
    }

	std::cout << "Image Count : " << ImgCount << std::endl;
    float digitClassNumber = 0;

    for(int z=0;z<int(0.9*ImgCount);z++){
        if(z % 450 == 0 && z != 0){
            digitClassNumber = digitClassNumber + 1;
        }
        trainLabels.push_back(digitClassNumber);
    }
    digitClassNumber = 0;
    for(int z=0;z<int(0.1*ImgCount);z++){
        if(z % 50 == 0 && z != 0){
            digitClassNumber = digitClassNumber + 1;
        }
        testLabels.push_back(digitClassNumber);
    }
}

std::vector <Mat> trainCells;
std::vector <Mat> testCells;
std::vector<int> trainLabels;
std::vector<int> testLabels;
std::vector<std::vector<float>> trainHogs;
std::vector<std::vector<float>> testHogs;

void loadMNIST() {
	std::string pathName = "digits.png";
    loadTrainTestLabel(pathName, trainCells, testCells, trainLabels, testLabels);
}

Mat ComputeGradX(Mat *block)
{
	int h = block->rows;
	int w = block->cols;
	Mat gradX = Mat::zeros(h - 2, w - 2, CV_32SC1);
	for(int i = 1; i < h - 1; i++)
	{
		for(int j = 1; j < w - 1; j++)
		{
			gradX.at<int>(i - 1, j - 1) = block->at<uchar>(i, j + 1) - block->at<uchar>(i, j - 1);
		}
	}
	return gradX;
}

Mat ComputeGradY(Mat *block)
{
	int h = block->rows;
	int w = block->cols;
	Mat gradY = Mat::zeros(h - 2, w - 2, CV_32SC1);
	for(int j = 1; j < w - 1; j++)
	{
		for(int i = 1; i < w - 1; i++)
		{
			gradY.at<int>(i - 1, j - 1) = block->at<uchar>(i + 1, j) - block->at<uchar>(i - 1, j);
		}
	}
	return gradY;
}

void ComputeMag(Mat_<int>& gradX, Mat_<int>& gradY, Mat_<float>& mag)
{
	int height = gradX.rows;
	int width = gradX.cols;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			mag.at<float>(i, j) = sqrt(pow(gradX(i, j), 2) + pow(gradY(i, j), 2));
		}
	}
}

void ComputeOrientation(Mat_<int>& gradX, Mat_<int>& gradY, Mat_<float>& orientation)
{
	int height = gradX.rows;
	int width = gradX.cols;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			orientation.at<float>(i, j) = atan2(gradY(i, j), gradX(i, j));
			orientation.at<float>(i, j) = (orientation.at<float>(i, j) < 0) ? orientation.at<float>(i, j) + CV_PI : orientation.at<float>(i, j);
			orientation.at<float>(i, j) = orientation.at<float>(i, j) * 180 / CV_PI;
		}
	}
}

int DegreeToBin(float deg)
{
	if (0 <= deg && deg < 20) return 0;
	if (20 <= deg && deg < 40) return 1;
	if (40 <= deg && deg < 60) return 2;
	if (60 <= deg && deg < 80) return 3;
	if (80 <= deg && deg < 100) return 4;
	if (100 <= deg && deg < 120) return 5;
	if (120 <= deg && deg < 140) return 6;
	if (140 <= deg && deg < 160) return 7;
	if (160 <= deg && deg < 180) return 8;
	if (deg == 180) return 0;
}

//orient -> rad -> degree
std::vector<float> process_one_block(Mat *block)
{
	std::vector<float> hog(9, 0);
	Mat_<int> gradX = ComputeGradX(block);
	Mat_<int> gradY = ComputeGradY(block);
	Mat_<float> mag = Mat::zeros(gradX.rows, gradX.cols, CV_32FC1);
	ComputeMag(gradX, gradY, mag);

	Mat_<float> orientation = Mat::zeros(gradX.rows, gradX.cols, CV_32FC1);
	ComputeOrientation(gradX, gradY, orientation);

	for(int i = 0; i < mag.rows; i++)
	{
		for(int j = 0; j < mag.cols; j++)
		{
			float deg = orientation.at<float>(i, j);
			int bin = DegreeToBin(deg);
			hog.at(bin) += mag.at<float>(i, j);
		}
	}

	return hog;
}

std::vector<float> process_one_sample(Mat *smpl)
{
	int smpl_half_rows = smpl->rows / 2;
	int smpl_half_cols = smpl->cols / 2;
	Rect rect12 = Rect(0, smpl_half_cols, smpl_half_rows, smpl_half_cols);
	Mat clck12(*smpl, rect12);
	std::vector<float> hog12 = process_one_block(&clck12);

	Rect rect3 = Rect(smpl_half_rows, smpl_half_cols, smpl_half_rows, smpl_half_cols);
	Mat clck3(*smpl, rect3);
	std::vector<float> hog3 = process_one_block(&clck3);

	Rect rect6 = Rect(smpl_half_rows, 0, smpl_half_rows, smpl_half_cols);
	Mat clck6(*smpl, rect6);
	std::vector<float> hog6 = process_one_block(&clck6);

	Rect rect9 = Rect(0, 0, smpl_half_rows, smpl_half_cols);
	Mat clck9(*smpl, rect9);
	std::vector<float> hog9 = process_one_block(&clck9);

	std::vector<float> bighog;
	bighog.insert(bighog.end(), hog12.begin(), hog12.end());
	bighog.insert(bighog.end(), hog3.begin(), hog3.end());
	bighog.insert(bighog.end(), hog6.begin(), hog6.end());
	bighog.insert(bighog.end(), hog9.begin(), hog9.end());
	//for(int i = 0; i < bighog.size(); i++)
	//{
	//	std::cout << bighog.at(i)<<" ";
	//}
	//std::cout<<std::endl;
	return bighog;
}

float EuclideanDistance(std::vector<float> p, std::vector<float> q)
{
	float d = 0;
	for(int i = 0; i < p.size(); i++)
	{
		d += pow((p.at(i) - q.at(i)), 2);
	}
	return sqrt(d);
}

int VerifyOne(int index)
{
	float min = INT_MAX;
	int labelMin = -1;
	for(int i = 0; i < trainCells.size(); i++)
	{
		float current = EuclideanDistance(testHogs.at(index), trainHogs.at(i));
		if(current < min)
		{
			min = current;
			labelMin = trainLabels.at(i);
		}
	}
	return labelMin;
}

int VerifyAll()
{
	int count = 0;
	for(int i = 0; i < testCells.size(); i++)
	{
		int assumedLabel = VerifyOne(i);
		if(assumedLabel == testLabels.at(i))
		{
			count++;
			std::cout << "match" << std::endl;
		}
		std::cout << i << std::endl;
	}
	return count;
}

std::vector<std::vector<float>> calcHogs(std::vector<Mat> Cells)
{
	std::vector<std::vector<float>> hogs;
	for (int i = 0; i < Cells.size(); i++)
	{
		hogs.push_back(process_one_sample(&Cells.at(i)));
		std::cout << i << std::endl;
	}
	return hogs;
}

int main()
{
	loadMNIST();
	std::cout << "LOADED" << std::endl;
	trainHogs = calcHogs(trainCells);
	std::cout << "TRAINHOGS" << std::endl;
	testHogs = calcHogs(testCells);
	std::cout << "TESTHOGS" << std::endl;
	int count = VerifyAll();
	std::cout << count << std::endl;
	std::cout << ((double)count / testCells.size()) * 100 << "%" << std::endl;
	getchar();
}