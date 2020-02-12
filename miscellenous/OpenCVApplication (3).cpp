#include "stdafx.h"
#include "common.h"
#include <math.h>
#include <queue>
#include <random>
#include <vector>


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

	void testNegativeImage()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			double t = (double)getTickCount(); // Get the current time [s]

			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			int height = src.rows;
			int width = src.cols;
			Mat dst = Mat(height, width, CV_8UC1);
			// Asa se acceseaaza pixelii individuali pt. o imagine cu 8 biti/pixel
			// Varianta ineficienta (lenta)
			for (int i = 0; i<height; i++)
			{
				for (int j = 0; j<width; j++)
				{
					uchar val = src.at<uchar>(i, j);
					uchar neg = 255 - val;
					dst.at<uchar>(i, j) = neg;
				}
			}

			// Get the current time again and compute the time difference [s]
			t = ((double)getTickCount() - t) / getTickFrequency();
			// Print (in the console window) the processing time in [ms] 
			printf("Time = %.3f [ms]\n", t * 1000);

			imshow("input image", src);
			imshow("negative image", dst);
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
			int w = (int)src.step; // no dword alignment is done !!!
			for (int i = 0; i<height; i++)
				for (int j = 0; j < width; j++) {
					uchar val = lpSrc[i*w + j];
					lpDst[i*w + j] = 255 - val;
				}

			// Get the current time again and compute the time difference [s]
			t = ((double)getTickCount() - t) / getTickFrequency();
			// Print (in the console window) the processing time in [ms] 
			printf("Time = %.3f [ms]\n", t * 1000);

			imshow("input image", src);
			imshow("negative image", dst);
			waitKey();
		}
	}

	void testColor2Gray()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname);

			int height = src.rows;
			int width = src.cols;

			Mat dst = Mat(height, width, CV_8UC1);

			// Asa se acceseaaza pixelii individuali pt. o imagine RGB 24 biti/pixel
			// Varianta ineficienta (lenta)
			for (int i = 0; i<height; i++)
			{
				for (int j = 0; j<width; j++)
				{
					Vec3b v3 = src.at<Vec3b>(i, j);
					uchar b = v3[0];
					uchar g = v3[1];
					uchar r = v3[2];
					dst.at<uchar>(i, j) = (r + g + b) / 3;
				}
			}

			imshow("input image", src);
			imshow("gray image", dst);
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
		while (openFileDlg(fname))
		{
			Mat src;
			src = imread(fname);
			Mat dst1, dst2;
			//without interpolation
			resizeImg(src, dst1, 320, false);
			//with interpolation
			resizeImg(src, dst2, 320, true);
			imshow("input image", src);
			imshow("resized image (without interpolation)", dst1);
			imshow("resized image (with interpolation)", dst2);
			waitKey();
		}
	}

	void testCanny()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src, dst, gauss;
			src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			double k = 0.4;
			int pH = 50;
			int pL = (int)k*pH;
			GaussianBlur(src, gauss, Size(5, 5), 0.8, 0.8);
			Canny(gauss, dst, pL, pH, 3);
			imshow("input image", src);
			imshow("canny", dst);
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
			Canny(grayFrame, edges, 40, 100, 3);
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

	void CalculProprietatiGeometrice(int event, int x, int y, int flags, void* param)
	{
		Mat* src = (Mat*)param;
		Mat axa = (*src).clone();
		Mat orizontal = (*src).clone();
		Mat vertical = (*src).clone();
		Mat contur = (*src).clone();

		Vec3b culoare = (*src).at<Vec3b>(y, x);

		int arie = 0, r = 0, c = 0, cr = 0, cc = 0, v = 0, w = 0, NP = 0;
		int cmax = 0, cmin = (*src).cols, rmax = 0, rmin = (*src).rows;
		float unghi = 0, unghiR = 0, perimetru = 0, T = 0, R = 0;
		int contCol = 0, contLinie = 0;

		if (event == CV_EVENT_LBUTTONDBLCLK)
		{

			printf("Pos(x,y): %d,%d  Color(RGB): %d,%d,%d\n",
				x, y,
				(int)(*src).at<Vec3b>(y, x)[2],
				(int)(*src).at<Vec3b>(y, x)[1],
				(int)(*src).at<Vec3b>(y, x)[0]);

			for (int i = 0; i < (*src).rows; i++)
				for (int j = 0; j < (*src).cols; j++)
				{
					if ((*src).at<Vec3b>(i, j) == culoare)
					{
						arie++;
						r += i;
						c += j;
						if (cmax < j)
							cmax = j;
						if (cmin > j)
							cmin = j;
						if (rmax < i)
							rmax = i;
						if (rmin > i)
							rmin = i;

					}

				}
			printf("Aria este : %d\n", arie);

			cr = r / arie;
			cc = c / arie;
			printf("r: %d si c: %d\n", cr, cc);

			for (int i = 0; i < (*src).rows; i++)
				for (int j = 0; j < (*src).cols; j++)
				{
					if ((*src).at<Vec3b>(i, j) == culoare)
					{
						v += (i - cr)*(j - cc);
						w += (j - cc)*(j - cc) - (i - cr)*(i - cr);
					}
				}
			v *= 2;
			unghi = atan2(v, w) / 2;
			unghiR = unghi;
			if (unghi < 0)
				unghi += CV_PI;
			unghi = unghi * 180 / CV_PI;
			printf("Axa alungire: %f\n", unghi);

			for (int i = 0; i < (*src).rows; i++)
				for (int j = 0; j < (*src).cols; j++)
				{
					if ((*src).at<Vec3b>(i, j) == culoare &&
						(i == 0 || i == (*src).rows - 1 || j == 0 || j == (*src).cols - 1 ||
						(*src).at<Vec3b>(i - 1, j - 1) != culoare ||
						(*src).at<Vec3b>(i - 1, j) != culoare ||
						(*src).at<Vec3b>(i - 1, j + 1) != culoare ||
						(*src).at<Vec3b>(i, j - 1) != culoare ||
						(*src).at<Vec3b>(i, j + 1) != culoare ||
						(*src).at<Vec3b>(i + 1, j - 1) != culoare ||
						(*src).at<Vec3b>(i + 1, j) != culoare ||
						(*src).at<Vec3b>(i + 1, j + 1) != culoare))
					{
						NP++;
						contur.at<Vec3b>(i, j).val[0] = 0;
						contur.at<Vec3b>(i, j).val[1] = 0;
						contur.at<Vec3b>(i, j).val[2] = 0;
					}
					else
					{
						contur.at<Vec3b>(i, j).val[0] = 255;
						contur.at<Vec3b>(i, j).val[1] = 255;
						contur.at<Vec3b>(i, j).val[2] = 255;
					}
				}
			perimetru = NP*CV_PI / 4;
			printf("Perimetru: %f\n", perimetru);

			T = 4 * CV_PI * arie / (perimetru*perimetru);
			printf("Factor de subtiere: %f\n", T);

			R = (float)(cmax - cmin + 1) / (rmax - rmin + 1);
			printf("Factor de aspect: %f\n", R);

			Point A(cmin, cr + (tan(unghiR))*(cmin - cc));
			Point B(cmax, cr + (tan(unghiR))*(cmax - cc));

			for (int i = 0; i < (*src).rows; i++)
			{
				contCol = 0;
				for (int j = 0; j < (*src).cols; j++)
					if ((*src).at<Vec3b>(i, j) == culoare)
					{
						vertical.at<Vec3b>(i, contCol).val[0] = 0;
						vertical.at<Vec3b>(i, contCol).val[1] = 0;
						vertical.at<Vec3b>(i, contCol).val[2] = 0;
						contCol++;
					}
					else
					{
						vertical.at<Vec3b>(i, j).val[0] = 255;
						vertical.at<Vec3b>(i, j).val[1] = 255;
						vertical.at<Vec3b>(i, j).val[2] = 255;
					}
			}

			for (int j = 0; j < (*src).cols; j++)
			{
				contLinie = 0;
				for (int i = 0; i < (*src).rows; i++)
					if ((*src).at<Vec3b>(i, j) == culoare)
					{
						orizontal.at<Vec3b>(contLinie, j).val[0] = 0;
						orizontal.at<Vec3b>(contLinie, j).val[1] = 0;
						orizontal.at<Vec3b>(contLinie, j).val[2] = 0;
						contLinie++;
					}
					else
					{
						orizontal.at<Vec3b>(i, j).val[0] = 255;
						orizontal.at<Vec3b>(i, j).val[1] = 255;
						orizontal.at<Vec3b>(i, j).val[2] = 255;
					}
			}

			line(axa, A, B, CV_RGB(255, 0, 0), 2);
			imshow("Axa", axa);
			imshow("Contur", contur);
			imshow("Proiectie Orizontala", orizontal);
			imshow("Proiectie Verticala", vertical);
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

	void aditivNiveluriGri()
	{
		int factor;
		uchar val, nou;
		printf("Introduceti factorul aditiv: ");
		scanf("%d", &factor);
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			int height = src.rows;
			int width = src.cols;
			Mat dst = Mat(height, width, CV_8UC1);
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					val = src.at<uchar>(i, j);
					int nou = val + factor;
					if (nou < 0)
						nou = 0;
					else if (nou > 255)
						nou = 255;
					dst.at<uchar>(i, j) = nou;
				}
			}

			imshow("input image", src);
			imshow("negative image", dst);
			waitKey();
		}
	}

	void multiplicativNiveluriGri()
	{
		float factor;
		uchar val, nou;
		printf("Introduceti factorul multiplicativ: ");
		scanf("%f", &factor);
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			int height = src.rows;
			int width = src.cols;
			Mat dst = Mat(height, width, CV_8UC1);
			for (int i = 0; i < height; i++)
			{
				for (int j = 0; j < width; j++)
				{
					val = src.at<uchar>(i, j);
					int nou = val * factor;
					if (nou < 0)
						nou = 0;
					else if (nou > 255)
						nou = 255;
					dst.at<uchar>(i, j) = nou;
				}
			}

			imshow("input image", src);
			imshow("negative image", dst);
			imwrite("dst.bmp", dst);
			waitKey();
		}
	}

	void sferturiColorate()
	{
		int i = 0, j = 0;
		Mat dst(256, 256, CV_8UC3);

		for (i = 0; i < 128; i++)
		{
			for (j = 0; j < 128; j++)
			{
				dst.at<Vec3b>(i, j) = Vec3b(255, 255, 255);
			}
		}
		for (i = 0; i<128; i++)
			for (j = 128; j<256; j++)
			{
				dst.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
			}


		for (i = 128; i < 256; i++)
		{
			for (j = 0; j < 128; j++)
			{
				dst.at<Vec3b>(i, j) = Vec3b(0, 255, 0);
			}
		}
		for (i = 128; i < 256; i++)
			for (j = 128; j<256; j++)
			{
				dst.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
			}


		imshow("color image", dst);
		waitKey();
	}

	void createMatrix()
	{
		float vals[9] = { 2, 0, 0, 0, 3, 0, 0, 0, 1 };
		Mat M(3, 3, CV_32FC1, vals);
		std::cout << M << std::endl;
		getchar();
		Mat MI = M.inv();
		std::cout << MI << std::endl;
		getchar();
	}

	void canaleRGB(){

		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat img = imread(fname, CV_LOAD_IMAGE_COLOR);
			Mat r(img.rows, img.cols, CV_8UC3);
			Mat g(img.rows, img.cols, CV_8UC3);
			Mat b(img.rows, img.cols, CV_8UC3);

			for (int i = 0; i < img.rows; i++)
				for (int j = 0; j < img.cols; j++)
				{
					Vec3b pixel = img.at<Vec3b>(i, j);

					r.at<Vec3b>(i, j)[0] = pixel[2];
					r.at<Vec3b>(i, j)[1] = 0;
					r.at<Vec3b>(i, j)[2] = 0;

					g.at<Vec3b>(i, j)[0] = 0;
					g.at<Vec3b>(i, j)[1] = pixel[1];
					g.at<Vec3b>(i, j)[2] = 0;

					b.at<Vec3b>(i, j)[0] = 0;
					b.at<Vec3b>(i, j)[1] = 0;
					b.at<Vec3b>(i, j)[2] = pixel[0];
				}
			imshow("R", r);
			imshow("G", g);
			imshow("B", b);

			waitKey();
		}
	}

	void color2gray()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat img = imread(fname, CV_LOAD_IMAGE_COLOR);
			Mat dest(img.rows, img.cols, CV_8UC1);

			for (int i = 0; i < img.rows; i++)
				for (int j = 0; j < img.cols; j++)
				{
					Vec3b pixel = img.at<Vec3b>(i, j);
					uchar Dest = (pixel[0] + pixel[1] + pixel[2]) / 3;
					dest.at<uchar>(i, j) = Dest;
				}

			imshow("initial image", img);
			imshow("grayscale image", dest);
			waitKey();
		}
	}

	void graytoblackwhite()
	{
		int prag;
		printf("Introduceti valoarea pragului: ");
		scanf("%d", &prag);
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dest(img.rows, img.cols, CV_8UC1);

			for (int i = 0; i < img.rows; i++)
				for (int j = 0; j < img.cols; j++)
				{
					if (img.at<uchar>(i, j) < prag)
						dest.at<uchar>(i, j) = 0;
					else
						dest.at<uchar>(i, j) = 255;
				}

			imshow("initial image", img);
			imshow("whiteblack image", dest);
			waitKey();
		}
	}

	void RGBtoHSV()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat img = imread(fname, CV_LOAD_IMAGE_COLOR);

			Mat imgH(img.rows, img.cols, CV_8UC1);
			Mat imgS(img.rows, img.cols, CV_8UC1);
			Mat imgV(img.rows, img.cols, CV_8UC1);

			for (int i = 0; i < img.rows; i++)
				for (int j = 0; j < img.cols; j++)
				{

					float r = (float)img.at<Vec3b>(i, j)[2] / 255;
					float g = (float)img.at<Vec3b>(i, j)[1] / 255;
					float b = (float)img.at<Vec3b>(i, j)[0] / 255;

					float M1 = max(r, g);
					float M = max(M1, b);
					float m1 = min(r, g);
					float m = min(m1, b);

					float C = M - m;
					float H;
					if (C != 0){
						if (M == r) H = 60 * (g - b) / C;
						if (M == g) H = 120 + 60 * (b - r) / C;
						if (M == b) H = 240 + 60 * (r - g) / C;
					}
					else{
						H = 0;
					}
					if (H < 0){
						H = H + 360;
					}

					imgH.at<uchar>(i, j) = H * 255 / 360;

					if (M != 0){
						imgS.at<uchar>(i, j) = (C / M) * 255;
					}
					else{
						imgS.at<uchar>(i, j) = 0;
					}

					imgV.at<uchar>(i, j) = M * 255;
				}

			imshow("H", imgH);
			imshow("S", imgS);
			imshow("V", imgV);
			waitKey();
		}
	}

	void proprietati()
	{
		Mat src;
		// Read image from file 
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			src = imread(fname, CV_LOAD_IMAGE_COLOR);
			//Create a window
			namedWindow("My Window", 1);

			//set the callback function for any mouse event
			setMouseCallback("My Window", CalculProprietatiGeometrice, &src);

			//show the image
			imshow("My Window", src);

			// Wait until user press some key
			waitKey();
		}
	}

	//lab5 - alg1
	void afisareLab5(Mat img1, Mat labels, int label)
	{
		Mat dst = Mat::zeros(img1.rows, img1.cols, CV_8UC3);
		vector<Vec3b> culori = vector<Vec3b>(label);
		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, 255);
		for (int i = 0; i < label; i++) {
			uchar r = d(gen);
			uchar g = d(gen);
			uchar b = d(gen);
			culori.at(i) = Vec3b(r, g, b);
		}

		for (int i = 0; i < labels.rows; i++) {
			for (int j = 0; j < labels.cols; j++) {
				if (labels.at<int>(i, j) > 0) {
					dst.at<Vec3b>(i, j) = culori.at(labels.at<int>(i, j) - 1);
				}
			}
		}
		imshow("imagine", dst);
		waitKey();
	}

	void lab5()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			int label;
			std::queue<Point2i> Q;
			Mat img1 = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat labels = Mat::zeros(img1.rows, img1.cols, CV_32SC1);
			int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
			int dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
			for (int i = 0; i < labels.rows; i++)
			{
				for (int j = 0; j < labels.cols; j++)
				{
					labels.at<int>(i, j) = 0;
				}
			}
			label = 0;
			for (int i = 0; i < labels.rows; i++)
			{
				for (int j = 0; j < labels.cols; j++)
				{
					if (img1.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0)
					{
						label++;
						Q = std::queue<Point2i>(); // queue <Point> que;
						labels.at<int>(i, j) = label;
						Q.push(Point2i(i, j));
						while (!Q.empty())
						{
							Point2i p = Q.front();
							Q.pop();
							for (int k = 0; k < 8; k++)
							{
								int ni, nj;
								ni = p.x + di[k];
								nj = p.y + dj[k];
								if (!(ni < 0 || ni >= img1.rows || nj < 0 || nj >= img1.cols)
									&& labels.at<int>(ni, nj) == 0 && img1.at<uchar>(ni, nj) == 0) {
									labels.at<int>(ni, nj) = label;
									Q.push({ ni, nj });
								}
							}
						}
					}
				}
			}



			afisareLab5(img1, labels, label);

		}


	}

	//sau

	void afisareAlgoritm1(Mat img1, Mat labels, int label)
	{
		Mat dst = Mat::zeros(img1.rows, img1.cols, CV_8UC3);
		Vec3b* culori = new Vec3b[label + 1];

		std::default_random_engine gen;
		std::uniform_int_distribution<int> d(0, 255);

		for (int i = 0; i < label; i++) {
			uchar r = d(gen);
			uchar g = d(gen);
			uchar b = d(gen);
			culori[i] = Vec3b(r, g, b);
		}

		for (int i = 0; i < labels.rows; i++) {
			for (int j = 0; j < labels.cols; j++) {
				if (labels.at<int>(i, j) > 0) {
					dst.at<Vec3b>(i, j) = culori[labels.at<int>(i, j)];
				}
			}
		}
		imshow("imagine", dst);
		waitKey();
	}

	void p1()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			int label;

			Mat img1 = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);			//grayscale image
			Mat labels = Mat::zeros(img1.rows, img1.cols, CV_32SC1);

			label = 0;

			for (int i = 0; i < labels.rows; i++)
			{
				for (int j = 0; j < labels.cols; j++)
				{
					if (img1.at<uchar>(i, j) == 0 && labels.at<int>(i, j) == 0)
					{
						label++;

						std::queue<Point2i> Q; // queue <Point> que;

						labels.at<int>(i, j) = label;

						Q.push(Point2i(i, j));

						while (!Q.empty())
						{
							Point2i q = Q.front();
							Q.pop();

							for (int k = q.x - 1; k <= q.x + 1; k++) {
								for (int m = q.y - 1; m <= q.y + 1; m++) {
									if ((img1.at<uchar>(k, m) == 0) && (labels.at<int>(k, m) == 0)) {

										labels.at<int>(k, m) = label;
										Q.push(Point2i(k, m));
									}
								}
							}
						}
					}
				}
			}

			afisareAlgoritm1(img1, labels, label);
		}
	}

	int minim_Vector(vector<int> L)				//minimul elementelor dintr-un vector
	{
		int minim = 10000;
		for (int i = 0; i < L.size(); i++)
		{

			if (minim >= L.at(i))
				minim = L.at(i);
		}
		return minim;
	}

	void etichetare_2steps(void* param)
	{
		Mat *src = (Mat*)param;

		int label = 0;
		Vec3b negru = (0, 0, 0);

		Mat labels(src->rows, src->cols, CV_32SC1, Scalar(0));
		int height = src->rows, width = src->cols;

		vector<vector<int>> edges;

		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				if (src->at<Vec3b>(i, j) == negru && labels.at<int>(i, j) == 0)
				{
					vector<int> L;

					for (int k = i - 1; k <= i; k++)
					{
						for (int l = j - 1; l <= j + 1; l++)
						{
							//if (k == i && l == j)
							//	break;
							if (labels.at<int>(k, l) > 0)
							{
								L.push_back(labels.at<int>(k, l));
							}
						}
					}

					if (L.size() == 0)
					{
						label++;
						labels.at<int>(i, j) = label;
					}
					else
					{
						//printf("%d %d  \n\n", L.size(), L.at(0));

						int min = minim_Vector(L);
						labels.at<int>(i, j) = min;
						for (int aux = 0; aux < L.size(); aux++)
						{
							//printf("%d - %d \n", L.at(aux), min);
							if (L.at(aux) != min)
							{
								edges.resize(label + 1);			//facem resize!!
								edges[min].push_back(L.at(aux));
								edges[L.at(aux)].push_back(min);
							}
						}
					}
				}
			}
		}

		int newlabel = 0;
		int *newlabels = new int[label + 1];

		for (int i = 0; i <= label; i++)
		{
			newlabels[i] = 0;
		}

		for (int i = 1; i <= label; i++)
		{
			if (newlabels[i] == 0)
			{
				newlabel++;
				std::queue<int> Q;
				newlabels[i] = newlabel;
				Q.push(i);

				while (!Q.empty())
				{
					int x = Q.front();
					Q.pop();

					for (int j = 0; j < edges[x].size(); j++)
					{
						if (newlabels[edges[x].at(j)] == 0)
						{
							newlabels[edges[x].at(j)] = newlabel;
							Q.push(edges[x].at(j));
						}
					}
				}

			}
		}

		Mat dst(height, width, CV_8UC3);
		Vec3b *color = new Vec3b[newlabel + 2];

		for (int i = 1; i <= newlabel; i++)
		{
			color[i][0] = rand() % 256;
			color[i][1] = rand() % 256;
			color[i][2] = rand() % 256;
		}


		color[0][0] = 255;
		color[0][1] = 255;
		color[0][2] = 255;

		for (int i = 0; i < height - 1; i++)
			for (int j = 0; j < width - 1; j++)
			{
				dst.at<Vec3b>(i, j) = color[newlabels[labels.at<int>(i, j)]];

			}

		//delete[] color;
		imshow("Colorat2", dst);

	}

	void p2()
	{
		Mat src;
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			src = imread(fname, CV_LOAD_IMAGE_COLOR);
			namedWindow("My Window", 1);
			etichetare_2steps(&src);
			imshow("My Window", src);
			waitKey(0);
		}

	}

	void contur()
	{
		char fname[MAX_PATH];

		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = Mat::zeros(src.rows, src.cols, CV_8UC1);

			int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
			int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

			vector<int> dirVec, deriv;
			Point2i p0, p1, pn, pn1;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
				{
					if (src.at<uchar>(i, j) == 0)
					{
						p0 = Point2i(i, j);
						pn = Point2i(i, j);
						dst.at<uchar>(i, j) = 255;

						i = src.rows;
						j = src.cols;
					}
				}
			int dir = 7;
			int n = 0;

			bool finish = false;

			while (!finish)
			{
				if (dir % 2 == 0)
					dir = (dir + 7) % 8;
				else dir = (dir + 6) % 8;

				while (src.at<uchar>(pn.x + di[dir], pn.y + dj[dir]) != 0)
					dir = (dir + 1) % 8;

				dirVec.push_back(dir);
				n++;

				if (n == 1)
					p1 = Point2i(pn.x + di[dir], pn.y + dj[dir]);

				if (n > 1)
					deriv.push_back((dirVec.at(n - 1) - dirVec.at(n - 2) + 8) % 8);
				dst.at<uchar>(pn.x + di[dir], pn.y + dj[dir]) = 255;

				pn1 = Point2i(pn.x, pn.y);
				pn = Point2i(pn.x + di[dir], pn.y + dj[dir]);

				if (pn.x == p1.x&&pn.y == p1.y&&pn1.x == p0.x&&pn1.y == p0.y&&n > 2)
					finish = true;

			}

			deriv.push_back((dirVec.at(0) - dirVec.at(n - 1) + 8) % 8);

			printf("Vector de directii: ");
			for (int i = 0; i < dirVec.size(); i++)
				printf("%d ", dirVec.at(i));

			printf("\nDerivata: ");
			for (int j = 0; j < deriv.size(); j++)
				printf("%d ", deriv.at(j));

			imshow("Contur", dst);
			waitKey();
		}
	}

	void reconstructie()
	{
		char fname[MAX_PATH];

		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src;

			int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
			int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

			FILE *f = fopen("D:\\Zeic Naomi-Ioana\\OpenCVApplication-VS2013_OCV2413_basic\\reconstruct.txt", "r");

			int x, y, n, dir;

			fscanf(f, "%d %d", &x, &y);
			fscanf(f, "%d", &n);

			for (int i = 0; i < n; i++)
			{
				dst.at<uchar>(x, y) = 0;
				fscanf(f, "%d", &dir);
				x += di[dir];
				y += dj[dir];
			}
			imshow("Constructie", dst);
			fclose(f);
			waitKey();
		}
	}

	void dilatare()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
			int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

			for (int i = 1; i < src.rows - 1; i++)
				for (int j = 1; j < src.cols - 1; j++)
					if (src.at<uchar>(i, j) == 0)
						for (int k = 0; k < 8; k++)
							dst.at<uchar>(i + di[k], j + dj[k]) = 0;

			imshow("Original", src);
			imshow("Dilatare", dst);
			waitKey();
		}
	}

	Mat dilatare(Mat src)
	{
		Mat dst = src.clone();

		int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
		int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

		for (int i = 1; i < src.rows - 1; i++)
			for (int j = 1; j < src.cols - 1; j++)
				if (src.at<uchar>(i, j) == 0)
					for (int k = 0; k < 8; k++)
						dst.at<uchar>(i + di[k], j + dj[k]) = 0;

		return dst;
	}

	void eroziune()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
			int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

			for (int i = 1; i < src.rows - 1; i++)
				for (int j = 1; j < src.cols - 1; j++)
					if (src.at<uchar>(i, j) == 0)
						for (int k = 0; k < 8; k++)
							if (src.at<uchar>(i + di[k], j + dj[k]) == 255)
								dst.at<uchar>(i, j) = 255;

			imshow("Original", src);
			imshow("Eroziune", dst);
			waitKey();
		}
	}

	Mat eroziune(Mat src)
	{
		Mat dst = src.clone();

		int di[8] = { 0, -1, -1, -1, 0, 1, 1, 1 };
		int dj[8] = { 1, 1, 0, -1, -1, -1, 0, 1 };

		for (int i = 1; i < src.rows - 1; i++)
			for (int j = 1; j < src.cols - 1; j++)
				if (src.at<uchar>(i, j) == 0)
					for (int k = 0; k < 8; k++)
						if (src.at<uchar>(i + di[k], j + dj[k]) == 255)
							dst.at<uchar>(i, j) = 255;

		return dst;
	}

	void deschidere()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat aux = src.clone();
			Mat dst = eroziune(aux);
			aux = dilatare(dst);
			dst = aux.clone();

			imshow("Original", src);
			imshow("Deschidere", dst);
			waitKey();
		}
	}

	void inchidere()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat aux = src.clone();
			Mat dst = dilatare(aux);
			aux = eroziune(dst);
			dst = aux.clone();

			imshow("Original", src);
			imshow("Inchidere", dst);
			waitKey();
		}
	}

	void dilatareN()
	{
		int n = 15;
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{

			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			for (int i = 0; i < n; i++)
			{
				dst = dilatare(dst);
			}

			imshow("Originala", src);
			imshow("DilatareN", dst);
			waitKey();
		}
	}

	void eroziuneN()
	{
		int n = 50;
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{

			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			for (int i = 0; i < n; i++)
			{
				dst = eroziune(dst);
			}

			imshow("Originala", src);
			imshow("EroziuneN", dst);
			waitKey();
		}
	}

	void conturL7()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src, dst, aux;
			src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

			dst = src.clone();
			aux = src.clone();
			aux = eroziune(src);

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					if (aux.at<uchar>(i, j) == src.at<uchar>(i, j))
						dst.at<uchar>(i, j) = 255;
					else
						dst.at<uchar>(i, j) = 0;

			imshow("Originala", src);
			imshow("Contur", dst);
			waitKey();
		}
	}

	void umplere()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src, dst, com, aux;
			src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Point2i p;
			p.x = src.rows / 2;
			p.y = src.cols / 2;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					if (src.at<uchar>(i, j) == 0)
						com.at<uchar>(i, j) = 255;
					else
						com.at<uchar>(i, j) = 0;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					dst.at<uchar>(i, j) = 255;
			dst.at<uchar>(p.x, p.y) = 0;

			bool ok = false;

			while (!ok)
			{
				dst = dilatare(dst);
				aux = dst.clone();

				for (int i = 0; i < src.rows; i++)
					for (int j = 0; j < src.cols; j++)
						if (src.at<uchar>(i, j) == 0 && dst.at<uchar>(i, j) == 0)
							dst.at<uchar>(i, j) = 0;
			}

		}

	}

	void histograma()
	{
		float M = 0;
		float p[256];
		float miu = 0;
		float sigma = 0;
		int h[256];
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			M = src.rows*src.cols;

			for (int i = 0; i < 256; ++i)
				h[i] = 0;

			for (int i = 0; i < src.rows; ++i)
				for (int j = 0; j < src.cols; ++j)
					h[src.at<uchar>(i, j)]++;

			for (int g = 0; g < 256; g++)
			{
				p[g] = h[g] / M;
				miu += g*p[g];
			}

			for (int g = 0; g < 256; g++)
				sigma += pow(g - miu, 2)*p[g];
			sigma = sqrt(sigma);


			showHistogram("histograma", h, 256, 500);
			printf("miu: %f\n", miu);
			printf("sigma: %f\n", sigma);
			imshow("Original", src);
			waitKey();
		}
	}

	void histogram(Mat img)
	{
		int h[256];

		for (int i = 0; i < 256; i++)
			h[i] = 0;

		for (int i = 0; i < img.rows; i++)
			for (int j = 0; j < img.cols; j++)
				h[img.at<uchar>(i, j)]++;

		showHistogram("Histogram", h, 256, 500);
		waitKey();
	}

	void binarizare()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int h[256];
			int maxI = INT_MIN;
			int minI = INT_MAX;

			for (int i = 0; i < 256; i++)
				h[i] = 0;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					h[src.at<uchar>(i, j)]++;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
				{
					if (src.at<uchar>(i, j)>maxI)
						maxI = src.at<uchar>(i, j);
					if (src.at<uchar>(i, j) < minI)
						minI = src.at<uchar>(i, j);
				}

			float T = (maxI + minI) / 2;
			float prevT = 0;
			float error = 0.1f;

			while (fabs(T - prevT) >= error)
			{
				float miuG1 = 0.0f;
				float miuG2 = 0.0f;
				float NG1 = 0.0f;
				float NG2 = 0.0f;

				for (int i = minI; i <= T; i++)
					NG1 += h[i];
				for (int i = T + 1; i <= maxI; i++)
					NG2 += h[i];
				for (int i = minI; i <= T; i++)
					miuG1 += i*h[i];
				for (int i = T + 1; i <= maxI; i++)
					miuG2 += i*h[i];

				miuG1 /= NG1;
				miuG2 /= NG2;
				prevT = T;
				T = (miuG1 + miuG2) / 2;


			}
			printf("prag: %f\n", T);

			for (int i = 0; i < dst.rows; ++i)
				for (int j = 0; j < dst.cols; ++j)
				{
					if (src.at<uchar>(i, j) < T)
						dst.at<uchar>(i, j) = 0;
					else
						dst.at<uchar>(i, j) = 255;
				}

			imshow("Original", src);
			imshow("Destination", dst);
			waitKey();
		}
	}

	void negativulImaginii()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int h[256];
			int newh[256];

			for (int i = 0; i < 256; i++)
				h[i] = 0;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					h[src.at<uchar>(i, j)]++;

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					dst.at<uchar>(i, j) = 255 - src.at<uchar>(i, j);

			for (int i = 0; i < 256; i++)
				newh[i] = 0;

			for (int i = 0; i < dst.rows; i++)
				for (int j = 0; j < dst.cols; j++)
					newh[dst.at<uchar>(i, j)]++;

			imshow("Original", src);
			imshow("Imaginea negativa", dst);
			showHistogram("Histograma imaginii initiale", h, 256, 500);
			showHistogram("Histograma negativului imaginii", newh, 256, 500);
			waitKey();
		}
	}


	void luminozitate()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int offset;
			int h[256];
			int newh[256];

			printf("Introduceti offset: ");
			scanf("%d", &offset);

			for (int i = 0; i < 256; i++)
			{
				h[i] = 0;
				newh[i] = 0;
			}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
				{
					dst.at<uchar>(i, j) = src.at<uchar>(i, j) + offset;
					if (dst.at<uchar>(i, j) < 0)
						dst.at<uchar>(i, j) = 0;
					if (dst.at<uchar>(i, j) > 255)
						dst.at<uchar>(i, j) = 255;
				}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					h[src.at<uchar>(i, j)]++;

			for (int i = 0; i < dst.rows; i++)
				for (int j = 0; j < dst.cols; j++)
					newh[dst.at<uchar>(i, j)]++;

			imshow("Original", src);
			imshow("Destinatie", dst);
			showHistogram("Histograma imaginii", h, 256, 500);
			showHistogram("Histograma destinatiei", newh, 256, 500);
			waitKey();
		}
	}

	void contrast()
	{
		int gOutMin, gOutMax, gInMin = 256, gInMax = 0;

		printf("Introduceti gOutMin: ");
		scanf("%d", &gOutMin);
		printf("\nIntroduceti gOutMax: ");
		scanf("%d", &gOutMax);

		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int h[256];
			int newh[256];


			for (int i = 0; i < 256; i++)
			{
				h[i] = 0;
				newh[i] = 0;
			}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					h[src.at<uchar>(i, j)]++;

			for (int i = 0; i < src.rows; ++i)
				for (int j = 0; j < src.cols; ++j)
				{
					if (src.at<uchar>(i, j) > gInMax)
						gInMax = src.at<uchar>(i, j);

					if (src.at<uchar>(i, j) < gInMin)
						gInMin = src.at<uchar>(i, j);
				}

			int x = (gOutMax - gOutMin) / (gInMax - gInMin);
			if (x > 1)
				printf("Latire");
			if (x < 1)
				printf("Ingustare");

			for (int i = 0; i < src.rows; ++i)
				for (int j = 0; j < src.cols; ++j)
				{
					dst.at<uchar>(i, j) = gOutMin + (src.at<uchar>(i, j) - gInMin)*x;

					if (dst.at<uchar>(i, j) < 0)
						dst.at<uchar>(i, j) = 0;

					if (dst.at<uchar>(i, j) > 255)
						dst.at<uchar>(i, j) = 255;
				}



			for (int i = 0; i < dst.rows; i++)
				for (int j = 0; j < dst.cols; j++)
					newh[dst.at<uchar>(i, j)]++;

			imshow("Original", src);
			imshow("Destinatie", dst);
			showHistogram("Histograma imaginii", h, 256, 500);
			showHistogram("Histograma destinatiei", newh, 256, 500);
			waitKey();
		}
	}

	void corectieGamma()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int h[256];
			int newh[256];

			float gamma;
			printf("Introduceti gamma: ");
			scanf("%f", &gamma);

			for (int i = 0; i < 256; i++)
			{
				h[i] = 0;
				newh[i] = 0;
			}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
				{
					dst.at<uchar>(i, j) = (uchar)(255 * pow(src.at<uchar>(i, j) / (float)255, gamma));

					if (dst.at<uchar>(i, j) < 0)
						dst.at<uchar>(i, j) = 0;

					if (dst.at<uchar>(i, j) > 255)
						dst.at<uchar>(i, j) = 255;
				}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					h[src.at<uchar>(i, j)]++;

			for (int i = 0; i < dst.rows; i++)
				for (int j = 0; j < dst.cols; j++)
					newh[dst.at<uchar>(i, j)]++;

			imshow("Original", src);
			imshow("Destinatie", dst);
			showHistogram("Histograma imaginii", h, 256, 500);
			showHistogram("Histograma destinatiei", newh, 256, 500);
			waitKey();
		}

	}

	void egalizare()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int h[256];
			int newh[256];
			int M = src.rows * src.cols;
			float probabilityDensity[256];
			int tab[256];

			for (int i = 0; i < 256; i++)
			{
				h[i] = 0;
				newh[i] = 0;
				probabilityDensity[i] = 0;
				tab[i] = 0;
			}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					h[src.at<uchar>(i, j)]++;

			for (int i = 0; i < 256; i++)
				probabilityDensity[i] = h[i] / (float)M;

			float rK = 0.0f;
			float FDPC[256];
			FDPC[0] = probabilityDensity[0];

			for (int k = 1; k < 256; k++)
			{
				rK = (k / (float)255);
				FDPC[k] = FDPC[k - 1] + probabilityDensity[k];
			}

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					tab[src.at<uchar>(i, j)] = (int)(255 * FDPC[src.at<uchar>(i, j)]);

			for (int i = 0; i < src.rows; i++)
				for (int j = 0; j < src.cols; j++)
					dst.at<uchar>(i, j) = tab[src.at<uchar>(i, j)];

			for (int i = 0; i < dst.rows; i++)
				for (int j = 0; j < dst.cols; j++)
					newh[dst.at<uchar>(i, j)]++;

			imshow("Original", src);
			imshow("Destinatie", dst);
			showHistogram("Histograma imaginii", h, 256, 500);
			showHistogram("Histograma destinatiei", newh, 256, 500);
			waitKey();
		}
	}

	void medie3x3()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int dim = 3;
			int mat[3][3] = { { 1, 1, 1 }, { 1, 1, 1 }, { 1, 1, 1 } };

			for (int i = 1; i < src.rows - 1; i++){
				for (int j = 1; j < src.cols - 1; j++){
					int suma = 0;
					for (int k = -dim / 2; k <= dim / 2; k++){
						for (int m = -dim / 2; m <= dim / 2; m++)
						{
							suma += src.at<uchar>(i + k, j + m)*mat[k + dim / 2][m + dim / 2];
						}

					}
					dst.at<uchar>(i, j) = suma / 9;
				}
			}

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void medie5x5()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int dim = 5;
			int mat[5][5] = { { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 } };


			for (int i = 2; i < src.rows - 2; i++){
				for (int j = 2; j < src.cols - 2; j++){
					int suma = 0;
					for (int k = -dim / 2; k <= dim / 2; k++){
						for (int m = -dim / 2; m <= dim / 2; m++)
						{
							suma += src.at<uchar>(i + k, j + m)*mat[k + dim / 2][m + dim / 2];
						}
					}
					dst.at<uchar>(i, j) = suma / 25;
				}
			}

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void filtruGauss()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int dim = 3;
			int mat[3][3] = { { 1, 2, 1 }, { 2, 4, 2 }, { 1, 2, 1 } };

			for (int i = 1; i < src.rows - 1; i++){
				for (int j = 1; j < src.cols - 1; j++){
					int suma = 0;
					for (int k = -dim / 2; k <= dim / 2; k++){
						for (int m = -dim / 2; m <= dim / 2; m++)
						{
							suma += src.at<uchar>(i + k, j + m)*mat[k + dim / 2][m + dim / 2];
						}

					}
					dst.at<uchar>(i, j) = suma / 16;
				}
			}

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void filtruLaplace()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int dim = 3;
			int mat[3][3] = { { 0, -1, 0 }, { -1, 4, -1 }, { 0, -1, 0 } };

			for (int i = 1; i < src.rows - 1; i++){
				for (int j = 1; j < src.cols - 1; j++){
					int suma = 0;
					for (int k = -dim / 2; k <= dim / 2; k++){
						for (int m = -dim / 2; m <= dim / 2; m++)
						{
							suma += src.at<uchar>(i + k, j + m)*mat[k + dim / 2][m + dim / 2];
						}

					}
					if (suma >= 255)
						dst.at<uchar>(i, j) = 255;
					else {
						if (suma < 0)
							dst.at<uchar>(i, j) = 0;
						else
							dst.at<uchar>(i, j) = suma;
					}
				}
			}

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void filtruTS()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();
			int dim = 3;
			int mat[3][3] = { { 0, -1, 0 }, { -1, 5, -1 }, { 0, -1, 0 } };

			for (int i = 1; i < src.rows - 1; i++){
				for (int j = 1; j < src.cols - 1; j++){
					int suma = 0;
					for (int k = -dim / 2; k <= dim / 2; k++){
						for (int m = -dim / 2; m <= dim / 2; m++)
						{
							suma += src.at<uchar>(i + k, j + m)*mat[k + dim / 2][m + dim / 2];
						}

					}
					if (suma >= 255)
						dst.at<uchar>(i, j) = 255;
					else {
						if (suma < 0)
							dst.at<uchar>(i, j) = 0;
						else
							dst.at<uchar>(i, j) = suma;
					}
				}
			}

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void centering_transform(Mat src) {
		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				src.at<float>(i, j) = ((i + j) & 1) ? -src.at<float>(i, j) : src.at<float>(i, j);
			}
		}
	}

	Mat logaritmMagnitudine(Mat src) {
		Mat srcf;
		Mat dst;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);

		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		Mat mag, phi;
		magnitude(channels[0], channels[1], mag);
		//phase(channels[0], channels[1], phi);

		for (int i = 0; i < mag.rows; i++)
			for (int j = 0; j < mag.cols; j++)
			{
				mag.at<float>(i, j) = log(1 + mag.at<float>(i, j));
			}

		normalize(mag, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		return dst;
	}

	void Fourier()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = logaritmMagnitudine(src);

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	Mat trecejos(Mat src) {
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);

		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		int h = fourier.rows;
		int w = fourier.cols;
		int R = 10;
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				if (((h / 2 - i)*(h / 2 - i) + (w / 2 - j)*(w / 2 - j)) > R*R)
				{
					channels[0].at<float>(i, j) = 0;
					channels[1].at<float>(i, j) = 0;
				}


		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		return dst;
	}

	void FTJIdeal()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = trecejos(src);

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	Mat trecesus(Mat src) {
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);

		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		int h = fourier.rows;
		int w = fourier.cols;
		int R = 10;
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
				if (((h / 2 - i)*(h / 2 - i) + (w / 2 - j)*(w / 2 - j)) <= R*R)
				{
					channels[0].at<float>(i, j) = 0;
					channels[1].at<float>(i, j) = 0;
				}


		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		return dst;
	}

	void FTSIdeal()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = trecesus(src);

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	Mat trecejosGauss(Mat src) {
		Mat srcf;
		src.convertTo(srcf, CV_32FC1);
		centering_transform(srcf);

		Mat fourier;
		dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

		Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
		split(fourier, channels);

		int h = fourier.rows;
		int w = fourier.cols;
		float A = 10;
		for (int i = 0; i < h; i++)
			for (int j = 0; j < w; j++)
			{
				channels[0].at<float>(i, j) = channels[0].at<float>(i, j)*exp(-(((h / 2 - i)*(h / 2 - i) + (w / 2 - j)*(w / 2 - j)) / (A*A)));
				channels[1].at<float>(i, j) = channels[1].at<float>(i, j)*exp(-(((h / 2 - i)*(h / 2 - i) + (w / 2 - j)*(w / 2 - j)) / (A*A)));
			}


		Mat dst, dstf;
		merge(channels, 2, fourier);
		dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

		centering_transform(dstf);
		normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		return dst;
	}

	void FTJGauss()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = trecejosGauss(src);

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void filtruMedian()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int w;
			printf(" Introduceti valorea pentru w: ");
			scanf("%d", &w);
			vector<uchar> v;

			for (int i = w / 2; i < src.rows - w / 2; i++)
				for (int j = w / 2; j < src.cols - w / 2; j++)
				{
					v.clear();
					for (int k = -w / 2; k <= w / 2; k++)
						for (int m = -w / 2; m <= w / 2; m++)
						{
							v.push_back(src.at<uchar>(k + i, m + j));
						}
					sort(v.begin(), v.end());
					dst.at<uchar>(i, j) = v[w*w / 2];
				}
			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void filtruGauss1x2()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat dst = src.clone();

			int k = 2;	//w/2
			float nucleu[5][5] = { 0 };
			float sigma;
			printf("Introduceti valoare pentru sigma: ");
			scanf("%f", &sigma);
			int w = (int)(6 * sigma);
			float rap = 2 * sigma*sigma*CV_PI;

			double t = (double)getTickCount();

			for (int i = 0; i < w; i++) {
				for (int j = 0; j < w; j++) {
					int deviatia = (i - k)*(i - k) + (j - k)*(j - k);
					nucleu[i][j] = exp((-1.0)*deviatia / 2 * sigma*sigma) / rap;
				}
			}

			float suma = 0;
			for (int i = 0; i < w; i++) {
				for (int j = 0; j < w; j++) {
					suma += nucleu[i][j];
				}
			}

			float value;

			for (int i = k; i < src.rows - k; i++) {
				for (int j = k; j < src.cols - k; j++) {

					value = 0;

					for (int m = -k; m <= k; m++) {
						for (int n = -k; n <= k; n++) {

							value += src.at<uchar>(i + m, j + n) *  nucleu[m + k][n + k];

						}
					}

					dst.at<uchar>(i, j) = value / suma;
				}
			}


			t = ((double)getTickCount() - t) / getTickFrequency();
			printf("Time = %.3f [ms]\n", t * 1000);

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void filtruGauss2x1()
	{
		char fname[MAX_PATH];
		while (openFileDlg(fname))
		{
			Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
			Mat aux = src.clone();

			int k = 2;	//w/2
			float nucleu[5][5] = { 0 };
			float linie[5] = { 0 };
			float sigma;
			printf("Introduceti valoare pentru sigma: ");
			scanf("%f", &sigma);
			int w = (int)(6 * sigma);
			float rap = sqrt(2 * CV_PI)*sigma;

			double t = (double)getTickCount();

			for (int i = 0; i < w; i++) {
				for (int j = 0; j < w; j++) {
					int deviatia = (i - k)*(i - k) + (j - k)*(j - k);
					nucleu[i][j] = exp((-1.0)*deviatia / 2 * sigma*sigma) / rap;
				}
			}

			float suma = 0;
			for (int i = 0; i < w; i++) {
				linie[i] = nucleu[i][k];
				suma += linie[i];
			}

			float value;

			for (int i = k; i < src.rows - k; i++) {
				for (int j = k; j < src.cols - k; j++) {

					value = 0;

					for (int m = -k; m <= k; m++) {
						value += src.at<uchar>(i + m, j) *  linie[m + k];
					}
					aux.at<uchar>(i, j) = value / suma;
				}
			}


			Mat dst = aux.clone();

			for (int i = k; i < src.rows - k; i++) {
				for (int j = k; j < src.cols - k; j++) {
					value = 0;

					for (int m = -k; m <= k; m++) {
						value += aux.at<uchar>(i, j + m) *  linie[m + k];
					}
					dst.at<uchar>(i, j) = value / suma;
				}
			}


			t = ((double)getTickCount() - t) / getTickFrequency();
			printf("Time = %.3f [ms]\n", t * 1000);

			imshow("Sursa", src);
			imshow("Destinatie", dst);
			waitKey();
		}
	}

	void detectieMuchiiCanny() {

		char fname[MAX_PATH];
		openFileDlg(fname);
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		int Sx[3][3] = { { -1, 0, 1 }, { -2, 0, 2 }, { -1, 0, 1 } };
		int Sy[3][3] = { { 1, 2, 1 }, { 0, 0, 0 }, { -1, -2, -1 } };

		Mat G = Mat(src.rows, src.cols, CV_32FC1);
		Mat Phi = Mat(src.rows, src.cols, CV_32FC1);
		Mat Gimag = Mat(src.rows, src.cols, CV_8UC1);

		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {

				int Gx = 0;
				int Gy = 0;

				for (int k = 0; k < 3; k++) {
					for (int m = 0; m < 3; m++) {

						Gx += src.at<uchar>(i + k - 1, j + m - 1) * Sx[k][m];
						Gy += src.at<uchar>(i + k - 1, j + m - 1) * Sy[k][m];
					}
				}

				Phi.at<float>(i, j) = atan2(Gy, Gx);
				G.at<float>(i, j) = sqrt(Gx*Gx + Gy*Gy);
				Gimag.at<uchar>(i, j) = G.at<float>(i, j) / (4 * sqrt(2));
			}
		}

		//imshow("start", src);
		//imshow("gradient_magnitude", Gimag);


		Mat Gs = G.clone();

		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				float u = Phi.at<float>(i, j);

				if ((u >= 7 * CV_PI / 8.0 || u <= (-7) * CV_PI / 8.0) || (u <= CV_PI / 8.0 && u >= (-1) * CV_PI / 8.0)) { //pt 0
					if (G.at<float>(i, j) < G.at<float>(i, j - 1) || G.at<float>(i, j) < G.at<float>(i, j + 1)) {
						Gs.at<float>(i, j) = 0;
						Gimag.at<uchar>(i, j) = 0;
					}
				}
				else
					if ((u <= 3 * CV_PI / 8.0 && u >= CV_PI / 8.0) || (u >= (-7) * CV_PI / 8.0 && u <= (-5) * CV_PI / 8.0)) { //pt 1
						if (G.at<float>(i, j) < G.at<float>(i + 1, j - 1) || G.at<float>(i, j) < G.at<float>(i - 1, j + 1)) {
							Gs.at<float>(i, j) = 0;
							Gimag.at<uchar>(i, j) = 0;
						}
					}
					else
						if ((u <= 5 * CV_PI / 8.0 && u >= 3 * CV_PI / 8.0) || (u <= (-3) * CV_PI / 8 && u >= (-5) * CV_PI / 8)) { //pt 2
							if (G.at<float>(i, j) < G.at<float>(i - 1, j) || G.at<float>(i, j) < G.at<float>(i + 1, j)) {
								Gs.at<float>(i, j) = 0;
								Gimag.at<uchar>(i, j) = 0;
							}
						}
						else {
							if (G.at<float>(i, j) < G.at<float>(i - 1, j - 1) || G.at<float>(i, j) < G.at<float>(i + 1, j + 1)) { //pt 3
								Gs.at<float>(i, j) = 0;
								Gimag.at<uchar>(i, j) = 0;
							}
						}
			}
		}

		//imshow("dupa_pas3", Gimag);
		//waitKey(0);

		//continuare
		int h[256] = { 0 };
		int height = Gimag.rows;
		int width = Gimag.cols;

		for (int i = 1; i < height - 1; ++i)
			for (int j = 1; j < width - 1; ++j)
				h[Gimag.at<uchar>(i, j)]++;

		float NrPctGradientNenul = (height - 2) * (width - 2) - h[0];
		float NrPctMuchie = 0.1 * NrPctGradientNenul;
		float sum = 0;
		float TH = 0;
		float TL = 0;
		float k = 0.4;

		for (int i = 255; i > 0; i--) {
			sum += h[i];
			if (sum >= NrPctMuchie) {
				TH = i;
				i = -1;
			}
		}

		TL = k * TH;

		Mat binarizare = Gimag.clone();

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {

				if (Gimag.at<uchar>(i, j) >= TH)
					binarizare.at<uchar>(i, j) = 255;

				else if ((Gimag.at<uchar>(i, j) < TH) && (Gimag.at<uchar>(i, j) >= TL))
					binarizare.at<uchar>(i, j) = 128;

				else binarizare.at<uchar>(i, j) = 0;
			}
		}

		//imshow("Destinatie", binarizare);
		//waitKey(0);

		std::queue<Point2i> Q;
		int di[8] = { -1, -1, -1, 0, 0, 1, 1, 1 };
		int dj[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

		for (int i = 1; i < height - 1; i++)
		{
			for (int j = 1; j < width - 1; j++)
			{
				if (binarizare.at<uchar>(i, j) == 255)
				{
					Q = std::queue<Point2i>(); // queue <Point> que;
					Q.push(Point2i(j, i));
					while (!Q.empty())
					{
						Point2i p = Q.front();
						Q.pop();

						for (int k = p.y - 1; k <= p.y + 1; k++)
							for (int l = p.x - 1; l <= p.x + 1; l++)
								if (binarizare.at<uchar>(k, l) == 128)
								{
									binarizare.at<uchar>(k, l) = 255;
									Q.push(Point2i(l, k));
								}

					}
				}
			}
		}
		for (int i = 1; i < height - 1; i++)
			for (int j = 1; j < width - 1; j++)
				if (binarizare.at<uchar>(i, j) == 128)
					binarizare.at<uchar>(i,j) = 0;

		imshow("Destinatie", binarizare);
		waitKey(0);
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
			printf(" 10 - L1- Aditiv niveluri gri\n");
			printf(" 11 - L1- Multiplicativ niveluri gri\n");
			printf(" 12 - L1- Sferturi colorate\n");
			printf(" 13 - L1- Create matrix\n");
			printf(" 14 - L2- Canale RGB\n");
			printf(" 15 - L2- Color to gray\n");
			printf(" 16 - L2- Gray to whiteblack image\n");
			printf(" 17 - L2- RGB to HSV\n");
			printf(" 18 - L4- Proprietati geometrice\n");
			printf(" 19 - L5- Parcurgere in latime\n");
			printf(" 20 - L5- 2 Treceri clase echivalenta\n");
			printf(" 21 - L6- Urmarirea conturului\n");
			printf(" 22 - L6- Reconstructie\n");
			printf(" 23 - L7- Dilatare\n");
			printf(" 24 - L7- Eroziune\n");
			printf(" 25 - L7- Deschidere\n");
			printf(" 26 - L7- Inchidere\n");
			printf(" 27 - L7- Dilatare n ori\n");
			printf(" 28 - L7- Eroziune n ori\n");
			printf(" 29 - L7- Contur\n");
			printf(" 30 - L7- Umplere\n");
			printf(" 31 - L8- Histograma, media divizatia standard\n");
			printf(" 32 - L8- Binarizare cu prag adaptiv\n");
			printf(" 33 - L8- Negativul imaginii\n");
			printf(" 34 - L8- Modificare luminozitate\n");
			printf(" 35 - L8- Modificare constrast\n");
			printf(" 36 - L8- Corectia Gamma\n");
			printf(" 37 - L8- Egalizarea histogramei\n");
			printf(" 38 - L9- Filtru medie aritmetica (3x3)\n");
			printf(" 39 - L9- Filtru medie aritmetica (5x5)\n");
			printf(" 40 - L9- Filtru Gaussian\n");
			printf(" 41 - L9- Filtru Lagrange\n");
			printf(" 42 - L9- Filtru trece sus\n");
			printf(" 43 - L9- Log magn sp Fourier\n");
			printf(" 44 - L9- FTJ ideal\n");
			printf(" 45 - L9- FTS ideal\n");
			printf(" 46 - L9- FTJ taiere Gauss\n");
			printf(" 47 - L9- FTS taiere Gauss\n");
			printf(" 48 - L10- Filtru Median\n");
			printf(" 49 - L10- Filtru Gauss 1x2D\n");
			printf(" 50 - L10- Filtru Gauss 2x1D\n");
			printf(" 51 - L11- Detectie muchii canny\n");
			printf(" 0 - Exit\n\n");
			printf("Option: ");
			scanf("%d", &op);
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
				aditivNiveluriGri();
				break;
			case 11:
				multiplicativNiveluriGri();
				break;
			case 12:
				sferturiColorate();
				break;
			case 13:
				createMatrix();
				break;
			case 14:
				canaleRGB();
				break;
			case 15:
				color2gray();
				break;
			case 16:
				graytoblackwhite();
				break;
			case 17:
				RGBtoHSV();
				break;
			case 18:
				proprietati();
				break;
			case 19:
				p1();
				break;
			case 20:
				p2();
				break;
			case 21:
				contur();
				break;
			case 22:
				reconstructie();
				break;
			case 23:
				dilatare();
				break;
			case 24:
				eroziune();
				break;
			case 25:
				deschidere();
				break;
			case 26:
				inchidere();
				break;
			case 27:
				dilatareN();
				break;
			case 28:
				eroziuneN();
				break;
			case 29:
				conturL7();
			case 30:
				umplere();
				break;
			case 31:
				histograma();
				break;
			case 32:
				binarizare();
				break;
			case 33:
				negativulImaginii();
				break;
			case 34:
				luminozitate();
				break;
			case 35:
				contrast();
				break;
			case 36:
				corectieGamma();
				break;
			case 37:
				egalizare();
				break;
			case 38:
				medie3x3();
				break;
			case 39:
				medie5x5();
				break;
			case 40:
				filtruGauss();
				break;
			case 41:
				filtruLaplace();
				break;
			case 42:
				filtruTS();
				break;
			case 43:
				Fourier();
				break;
			case 44:
				FTJIdeal();
				break;
			case 45:
				FTSIdeal();
				break;
			case 46:
				FTJGauss();
				break;
			case 47:

				break;
			case 48:
				filtruMedian();
				break;
			case 49:
				filtruGauss1x2();
				break;
			case 50:
				filtruGauss2x1();
				break;
			case 51:
				detectieMuchiiCanny();
				break;
			}
		} while (op != 0);
		return 0;
	}