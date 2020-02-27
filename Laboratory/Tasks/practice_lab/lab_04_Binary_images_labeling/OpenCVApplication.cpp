// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include <random>
#include <limits>


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

Point neighbour_8[] = {
	Point(1, 1),
	Point(1, 0),
	Point(1, -1),
	Point(0, 1),
	Point(0, -1),
	Point(-1, 1),
	Point(-1, 0),
	Point(-1, -1)
};

Point neighbour_p[] = {
	Point(-1, -1),
	Point(-1, 0),
	Point(0, -1),
	Point(1, -1)
};

void labelingEquivalence()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int label = 0;
		Mat_<int> labels = Mat_<int>(img.rows, img.cols, 0);
		std::vector<std::vector<int>> edges;
		for (int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				if (img(i, j) == 0 && labels(i, j) == 0)
				{
					std::vector<int> L;
					for (Point p : neighbour_p)
					{
						int n_i = i + p.y;
						int n_j = j + p.x;
						if (n_i < 0 || n_i >= img.rows)
						{
							continue;
						}
						if (n_j < 0 || n_j >= img.cols)
						{
							continue;
						}

						if (labels(n_i, n_j) > 0)
						{
							L.push_back(labels(n_i, n_j));
						}
					}
					if (L.size() == 0)
					{
						label++;
						labels(i, j) = label;
						edges.resize(label + 1);
					}
					else
					{
						int min_label = INT_MAX;
						// int min_label = min(L);
						for (int label_l : L)
						{
							if (min_label > label_l)
							{
								min_label = label_l;
							}
						}
						labels(i, j) = min_label;
						for (int label : L)
						{
							if (label != min_label)
							{
								edges[label].push_back(min_label);
								edges[min_label].push_back(label);
							}
						}
					}

				}
			}
		}
		
		int new_label = 0;
		int *new_labels = new int[label + 1];
		for(int i = 0; i < label + 1; i++)
		{
			new_labels[i] = 0;
		}
		for (int i = 0; i < label; i++)
		{
			if (new_labels[i] == 0)
			{
				new_label++;
				std::queue<int> Q;
				new_labels[i] = new_label;
				Q.push(i);
				while(!Q.empty())
				{
					int label_x = Q.front();
					Q.pop();
					for (int label_y : edges[label_x])
					{
						if (new_labels[label_y] == 0)
						{
							new_labels[label_y] = new_label;
							Q.push(label_y);
						}
					}
				}
			}
		}
		printf("label count: %d\n", label);
		printf("new label count: %d\n", new_label);

		std::default_random_engine gen;
		std::uniform_int_distribution<int> dist(0, 255);

		Vec3b *colors = new Vec3b[label + 1];
		for(int i = 1; i < label + 1; i++)
		{
			colors[i][0] = dist(gen);
			colors[i][1] = dist(gen);
			colors[i][2] = dist(gen);
		}
		colors[0][0] = 255;
		colors[0][1] = 255;
		colors[0][2] = 255;

		Mat_<Vec3b> labeled_img = Mat_<Vec3b>(img.rows, img.cols, Vec3b(0, 0, 0));

		for(int i = 0; i < img.rows; i++)
		{
			for (int j = 0; j < img.cols; j++)
			{
				int current_label = labels(i, j);
				int current_new_label = new_labels[current_label];
				labeled_img(i, j) = colors[current_new_label];
			}
		}

		delete[] colors;
		imshow("labeled", labeled_img);
		waitKey();

	}
}

void labelingBfs()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		int label = 0;
		Mat_<int> labels = Mat_<int>(img.rows, img.cols, 0);
		for(int i = 0; i < img.rows; i++)
		{
			for(int j = 0; j < img.cols; j++)
			{
				if (img(i, j) == 0 && labels(i, j) == 0)
				{
					label++;
					std::queue<Point> q = std::queue<Point>();
					q.push(Point(j, i));
					while(!q.empty())
					{
						Point p = q.front();
						q.pop();
						for (Point d : neighbour_8)
						{
							int n_i = p.y + d.y;
							int n_j = p.x + d.x;
							if (n_i < 0 || n_i >= img.rows)
							{
								continue;
							}
							if (n_j < 0 || n_j >= img.cols)
							{
								continue;
							}
							if (img(n_i, n_j) == 0 && labels(n_i, n_j) == 0)
							{
								labels(n_i, n_j) = label;
								q.push(Point(n_j, n_i));
							}
						}
					}
				}
			}
		}

		printf("label count: %d\n", label);

		std::default_random_engine gen;
		std::uniform_int_distribution<int> dist(0, 255);

		Vec3b *colors = new Vec3b[label + 1];
		for(int i = 1; i < label + 1; i++)
		{
			colors[i][0] = dist(gen);
			colors[i][1] = dist(gen);
			colors[i][2] = dist(gen);
		}
		colors[0][0] = 255;
		colors[0][1] = 255;
		colors[0][2] = 255;

		Mat_<Vec3b> labeled_img = Mat_<Vec3b>(img.rows, img.cols, Vec3b(0, 0, 0));
		for(int i = 0; i < labeled_img.rows; i++)
		{
			for(int j = 0; j < labeled_img.cols; j++)
			{
				labeled_img(i, j) = colors[labels(i, j)];
			}
		}

		delete[] colors;
		imshow("labeled", labeled_img);
		waitKey();
	}
}

int neigh_8_row[] = {
	0, -1, -1, -1, 0, 1, 1, 1
};

int neigh_8_col[] = {
	1, 1, 0, -1, -1, -1, 0, 1
};

int update_direction(int dir)
{
	if (dir % 2 == 1)
	{
		return (dir + 6) % 8;
	}
	else
	{
		return (dir + 7) % 8;
	}
}

Point updatePointByDir(Point current, int dir)
{
	Point dir_point = Point(current.x + neigh_8_col[dir], current.y + neigh_8_row[dir]);
	return dir_point;
}

std::vector<uchar> walkBorder(Point p0, Mat_<uchar> src, Mat_<uchar> dst)
{
	std::vector<uchar> ac;

	int n = 0;
	int dir = 7;
	boolean stop;
	Point p1;

	Point current_pos = p0;
	do
	{
		dir = update_direction(dir);

		// spin
		Point dir_point = updatePointByDir(current_pos, dir);
		while(src(dir_point.y, dir_point.x) != 0) 
		{
			dir = (dir + 1) % 8;
			dir_point = updatePointByDir(current_pos, dir);
		}

		dst(dir_point.y, dir_point.x) = 255;

		n++;
		if (n == 1)
		{
			p1 = dir_point;
		}
		ac.push_back(dir);

		Point pn_1 = current_pos;
		current_pos = dir_point;

		stop = n > 1 && dir_point == p1 && pn_1 == p0;
	} while (!stop);
	
	return ac;
}

std::vector<uchar> calculateDc(std::vector<uchar> ac)
{
	std::vector<uchar> dc;
	for(int i = 1; i < ac.size(); i++)
	{
		int aci_plus = ac.at(i);
		int aci = ac.at(i - 1);
		uchar dc_curr = (aci_plus - aci + 8) % 8;
		dc.push_back(dc_curr);
	}
	return dc;
}

void borderTracing()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);
		std::vector<uchar> ac;

		for (int row = 0; row < src.rows; row++)
		{
			for (int col = 0; col < src.cols; col++)
			{
				if (src(row, col) == 0)
				{
					Point p0 = Point(col, row);
					ac = walkBorder(p0, src, dst);
					col = src.cols;
					row = src.rows;
				}
			}
		}
		std::cout << "ac:";
		for (int dir : ac)
		{
			std::cout << dir << " ";
		}
		std::cout << std::endl;
		std::cout << "dc:";

		std::vector<uchar> dc = calculateDc(ac);

		for (int dir : dc)
		{
			std::cout << dir << " ";
		}
		std::cout << std::endl;

		imshow("labeled", dst);
		waitKey();
	}
	
}

void borderReconstruction()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		FILE* file = fopen(fname, "r");
		if (file == NULL)
		{
			return;
		}

		Mat_<uchar> dst = imread(
			R"(D:\git\TUCN_CS_3yr-2\IP\practice_lab\lab_04_Binary_images_labeling\Images\border\gray_background.bmp)", CV_LOAD_IMAGE_GRAYSCALE);

		Point currentPoint;
		fscanf(file, "%d %d", &currentPoint.y, &currentPoint.x);
		int ac_count;
		fscanf(file, "%d", &ac_count);
		for(int i = 0; i < ac_count; i++)
		{
			int dir;
			fscanf(file, "%d", &dir);
			currentPoint = updatePointByDir(currentPoint, dir);
			dst(currentPoint.y, currentPoint.x) = 0;
		}
		imshow("test", dst);
		waitKey();
	}
}

uchar cross[3][3] = {
	0, 1, 0,
	1, 1, 1,
	0, 1, 0
};

Mat_<uchar> dilation_core(Mat_<uchar> src)
{
	Mat_<uchar> dilated = Mat_<uchar>(src.rows, src.cols, (uchar)255);
	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			if (src(row, col) == 0)
			{
				for(int i = -1; i <= 1; i++)
				{
					for(int j = -1; j <= 1; j++)
					{
						if (cross[i + 1][j + 1] && 
							row + i >= 0 && col + j >= 0 &&
							row + i < src.rows && col + j < src.cols)
						{
							dilated(row + i, col + j) = 0;
						}
					}
				}
			}
		}
	}
	return dilated;
}

void dilation()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		printf("number of times to perform operation");
		int nr_times = 0;
		scanf("%d", &nr_times);

		Mat_<uchar> dilated = src.clone();
		for(int i = 0; i < nr_times; i++)
		{
			dilated = dilation_core(dilated);
		}
		imshow("original", src);
		imshow("dilated", dilated);
		waitKey();
	}
}

bool all_in_cross(Mat_<uchar> src, int row, int col)
{
	for(int i = -1; i <= 1; i++)
	{
		for(int j = -1; j <= 1; j++)
		{
			if(!(row + i >= 0 && col + j >= 0 &&
			   row + i < src.rows && col + j < src.cols))
			{
				return false;
			}

			if (cross[i + 1][j + 1])
			{
				if (src(row + i, col + j) != 0)
				{
					return false;
				}
			}
		}
	}
	
	return true;
}

Mat_<uchar> erosion_core(Mat_<uchar> src)
{
	Mat_<uchar> eroded = Mat_<uchar>(src.rows, src.cols, (uchar)255);
	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			if (src(row, col) == 0)
			{
				if (all_in_cross(src, row, col))
				{
					eroded(row, col) = 0;
				}
			}
		}
	}
	return eroded;
}

void erosion()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		printf("number of times to perform operation");
		int nr_times = 0;
		scanf("%d", &nr_times);

		
		Mat_<uchar> eroded = src.clone();
		for(int i = 0; i < nr_times; i++)
		{
			eroded = erosion_core(eroded);
		}

		imshow("original", src);
		imshow("eroded", eroded);
		waitKey();
	}
}

Mat_<uchar> opening_core(Mat_<uchar> src)
{
	Mat_<uchar> eroded = erosion_core(src);
	return dilation_core(eroded);
}

void opening()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		printf("number of times to perform operation");
		int nr_times = 0;
		scanf("%d", &nr_times);

		
		Mat_<uchar> opened = src.clone();
		for(int i = 0; i < nr_times; i++)
		{
			opened = opening_core(opened);
		}
		imshow("original", src);
		imshow("opening", opened);
		waitKey();
	}
}

Mat_<uchar> closing_core(Mat_<uchar> src)
{
	Mat_<uchar> dilated = dilation_core(src);
	return erosion_core(dilated);
}

void closing()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		printf("number of times to perform operation");
		int nr_times = 0;
		scanf("%d", &nr_times);

		
		Mat_<uchar> closed = src.clone();
		for(int i = 0; i < nr_times; i++)
		{
			closed = closing_core(closed);
		}
		imshow("closing", closed);
		imshow("original", src);
		waitKey();
	}
}

Mat_<uchar> complement(Mat_<uchar> src)
{
	Mat_<uchar> comp = src.clone();
	for(int i = 0; i < src.rows; i++)
	{
		for(int j = 0; j < src.cols; j++)
		{
			comp(i, j) = ~(src(i, j));
		}
	}
	return comp;
}

Mat_<uchar> difference(Mat_<uchar> a, Mat_<uchar> b)
{
	Mat_<uchar> diff = a.clone();
	for(int i = 0; i < a.rows; i++)
	{
		for(int j = 0; j < a.cols; j++)
		{
			if (b(i, j) == 0)
			{
				diff(i, j) = 255;
			}
		}
	}

	return diff;
}

Mat_<uchar> intersect(Mat_<uchar> a, Mat_<uchar> b)
{
	Mat_<uchar> inter = a.clone();
	for(int i = 0; i < a.rows; i++)
	{
		for(int j = 0; j < a.cols; j++)
		{
			if (a(i, j) == 0 && b(i, j) == 0)
			{
				inter(i, j) = 0;
			}
			else
			{
				inter(i, j) = 255;
			}
		}
	}

	return inter;
	
}

Mat_<uchar> boundary_core(Mat_<uchar> src)
{
	Mat_<uchar> eroded = erosion_core(src);
	return difference(src, eroded);

}

void boundaryExtraction()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		Mat_<uchar> boundary = boundary_core(src);
		imshow("original", src);
		imshow("boundary", boundary);
		waitKey();
	}
}

Mat_<uchar> region_filling_core(Mat_<uchar> src, int row, int col)
{
	bool no_change;
	Mat_<uchar> prev = Mat_<uchar>(src.rows, src.cols, (uchar)255);
	prev(row, col) = 0;
	Mat_<uchar> comp = complement(src);
	do
	{
		Mat_<uchar> dilated = dilation_core(prev);
		Mat_<uchar> curr = intersect(dilated, comp);
		
		no_change = countNonZero(prev != curr);
		prev = curr.clone();
	} while (no_change);

	return prev;
}

void regionFilling()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		Mat_<uchar> filled = region_filling_core(src, src.rows / 2, src.cols / 2);
		imshow("original", src);
		imshow("filled", filled);
		waitKey();
	}
}

int* calculateHistogram(Mat_<uchar> src)
{
	int* hist = new int[256];
	for(int i = 0; i < 256; i++)
	{
		hist[i] = 0;
	}

	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			uchar pixel = src(row, col);
			hist[pixel]++;
		}
	}

	return hist;
}

double* hist2pdf(int* histogram, const int hist_count, const int pixel_count)
{
	double* pdf = new double[hist_count];
	for(int i = 0; i < hist_count; i++)
	{
		pdf[i] = (double)histogram[i] / pixel_count;
	}
	return pdf;
}

double* pdf2cpdf(double* pdf, const int hist_count)
{
	double* cpdf = new double[hist_count];
	cpdf[0] = pdf[0];
	for(int i = 1; i < hist_count; i++)
	{
		cpdf[i] = cpdf[i - 1] + pdf[i];
	}
	return cpdf;
}

int* pdf2hist(double* pdf, const int hist_count, const int pixel_count)
{
	int* histogram = new int[hist_count];
	for(int i = 0; i < hist_count; i++)
	{
		histogram[i] = (int)(pdf[i] * pixel_count);
	}
	return histogram;
}

double calculateMean(int* hist, int start, int end)
{
	int sum = 0;
	int count = 0;
	for(int g = start; g < end; g++)
	{
		sum += g * hist[g];
		count += hist[g];
	}
	double mean = (double)sum / count;
	return mean;
}

double calculateStdDev(double* pdf, int start, int end, double mean)
{
	double sum = 0;
	for(int g = start; g < end; g++)
	{
		sum += (g - mean) * (g - mean) * pdf[g];
	}
	return sqrt(sum);
}

void showHist()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int* histogram = calculateHistogram(src);

		const int HIST_COUNT = 256;
		const int PIXEL_COUNT = src.cols * src.rows;

		double* pdf = hist2pdf(histogram, HIST_COUNT, PIXEL_COUNT);
		double* cpdf = pdf2cpdf(pdf, HIST_COUNT);
		int* cum_hist = pdf2hist(cpdf, HIST_COUNT, PIXEL_COUNT);

		double mean = calculateMean(histogram, 0, HIST_COUNT);
		std::cout << "mean: " << mean << std::endl;

		double std_dev = calculateStdDev(pdf, 0, HIST_COUNT, mean);
		std::cout << "std_dev: " << std_dev << std::endl;

		showHistogram("histogram", histogram, HIST_COUNT, 256);
		showHistogram("cum_hist", cum_hist, HIST_COUNT, 256);
		waitKey();
	}
	
}

int getMaxIntensity(int* hist, const int hist_count)
{
	for( int i = hist_count - 1; i >= 0; i--)
	{
		if(hist[i] > 0)
		{
			return i;
		}
	}
	return 0;
}

int getMinIntensity(int* hist, const int hist_count)
{
	for( int i = 0; i < hist_count; i++)
	{
		if(hist[i] > 0)
		{
			return i;
		}
	}
	return hist_count - 1;
}

int calculateBasicThreshold(Mat_<uchar> src)
{
	const int HIST_COUNT = 256;
	const int PIXEL_COUNT = src.cols * src.rows;

	int* histogram = calculateHistogram(src);
	int min_intensity = getMinIntensity(histogram, HIST_COUNT);
	int max_intensity = getMaxIntensity(histogram, HIST_COUNT);
	
	int curr_threshold = (min_intensity + max_intensity) / 2;
	double error;
	do
	{
		double mean_1 = calculateMean(histogram, 0, curr_threshold + 1);
		double mean_2 = calculateMean(histogram, curr_threshold, HIST_COUNT);
		double prev_threshold = curr_threshold;
		curr_threshold = (int)(mean_1 + mean_2) / 2;
		error = abs(curr_threshold - prev_threshold);
	} while (error > 0.1);

	return curr_threshold;
}

Mat_<uchar> thresholdImage(Mat_<uchar> src, uchar threshold)
{
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);
	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			if (src(row, col) > threshold)
			{
				dst(row, col) = 255;
			}
		}
	}

	return dst;
}

void basicThresholding()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		int threshold = calculateBasicThreshold(src);
		std::cout << "threshold: " << threshold;

		Mat_<uchar> thresholded = thresholdImage(src, threshold);

		imshow("original ", src);
		imshow("thresholded", thresholded);
	
		waitKey();
	}
}

uchar coercePixel(int pixel)
{
	if (pixel < 0)
	{
		return 0;
	}
	if (pixel > 255)
	{
		return 255;
	}
	return pixel;
	
}

uchar streachShrink(int g_in, int g_out_min, int g_out_max, int g_in_min, int g_in_max)
{
	double ratio = ((double)(g_out_max - g_out_min) / (g_in_max - g_in_min));
	int streachedShrunk = g_out_min + (int)((g_in - g_in_min) * ratio);

	return coercePixel(streachedShrunk);
}

Mat_<uchar> histogramStreachShirnkCore(Mat_<uchar> src, uchar g_min, uchar g_max)
{
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);
	int* hist = calculateHistogram(src);
	const int HIST_COUNT = 256;
	int g_in_min = getMinIntensity(hist, HIST_COUNT);
	int g_in_max = getMaxIntensity(hist, HIST_COUNT);
	
	for(int row = 0; row < src.rows; row++)
	{
		for (int col = 0; col < src.cols; col++)
		{
			dst(row, col) = streachShrink(src(row, col), g_min, g_max, g_in_min, g_in_max);
		}
	}
	return dst;
}

void histogramStreachShirnk()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		std::cout << "enter the min:";
		int g_min;
		std::cin >> g_min;

		std::cout << "enter the max:";
		int g_max;
		std::cin >> g_max;

		Mat_<uchar> changed = histogramStreachShirnkCore(src, g_min, g_max);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> brightnessChangeCore(Mat_<uchar> src, int offset)
{
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);
	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			int offsetedPixel = src(row, col) + offset;
			dst(row, col) = coercePixel(offsetedPixel);
		}
	}
	
	return dst;
}

void brightnessChange()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		std::cout << "enter the offset:";
		int offset;
		std::cin >> offset;

		Mat_<uchar> changed = brightnessChangeCore(src, offset);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> gammaCorrectionCore(Mat_<uchar> src, double gamma)
{
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);
	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			double normalized = (double)src(row, col) / 256;
			int gamma_corrected_pixel = (int)(pow(normalized, gamma) * 256);
			dst(row, col) = coercePixel(gamma_corrected_pixel);
		}
	}
	
	return dst;
}

void gammaCorrection()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		std::cout << "enter the gamma:";
		double gamma;
		std::cin >> gamma;

		Mat_<uchar> changed = gammaCorrectionCore(src, gamma);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> histogramEqualizationCore(Mat_<uchar> src)
{
	int* histogram = calculateHistogram(src);
	const int HIST_COUNT = 256;
	double* pdf = hist2pdf(histogram, 256, src.rows * src.cols);
	double* cpdf = pdf2cpdf(pdf, HIST_COUNT);
	
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);
	for (int row = 0; row < src.rows; row++)
	{
		for (int col = 0; col < src.cols; col++) 
		{
			uchar g_in = src(row, col);
			int g_out = (int)(255 * cpdf[g_in]);
			dst(row, col) = coercePixel(g_out);
		}
	}

	return dst;
}

void histogramEqualization()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = histogramEqualizationCore(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

float calculateConvolutionPixel_no_saturate(Mat_<uchar> src, int row, int col, Mat_<float> kernel, float normalizer)
{
	int kernelRowOffset = kernel.rows / 2;
	int kernelColOffset = kernel.cols / 2;

	float convolution = 0;
	for(int kernel_row = 0; kernel_row < kernel.rows ; kernel_row++)
	{
		for(int kernel_col = 0; kernel_col < kernel.cols; kernel_col++)
		{
			int pixel_row = row + kernel_row - kernelRowOffset;
			int pixel_col = col + kernel_col - kernelColOffset;
			uchar current_pixel = src(pixel_row, pixel_col);
			convolution += current_pixel * kernel(kernel_row, kernel_col);
		}
	}

	float normalized_conv = convolution / normalizer;

	return normalized_conv;
}

uchar calculateConvolutionPixel(Mat_<uchar> src, int row, int col, Mat_<float> kernel, float normalizer)
{
	float normalized_conv = calculateConvolutionPixel_no_saturate(src, row, col, kernel, normalizer);

	return saturate_cast<uchar>(normalized_conv);
}

Mat_<uchar> applyConvolution(Mat_<uchar> src, Mat_<float> kernel, float normalizer)
{
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);

	int kernelRowOffset = kernel.rows / 2;
	int kernelColOffset = kernel.cols / 2;

	for(int row = kernelRowOffset; row < src.rows - kernelRowOffset; row++)
	{
		for(int col = kernelColOffset; col < src.cols - kernelColOffset; col++)
		{
			dst(row, col) = calculateConvolutionPixel(src, row, col, kernel, normalizer);
		}
	}

	return dst;
}

Mat_<float> applyConvolution_no_saturate(Mat_<uchar> src, Mat_<float> kernel, float normalizer)
{
	Mat_<float> dst = Mat_<float>(src.rows, src.cols, (float)0);

	int kernelRowOffset = kernel.rows / 2;
	int kernelColOffset = kernel.cols / 2;

	for(int row = kernelRowOffset; row < src.rows - kernelRowOffset; row++)
	{
		for(int col = kernelColOffset; col < src.cols - kernelColOffset; col++)
		{
			dst(row, col) = calculateConvolutionPixel_no_saturate(src, row, col, kernel, normalizer);
		}
	}

	return dst;
}

Mat_<uchar> meanFilter3x3Core(Mat_<uchar> src)
{
	Mat_<int> kernel = Mat_<int>(3, 3, 1);
	return applyConvolution(src, kernel, 9);
}

void meanFilter3x3()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = meanFilter3x3Core(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> meanFilter5x5Core(Mat_<uchar> src)
{
	Mat_<int> kernel = Mat_<int>(5, 5, 1);
	return applyConvolution(src, kernel, 25);
}

void meanFilter5x5()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = meanFilter5x5Core(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> gaussianFilterCore(Mat_<uchar> src)
{
	char kerneldata[] = {
		1, 2, 1,
		2, 4, 2,
		1, 2, 1 
	};
	Mat_<char> kernel = Mat_<char>(3, 3, kerneldata);
	return applyConvolution(src, kernel, 16);
}

void gaussianFilter()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = gaussianFilterCore(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> laplaceFilterCore(Mat_<uchar> src)
{
	char kerneldata[] = {
		 0, -1,  0,
		-1,  4, -1,
		 0, -1,  0 
	};
	Mat_<char> kernel = Mat_<char>(3, 3, kerneldata);
	return applyConvolution(src, kernel, 1);
}

void laplaceFilter()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = laplaceFilterCore(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> highPassFilterCore(Mat_<uchar> src)
{
	int kerneldata[] = {
		 0, -1,  0,
		-1,  5, -1,
		 0, -1,  0 
	};
	Mat_<int> kernel = Mat_<int>(3, 3, kerneldata);
	return applyConvolution(src, kernel, 1);
}

void highPassFilter()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = highPassFilterCore(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<float> centerOperation(Mat_<float> src)
{
	Mat_<float> centered = Mat_<float>(src.rows, src.cols);
	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			bool even = (row + col) % 2 == 0;
			centered(row, col) = (even) ? src(row, col) : -src(row, col);
		}
	}

	return centered;
}


Mat_<uchar> magnitudeLogOfCenteredFourierCore(Mat_<uchar> src)
{
	Mat_<float> srcf;
	src.convertTo(srcf, CV_32FC1);

	Mat_<float> centered = centerOperation(srcf);

	Mat fourier;
	dft(centered, fourier, DFT_COMPLEX_OUTPUT);

	Mat channels[] = {
		Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F)
	};

	split(fourier, channels);

	Mat mag, phi;
	magnitude(channels[0], channels[1], mag);

	for(int row = 0; row < mag.rows; row++)
	{
		for(int col = 0; col < mag.cols; col++)
		{
			mag.at<float>(row, col) =
				log(mag.at<float>(row, col) + 1);
		}
	}

	Mat_<uchar> dst;
	normalize(mag, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;
}

void magnitudeLogOfCenteredFourier()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);

		Mat_<uchar> changed = magnitudeLogOfCenteredFourierCore(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> idealLowPassFilterCore(Mat_<uchar> src, float R)
{
	Mat_<float> srcf;
	src.convertTo(srcf, CV_32FC1);

	Mat_<float> centered = centerOperation(srcf);

	Mat fourier;
	dft(centered, fourier, DFT_COMPLEX_OUTPUT);

	Mat channels[] = {
		Mat::zeros(src.size(), CV_32FC1), Mat::zeros(src.size(), CV_32FC1)
	};
	split(fourier, channels);

	Mat mag = channels[0];
	Mat phi = channels[1];

	for(int row = 0; row < mag.rows; row++)
	{
		for(int col = 0; col < mag.cols; col++)
		{
			float dist =
				  pow((float)mag.rows / 2 - row, 2)
				+ pow((float)mag.cols / 2 - col, 2);
			if (dist > R * R)
			{
				mag.at<float>(row, col) = 0;
				phi.at<float>(row, col) = 0;
			}
		}
	}

	channels[0] = mag;
	channels[1] = phi;

	Mat dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	Mat_<float> uncentered = centerOperation(dstf);

	Mat_<uchar> dst;
	normalize(uncentered, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;
}

void idealLowPassFilter()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		float R;

		printf("enter R:\n");
		scanf("%f", &R);

		Mat_<uchar> changed = idealLowPassFilterCore(src, R);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> idealHighPassFilterCore(Mat_<uchar> src, float R)
{
	Mat_<float> srcf;
	src.convertTo(srcf, CV_32FC1);

	Mat_<float> centered = centerOperation(srcf);

	Mat fourier;
	dft(centered, fourier, DFT_COMPLEX_OUTPUT);

	Mat channels[] = {
		Mat::zeros(src.size(), CV_32FC1), Mat::zeros(src.size(), CV_32FC1)
	};
	split(fourier, channels);

	Mat mag = channels[0];
	Mat phi = channels[1];

	for(int row = 0; row < mag.rows; row++)
	{
		for(int col = 0; col < mag.cols; col++)
		{
			float dist =
				  pow((float)mag.rows / 2 - row, 2)
				+ pow((float)mag.cols / 2 - col, 2);
			if (dist < R * R)
			{
				mag.at<float>(row, col) = 0;
				phi.at<float>(row, col) = 0;
			}
		}
	}

	channels[0] = mag;
	channels[1] = phi;

	Mat dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	Mat_<float> uncentered = centerOperation(dstf);

	Mat_<uchar> dst;
	normalize(uncentered, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;
}

void idealHighPassFilter()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		float R;

		printf("enter R:\n");
		scanf("%f", &R);

		Mat_<uchar> changed = idealHighPassFilterCore(src, R);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> gaussianCutLPFCore(Mat_<uchar> src, float A)
{
	Mat_<float> srcf;
	src.convertTo(srcf, CV_32FC1);

	Mat_<float> centered = centerOperation(srcf);

	Mat fourier;
	dft(centered, fourier, DFT_COMPLEX_OUTPUT);

	Mat channels[] = {
		Mat::zeros(src.size(), CV_32FC1), Mat::zeros(src.size(), CV_32FC1)
	};
	split(fourier, channels);

	Mat mag = channels[0];
	Mat phi = channels[1];

	for(int row = 0; row < mag.rows; row++)
	{
		for(int col = 0; col < mag.cols; col++)
		{
			float dist =
				  pow((float)mag.rows / 2 - row, 2)
				+ pow((float)mag.cols / 2 - col, 2);
			float coeff = exp(- dist / pow(A, 2));
			mag.at<float>(row, col) = mag.at<float>(row, col) * coeff;
			phi.at<float>(row, col) = phi.at<float>(row, col) * coeff;
		}
	}

	channels[0] = mag;
	channels[1] = phi;

	Mat dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	Mat_<float> uncentered = centerOperation(dstf);

	Mat_<uchar> dst;
	normalize(uncentered, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;
}

void gaussianCutLPF()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		float A;

		printf("enter A:\n");
		scanf("%f", &A);

		Mat_<uchar> changed = gaussianCutLPFCore(src, A);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<uchar> gaussianCutHPFCore(Mat_<uchar> src, float A)
{
	Mat_<float> srcf;
	src.convertTo(srcf, CV_32FC1);

	Mat_<float> centered = centerOperation(srcf);

	Mat fourier;
	dft(centered, fourier, DFT_COMPLEX_OUTPUT);

	Mat channels[] = {
		Mat::zeros(src.size(), CV_32FC1), Mat::zeros(src.size(), CV_32FC1)
	};
	split(fourier, channels);

	Mat mag = channels[0];
	Mat phi = channels[1];

	for(int row = 0; row < mag.rows; row++)
	{
		for(int col = 0; col < mag.cols; col++)
		{
			float dist =
				  pow((float)mag.rows / 2 - row, 2)
				+ pow((float)mag.cols / 2 - col, 2);
			float coeff = exp(- dist / pow(A, 2));
			mag.at<float>(row, col) = mag.at<float>(row, col) * (1 - coeff);
			phi.at<float>(row, col) = phi.at<float>(row, col) * (1 - coeff);
		}
	}

	channels[0] = mag;
	channels[1] = phi;

	Mat dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	Mat_<float> uncentered = centerOperation(dstf);

	Mat_<uchar> dst;
	normalize(uncentered, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;
}

void gaussianCutHPF()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		float A;

		printf("enter A:\n");
		scanf("%f", &A);

		Mat_<uchar> changed = gaussianCutHPFCore(src, A);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

uchar calculateMeanFilterPixel(const Mat_<uchar> src, int row, int col, int w)
{
	int w_half = w / 2;
	std::vector<uchar> val;
	for(int median_row = 0; median_row < w; median_row++)
	{
		for(int meadian_col = 0; meadian_col < w; meadian_col++)
		{
			int pixel_row = row + median_row - w_half;
			int pixel_col = col + meadian_col - w_half;
			uchar current_pixel = src(pixel_row, pixel_col);
			val.push_back(current_pixel);
		}
	}

	std::sort(val.begin(), val.end());
	return val[(w * w) / 2];
}

Mat_<uchar> meanFilterSaltAndPepperCore(Mat_<uchar> src, int w)
{
	Mat_<uchar> dst = Mat_<uchar>(src.rows, src.cols, (uchar)0);

	int w_half = w / 2;

	for(int row = w_half; row < src.rows - w_half; row++)
	{
		for(int col = w_half; col < src.cols - w_half; col++)
		{
			dst(row, col) = calculateMeanFilterPixel(src, row, col, w);
		}
	}

	return dst;
}

void meanFilterSaltAndPepper()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		int w;

		printf("enter w:\n");
		scanf("%d", &w);

		Mat_<uchar> changed = meanFilterSaltAndPepperCore(src, w);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<float> createGaussianKernel(int w, float sig)
{
	int half_w = w / 2;;
	Mat_<float> kernel = Mat_<float>(w, w);

	for(int row = 0; row < w; row++)
	{
		for(int col = 0; col < w; col++)
		{
			float power = -(pow(row - half_w, 2) + pow(col - half_w, 2));
			power /= 2 * pow(sig, 2);
			kernel(row, col) = exp(power);
			kernel(row, col) /= 2 * PI * pow(sig, 2);
		}
	}

	return kernel;
}

Mat_<uchar> gaussianFilter1x2DCore(Mat_<uchar> src, float sig)
{
	int w = round(6 * sig);

	// if w is even add one (to make it odd)
	w = (w % 2 == 0) ? w + 1 : w;

	Mat_<float> kernel = createGaussianKernel(w, sig);

	float sum_kernel = sum(kernel)[0];

	Mat_<uchar> dst = applyConvolution(src, kernel, sum_kernel);

	return dst;
}

void gaussianFilter1x2D()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		float sig;

		printf("enter sig:\n");
		scanf("%f", &sig);

		double t = (double)getTickCount();
		Mat_<uchar> changed = gaussianFilter1x2DCore(src, sig);
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

Mat_<float> createGaussianKernelX(int w, float sig)
{
	int half_w = w / 2;;
	Mat_<float> kernel = Mat_<float>(w, 1);

	for(int row = 0; row < w; row++)
	{
			float power = -(pow(row - half_w, 2));
			power /= 2 * pow(sig, 2);
			kernel(row, 0) = exp(power);
			kernel(row, 0) /= sqrt(2 * PI) * sig;
	}

	return kernel;
}

Mat_<float> createGaussianKernelY(int w, float sig)
{
	int half_w = w / 2;;
	Mat_<float> kernel = Mat_<float>(1, w);

	for(int col = 0; col < w; col++)
	{
			float power = -(pow(col - half_w, 2));
			power /= 2 * pow(sig, 2);
			kernel(0, col) = exp(power);
			kernel(0, col) /= sqrt(2 * PI) * sig;
	}

	return kernel;
}

Mat_<uchar> gaussianFilter2x1DCore(Mat_<uchar> src, float sig)
{
	int w = round(6 * sig);

	// if w is even add one (to make it odd)
	w = (w % 2 == 0) ? w + 1 : w;

	Mat_<float> kernelx = createGaussianKernelX(w, sig);

	float sum_kernel = sum(kernelx)[0];

	Mat_<uchar> inter = applyConvolution(src, kernelx, sum_kernel);

	Mat_<float> kernely = createGaussianKernelY(w, sig);

	sum_kernel = sum(kernely)[0];

	Mat_<uchar> dst = applyConvolution(inter, kernely, sum_kernel);

	return dst;
}

void gaussianFilter2x1D()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		float sig;

		printf("enter sig:\n");
		scanf("%f", &sig);

		double t = (double)getTickCount();
		Mat_<uchar> changed = gaussianFilter2x1DCore(src, sig);
		t = ((double)getTickCount() - t) / getTickFrequency();
		printf("Time = %.3f [ms]\n", t * 1000);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
	}
}

int getArea(float phase)
{
	if (phase > PI)
	{
		phase -= 2 * PI;
	}
	if (phase < PI/8 && phase > -PI/8)
	{
		return 2;
	}

	if (phase > 7*PI/8 || phase < -7*PI/8)
	{
		return 2;
	}

	if (phase < 3*PI/8 && phase > PI/8)
	{
		return 1;
	}

	if (phase < -5*PI/8 && phase > -7*PI/8)
	{
		return 1;
	}

	if (phase < 5*PI/8 && phase > 3*PI/8)
	{
		return 0;
	}

	if (phase < -3*PI/8 && phase > -5*PI/8)
	{
		return 0;
	}

	if (phase < 7*PI/8 && phase > 5*PI/8)
	{
		return 3;
	}

	if (phase < -1*PI/8 && phase > -3*PI/8)
	{
		return 3;
	}

	return 0;
}

float calculateNonMaximaPixel(const Mat_<float>& magnitudes, int row, int col, int area)
{
	float current = magnitudes(row, col);
	float left = 0;
	float right = 0;
	switch(area)
	{
		case 0:
			left = magnitudes(row - 1, col);
			right = magnitudes(row + 1, col);
			break;
		case 1:
			left = magnitudes(row + 1, col - 1);
			right = magnitudes(row - 1, col + 1);
			break;
		case 2:
			left = magnitudes(row, col - 1);
			right = magnitudes(row, col + 1);
			break;
		case 3:
			left = magnitudes(row - 1, col - 1);
			right = magnitudes(row + 1, col + 1);
			break;
		default:
			exit(0);
	}

	if (current < left || current < right)
	{
		return 0;
	}

	return current;
}

Mat_<float> nonMaximaSuppression(Mat_<float> magnitudes, Mat_<float> phases)
{
	Mat_<float> nonMaxima = Mat_<float>(magnitudes.rows, magnitudes.cols, (float)0);

	for(int row = 1; row < magnitudes.rows - 1; row++)
	{
		for(int col = 1; col < magnitudes.cols - 1; col++)
		{
			float phase = phases(row, col);
			int area = getArea(phase);
			nonMaxima(row, col) = calculateNonMaximaPixel(magnitudes, row, col, area);
		}
	}

	return nonMaxima;
}

Mat_<uchar> doWeakEdgeLinking(Mat_<uchar> src)
{

	Mat_<uchar> img = src.clone();

	for(int i = 0; i < img.rows; i++)
	{
		for(int j = 0; j < img.cols; j++)
		{
			if (img(i, j) == 255)
			{
				std::queue<Point> q = std::queue<Point>();
				q.push(Point(j, i));
				while(!q.empty())
				{
					Point p = q.front();
					q.pop();
					for (Point d : neighbour_8)
					{
						int n_i = p.y + d.y;
						int n_j = p.x + d.x;
						if (n_i < 0 || n_i >= img.rows)
						{
							continue;
						}
						if (n_j < 0 || n_j >= img.cols)
						{
							continue;
						}
						if (img(n_i, n_j) == 128)
						{
							img(n_i, n_j) = 255;
							q.push(Point(n_j, n_i));
						}
					}
				}
			}
		}
	}

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			if (img(i, j) != 255)
			{
				img(i, j) = 0;
			}
		}
	}

	return img;
}

Mat_<uchar> cannyEdgeDetectionCore(Mat_<uchar> src, double p = 0.1, double k = 0.4)
{
	Mat_<uchar> gaussian = gaussianFilter1x2DCore(src, 0.8);

	Mat_<float> kernel_x = Mat_<float>(3, 3);
	kernel_x(0, 0) = kernel_x(2, 0) = -1;
	kernel_x(1, 0) = -2;

	kernel_x(0, 1) = kernel_x(1, 1) = kernel_x(2, 1) = 0;

	kernel_x(0, 2) = kernel_x(2, 2) = 1;
	kernel_x(1, 2) = 2;

	Mat_<float> GX = applyConvolution_no_saturate(gaussian, kernel_x, 1);

	Mat_<float> kernel_y = Mat_<float>(3, 3);
	kernel_y(0, 0) = kernel_y(0, 2) = 1;
	kernel_y(0, 1) = 2;

	kernel_y(1, 0) = kernel_y(1, 1) = kernel_y(1, 2) = 0;

	kernel_y(2, 0) = kernel_y(2, 2) = -1;
	kernel_y(2, 1) = -2;

	Mat_<float> GY = applyConvolution_no_saturate(gaussian, kernel_y, 1);

	//magnitude
	Mat_<float> magnitudes;
	magnitude(GX, GY, magnitudes);

	//phase
	Mat_<float> phases;
	phase(GX, GY, phases);

	// normalize
	Mat_<float> normalized_mag;
	normalize(magnitudes, normalized_mag, 0, 255, NORM_MINMAX, CV_32FC1);

	Mat_<float> suppressed = nonMaximaSuppression(normalized_mag, phases);

	Mat_<uchar> converted;
	suppressed.convertTo(converted, CV_8UC1);

	int* hist = calculateHistogram(converted);

	int noEdgePixels = (int)(p * (src.rows * src.cols - hist[0]));

	int sum = 0;
	int highThreshold = 0;
	for (int i = 255; i >= 0; i--)
	{
		sum += hist[i];

		if (sum > noEdgePixels)
		{
			highThreshold = i;
			break;
		}
	}

	printf("high threshold: %d", highThreshold);

	int lowThreshold = k * highThreshold;

	Mat_<uchar> strongThreshold = converted.clone();

	for(int row = 0; row < src.rows; row++)
	{
		for(int col = 0; col < src.cols; col++)
		{
			uchar pixel = converted(row, col);
			if (pixel > highThreshold)
			{
				strongThreshold(row, col) = 255;
			}else if (pixel < lowThreshold)
			{
				strongThreshold(row, col) = 0;
			}else
			{
				strongThreshold(row, col) = 128;
			}
		}
	}


	Mat_<uchar> connectedComp = doWeakEdgeLinking(strongThreshold);

	return connectedComp;
}

void cannyEdgeDetection()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, IMREAD_GRAYSCALE);
		Mat_<uchar> changed = cannyEdgeDetectionCore(src);

		imshow("original ", src);
		imshow("changed", changed);
	
		waitKey();
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
		printf(" 11 - L5 - Labeling (BFS)\n");
		printf(" 12 - L5 - Labeling (equivalence classes)\n");
		printf(" 13 - L6 - Border tracing alg\n");
		printf(" 14 - L6 - Border reconstruction\n");
		printf(" 15 - L7 - Dilation\n");
		printf(" 16 - L7 - Erosion\n");
		printf(" 17 - L7 - Opening\n");
		printf(" 18 - L7 - Closing\n");
		printf(" 19 - L7 - Boundary extraction\n");
		printf(" 20 - L7 - Region filling\n");
		printf(" 21 - L8 - Show histogram\n");
		printf(" 22 - L8 - Basic thresholding\n");
		printf(" 23 - L8 - Histogram stretch shrink\n");
		printf(" 24 - L8 - Brightness change\n");
		printf(" 25 - L8 - gamma correction\n");
		printf(" 26 - L8 - equalization of histogram\n");

		printf(" 27 - L9 - Mean filter 3x3\n");
		printf(" 28 - L9 - Mean filter 5x5\n");
		printf(" 29 - L9 - Gaussian filter\n");
		printf(" 30 - L9 - Laplace filter\n");
		printf(" 31 - L9 - High-pass filter\n");
		printf(" 32 - L9 - Magnitude log of the Fourier spectrum\n");
		printf(" 33 - L9 - Ideal low-pass filter\n");
		printf(" 34 - L9 - Idea high-pass filter\n");
		printf(" 35 - L9 - Gaussian-cut LPF\n");
		printf(" 36 - L9 - Gaussian-cut HPF\n");

		printf(" 37 - L10 - Median filter (w - variable)\n");
		printf(" 38 - L10 - Gaussian filter (1x2D - sig-variable)\n");
		printf(" 39 - L10 - Gaussian filter (2x1D - sig-variable)\n");

		printf(" 40 - L11 - Canny edge detection\n");

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
			case 11:
				labelingBfs();
				break;
			case 12:
				labelingEquivalence();
				break;
			case 13:
				borderTracing();
				break;
			case 14:
				borderReconstruction();
				break;
			case 15:
				dilation();
				break;
			case 16:
				erosion();
				break;
			case 17:
				opening();
				break;
			case 18:
				closing();
				break;
			case 19:
				boundaryExtraction();
				break;
			case 20:
				regionFilling();
				break;
			case 21:
				showHist();
				break;
			case 22:
				basicThresholding();
				break;
			case 23:
				histogramStreachShirnk();
				break;
			case 24:
				brightnessChange();
				break;
			case 25:
				gammaCorrection();
				break;
			case 26:
				histogramEqualization();
				break;
			case 27:
				meanFilter3x3();
				break;
			case 28:
				meanFilter5x5();
				break;
			case 29:
				gaussianFilter();
				break;
			case 30:
				laplaceFilter();
				break;
			case 31:
				highPassFilter();
				break;
			case 32:
				magnitudeLogOfCenteredFourier();
				break;
			case 33:
				idealLowPassFilter();
				break;
			case 34:
				idealHighPassFilter();
				break;
			case 35:
				gaussianCutLPF();
				break;
			case 36:
				gaussianCutHPF();
				break;
			case 37:
				meanFilterSaltAndPepper();
				break;
			case 38:
				gaussianFilter1x2D();
				break;
			case 39:
				gaussianFilter2x1D();
				break;
			case 40:
				// gaussian filtering
				// gradient magnitude & orientation
				// Non-Maxima suppression

				// Adaptive thresholding + edge linking
				cannyEdgeDetection();
				break;
		}
	}
	while (op!=0);
	return 0;
}
