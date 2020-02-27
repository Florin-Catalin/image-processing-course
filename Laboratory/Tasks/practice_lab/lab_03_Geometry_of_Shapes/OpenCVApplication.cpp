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

boolean i_func(Mat_<Vec3b> color_image, int row, int col, Vec3b color)
{
	if (color_image(row, col) == color)
	{
		return true;
	}
	else
	{
		return false;
	}
}

Mat_<uchar> filter_color(Mat_<Vec3b> color_image, Vec3b color)
{
	Mat_<uchar> filtered_image = Mat_<uchar>(color_image.rows, color_image.cols);
	for (int row = 0; row < color_image.rows; row++)
	{
		for (int col = 0; col < color_image.cols; col++)
		{
			boolean i_value = i_func(color_image, row, col, color);
			filtered_image(row, col) = i_value ? 1 : 0;
		}
	}
	return filtered_image;
}

int area(Mat_<uchar> image)
{
	int area = 0;
	for (int row = 0; row < image.rows; row++)
	{
		for(int col = 0; col < image.cols; col++)
		{
			area += image(row, col);
		}
	}
	return area;
}

Point center_of_mass(Mat_<uchar> image)
{
	int sum_row = 0;
	int sum_col = 0;
	for(int row = 0; row < image.rows; row++)
	{
		for(int col = 0; col < image.cols; col++)
		{
			sum_row += row * image(row, col);
			sum_col += col * image(row, col);
		}
	}
	int area_img = area(image);
	return Point(sum_col/area_img, sum_row/area_img);
}

double angle_of_elongation(Mat_<uchar> image, Point center_of_m)
{
	int nominator = 0;
	int c_denominator = 0;
	int r_denominator = 0;

	for(int row = 0; row < image.rows; row++)
	{
		for(int col = 0; col < image.cols; col++)
		{
			nominator += (row - center_of_m.y) * (col - center_of_m.x) * image(row, col);
			c_denominator += (col - center_of_m.x) * (col - center_of_m.x) * image(row, col);
			r_denominator += (row - center_of_m.y) * (row - center_of_m.y) * image(row, col);
		}
	}
	int Y = 2 * nominator;
	int X = c_denominator - r_denominator;

	
	double radian_angle = atan2(Y, X) / (double)2;
	if (radian_angle < 0)
	{
		radian_angle += CV_PI;
	}

	double degree_angle = (radian_angle * 180) / CV_PI;

	return degree_angle;
}

boolean is_on_edge(Mat_<uchar> image, int row, int col)
{
	if (image(row, col) == 0)
	{
		return false;
	}

	for(int col_off = -1; col_off <= 1; col_off++)
	{
		for (int row_off = -1; row_off <= 1; row_off++)
		{
			if (image(row + col_off, col + row_off) == 0)
			{
				return true;
			}
		}
	}
	return false;
}

double perimeter(Mat_<uchar> image)
{
	int perimeter_count = 0;
	for(int row = 1; row < image.rows - 1; row++)
	{
		for(int col = 1; col < image.cols - 1; col++)
		{
			if (is_on_edge(image, row, col))
			{
				perimeter_count++;
			}
		}
	}

	double corrected_perimeter = (perimeter_count * CV_PI) / 4;

	return corrected_perimeter;
}

double thinnes_ratio(double area_img, double perimeter_img)
{
	return (4 * CV_PI * area_img) / (perimeter_img * perimeter_img);
}

double aspect_ratio(Mat_<uchar> image, int* c_min_ret, int* c_max_ret)
{
	int c_min = image.cols;
	int c_max = 0;
	int r_min = image.rows;
	int r_max = 0;

	for(int row = 0; row < image.rows; row++)
	{
		for(int col = 0; col < image.cols; col++)
		{
			if (image(row, col) == 1)
			{
				c_min = min(c_min, col);
				c_max = max(c_max, col);
				r_min = min(r_min, row);
				r_max = max(r_max, row);
			}
		}
	}
	*c_min_ret = c_min;
	*c_max_ret = c_max;

	int numerator = c_max - c_min + 1;
	int denominator = r_max - r_min + 1;
	return (double)numerator / denominator;
}

int horiz_projection(Mat_<uchar> image, int row)
{
	int horiz_sum = 0;
	for(int col = 0; col < image.cols; col++)
	{
		horiz_sum += image(row, col);
	}
	return horiz_sum;
}

int vert_projection(Mat_<uchar> image, int col)
{
	int ver_sum = 0;
	for(int row = 0; row < image.rows; row++)
	{
		ver_sum += image(row, col);
	}
	return ver_sum;
}

void draw_axis_of_elongation(Mat_<Vec3b> src, int c_min, int c_max, Point ctr_mass, double angle)
{
	Mat_<Vec3b> dst = src.clone();
	double angle_radian = angle * CV_PI / 180;

	double tan_angle = tan(angle_radian);
	double r_a = ctr_mass.y + tan_angle * (c_min - ctr_mass.x);
	double c_a = c_min;

	double r_b = ctr_mass.y + tan_angle * (c_max - ctr_mass.x);
	double c_b = c_max;

	line(dst, Point(c_a, r_a), Point(c_b, r_b), Vec3b(0, 0, 0));

	imshow("elongation line", dst);
}

void draw_horizontal_proj(Mat_<uchar> image)
{
	Mat_<uchar> proj = Mat_<uchar>(image.rows, image.cols, 255);
	for(int row = 0; row < image.rows; row++)
	{
		int nr_pixel = horiz_projection(image, row);
		for(int col = 0; col < nr_pixel; col++)
		{
			proj(row, col) = 0;
		}
	}
	imshow("horizontal", proj);
}

void draw_vertical_proj(Mat_<uchar> image)
{
	Mat_<uchar> proj = Mat_<uchar>(image.rows, image.cols, 255);
	for(int col = 0; col < image.cols; col++)
	{
		int nr_pixel = vert_projection(image, col);
		for(int row = 0; row < nr_pixel; row++)
		{
			proj(row, col) = 0;
		}
	}
	imshow("vertical", proj);
}

void geometrical_features_computation(int event, int x, int y, int flags, void* param)
{
	Mat* src = (Mat*)param;
	if (event == CV_EVENT_LBUTTONDBLCLK)
	{
		Vec3b color = src->at<Vec3b>(y, x);
		printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
			x, y,
			(int)(*src).at<Vec3b>(y, x)[2],
			(int)(*src).at<Vec3b>(y, x)[1],
			(int)(*src).at<Vec3b>(y, x)[0]);

		Mat_<uchar> filtered_image = filter_color(*src, color);

		int img_area = area(filtered_image);
		printf("area: %d\n", img_area);

		Point ctr_mass = center_of_mass(filtered_image);
		printf("center of mass: %d, %d\n", ctr_mass.x, ctr_mass.y);

		double elongation = angle_of_elongation(filtered_image, ctr_mass);
		printf("elongation: %lf\n", elongation);

		double perimeter_img = perimeter(filtered_image);
		printf("perimeter: %lf\n", perimeter_img);

		double thinnes_img = thinnes_ratio(img_area, perimeter_img);
		printf("thinness: %lf\n", thinnes_img);

		int c_min;
		int c_max;
		double aspect_rat = aspect_ratio(filtered_image, &c_min, &c_max);
		printf("aspect ratio: %lf\n", aspect_rat);

		int horizontal_proj = horiz_projection(filtered_image, y);
		printf("horizontal proj: %d\n", horizontal_proj);
		int vertical_proj = vert_projection(filtered_image, x);
		printf("vertical proj: %d\n", vertical_proj);

		draw_axis_of_elongation(*src, c_min, c_max, ctr_mass, elongation);
		draw_horizontal_proj(filtered_image);
		draw_vertical_proj(filtered_image);
	}
}

void geometricalFeatures()
{
	// Read image from file 
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<Vec3b> src = imread(fname, CV_LOAD_IMAGE_COLOR);
		//Create a window
		namedWindow("My Window", 1);

		//set the callback function for any mouse event
		setMouseCallback("My Window", geometrical_features_computation, &src);

		//show the image
		imshow("My Window", src);

		// Wait until user press some key
		waitKey(0);
	}
	
}

int main()
{
	int op;
	do
	{
		system("cls");
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
		printf(" 10 - L4 - Geometrical features\n");
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
				geometricalFeatures();
				break;
		}
	}
	while (op!=0);
	return 0;
}
