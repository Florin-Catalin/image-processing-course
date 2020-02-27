void median_filter(Mat kernel)
{
	char frame[MAX_PATH];
	openFileDlg(frame);
	Mat img = imread(frame, CV_LOAD_IMAGE_GRAYSCALE);
	imshow("original", img);

	Mat dst(img.rows, img.cols, CV_8UC1);

	int k = kernel.rows / 2;
	printf("%d", k);

	int a[100];

	for (int i = kernel.rows; i < (img.rows - kernel.rows); i++)
	{
		for (int j = kernel.cols; j < (img.cols - kernel.cols); j++)
		{
			int x = 0;
			for (int p = 0; p < kernel.rows; p++)
			{
				for (int l = 0; l < kernel.cols; l++)
				{
					a[x] = (int)img.at<uchar>(i + p - k, j + l - k);
					x++;
				}

			}
			int aux = 0;
			for (int ii = 0; ii < x - 1; ii++)
			{
				for (int jj = ii + 1; jj < x; jj++)
				{
					if (a[ii] > a[jj])
					{
						aux = a[ii];
						a[ii] = a[jj];
						a[jj] = aux;
					}
				}
			}

			int val = a[x / 2];
			dst.at<uchar>(i, j) = val;
		}
	}



	imshow("Final result", dst);
	waitKey(0);
}

void lgaussian_1x2D(float sigma)
{
	char frame[MAX_PATH];
	openFileDlg(frame);
	Mat img = imread(frame, CV_LOAD_IMAGE_GRAYSCALE);
	imshow("initial", img);

	double t = (double)getTickCount();

	Mat dst(img.rows, img.cols, CV_8UC1);
	Mat g(img.rows, img.cols, CV_32FC1);



	int k = (6 * sigma) / 2;
	//printf("%d", k);

	int dimensiune = 6 * sigma;
	if (dimensiune % 2 == 0)
	{
		dimensiune++;
	}

	//calculate nucleus
	int pi = 3.14;
	float sum = 0.0f;
	for (int i = 0; i < dimensiune; i++)
	{
		for (int j = 0; j < dimensiune; j++)
		{
			g.at<float>(i, j) = (exp(-((pow(i - dimensiune / 2, 2) + pow(j - dimensiune / 2, 2)) / (2 * sigma*sigma))) / (2 * pi*sigma*sigma));
			sum = sum + g.at<float>(i, j);
		}
	}
	printf("%f \n", sum);

	for (int i = dimensiune; i < img.rows - dimensiune; i++)
	{
		for (int j = dimensiune; j < img.cols - dimensiune; j++)
		{
			int aux = 0;
			for (int p = 0; p < dimensiune; p++)
			{
				for (int l = 0; l < dimensiune; l++)
				{
					aux += g.at<float>(p, l) * img.at<uchar>(i + p - 1, j + l - 1);
				}
			}
			dst.at<uchar>(i, j) = aux;
		}
	}

	t = ((double)getTickCount() - t) / getTickFrequency();
	printf("Time = %.3f [ms]\n", t * 1000);

	imshow("dst", dst);
	waitKey(0);

}

void lgaussian_2x1D(float sigma)
{
	char frame[MAX_PATH];
	openFileDlg(frame);
	Mat img = imread(frame, CV_LOAD_IMAGE_GRAYSCALE);
	imshow("initial", img);

	double t = (double)getTickCount();

	Mat dst(img.rows, img.cols, CV_8UC1);
	Mat temp(img.rows, img.cols, CV_8UC1);

	int k = (6 * sigma) / 2;
	printf("%d", k);

	int dimensiune = 6 * sigma;
	if (dimensiune % 2 == 0)
	{
		dimensiune++;
	}

	float *Gx = (float*)malloc(sizeof(float)*dimensiune);
	float *Gy = (float*)malloc(sizeof(float)*dimensiune);

	int pi = 3.14;
	float sum = 0.0f;

	for (int i = 0; i < dimensiune; i++)
	{
		Gx[i] = (exp(-((pow(i - dimensiune / 2, 2)) / (2 * sigma*sigma)))) / ((sqrt(2 * pi)) *sigma);
		Gy[i] = (exp(-((pow(i - dimensiune / 2, 2)) / (2 * sigma*sigma)))) / ((sqrt(2 * pi)) *sigma);
		sum = Gx[i] + Gx[i];
	}
	//printf("%f\n", sum);

	int aux;
	for (int i = dimensiune; i < img.rows - dimensiune; i++)
	{
		for (int j = dimensiune; j < img.cols - dimensiune; j++)
		{
			int aux = 0;
			for (int p = 0; p < dimensiune; p++)
			{
				aux += img.at<uchar>(i + p - 1, j + p - 1) * Gx[p];
			}
			temp.at<uchar>(i, j) = aux;
		}
	}

	//aux = 0;
	for (int i = dimensiune; i < img.rows - dimensiune; i++)
	{
		for (int j = dimensiune; j < img.cols - dimensiune; j++)
		{
			int aux = 0;
			for (int p = 0; p < dimensiune; p++)
			{
				aux += temp.at<uchar>(i + p - 1, j + p - 1) * Gy[p];
			}
			dst.at<uchar>(i, j) = aux;
		}
	}

	t = ((double)getTickCount() - t) / getTickFrequency();
	printf("Time = %.3f [ms]\n", t * 1000);

	imshow("dst", dst);
	waitKey(0);
}

void centeringTransform(Mat img) {
	int height = img.rows;
	int width = img.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}

void magPhiToReIm(Mat mag, Mat phi, Mat re, Mat im) {
	int height = mag.rows;
	int width = mag.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			im.at<float>(i, j) = sin(phi.at<float>(i, j))*mag.at<float>(i, j);
			re.at<float>(i, j) = cos(phi.at<float>(i, j))*mag.at<float>(i, j);
		}
	}
}

Mat lowPassFourier(Mat mag, float r) {
	Mat dst(mag.rows, mag.cols, CV_32FC1);
	int height = mag.rows;
	int width = mag.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float rez = (height / 2.0 - i)*(height / 2.0 - i) + (width / 2.0 - j)*(width / 2.0 - j);
			float r2 = r * r;
			if (rez <= r2) {
				dst.at<float>(i, j) = mag.at<float>(i, j);
			}
			else {
				dst.at<float>(i, j) = 0;
			}
		}
	}
	return dst;
}

Mat highPassFourier(Mat mag, float r) {
	Mat dst(mag.rows, mag.cols, CV_32FC1);
	int height = mag.rows;
	int width = mag.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float rez = (height / 2.0 - i)*(height / 2.0 - i) + (width / 2.0 - j)*(width / 2.0 - j);
			float r2 = r * r;
			if (rez > r2) {
				dst.at<float>(i, j) = mag.at<float>(i, j);
			}
			else {
				dst.at<float>(i, j) = 0;
			}
		}
	}
	return dst;
}

Mat gaussianHighFourier(Mat mag) {
	Mat dst(mag.rows, mag.cols, CV_32FC1);
	int height = mag.rows;
	int width = mag.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float rez = (height / 2.0 - i)*(height / 2.0 - i) + (width / 2.0 - j)*(width / 2.0 - j);
			dst.at<float>(i, j) = mag.at<float>(i, j)*(1 - exp(-rez / 100.0));
		}
	}
	return dst;
}

Mat gaussianLowFourier(Mat mag) {
	Mat dst(mag.rows, mag.cols, CV_32FC1);
	int height = mag.rows;
	int width = mag.cols;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float rez = (height / 2.0 - i)*(height / 2.0 - i) + (width / 2.0 - j)*(width / 2.0 - j);
			dst.at<float>(i, j) = mag.at<float>(i, j)*(exp(-rez / 100.0));
		}
	}
	return dst;
}




void addLog(Mat src, Mat dst) {
	int height = src.rows;
	int width = src.cols;
	float max = -1;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst.at<float>(i, j) = log(1 + src.at<float>(i, j));
			if (dst.at<float>(i, j) > max) {
				max = dst.at<float>(i, j);
			}
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			dst.at<float>(i, j) /= max;
		}
	}

}


Mat fraquencyProcess(Mat src) {
	Mat srcf;
	Mat dst(src.rows, src.cols, CV_8UC1);

	src.convertTo(srcf, CV_32FC1);
	centeringTransform(srcf);

	//perform forward transform with complex image output
	Mat fourier;
	dft(srcf, fourier, DFT_COMPLEX_OUTPUT);

	//split into real and imaginary channels
	Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	split(fourier, channels);

	//calculate magnitude and phase in floating point images mag and phi 
	Mat mag, phi;
	magnitude(channels[0], channels[1], mag);
	phase(channels[0], channels[1], phi);

	//process magnitude and phase
	
	Mat magLog(src.rows, src.cols, CV_32FC1);
	Mat phiLog(src.rows, src.cols, CV_32FC1);
	addLog(mag, magLog);
	addLog(phi, phiLog);
	imshow("Magnitude", magLog); //asta e ok
	//imshow("Phase", phiLog);
	
	
	//mag = lowPassFourier(mag, 15);
	//imshow("Fourier low pass",mag);
	
	
	//mag = highPassFourier(mag, 15);
	//imshow("Fourier high pass", mag);
	
	mag = gaussianLowFourier(mag);
	//imshow("Fourier low pass", mag);

	//mag = gaussianHighFourier(mag);
	//imshow("Fourier high pass", mag);

	//convert back to real and imaginary
	magPhiToReIm(mag, phi, channels[0], channels[1]);

	//perform inverse transform and put results in dstf 
	Mat dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);

	//inverse centering transformation 
	centeringTransform(dstf);

	normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);

	return dst;
}

void fourierTransformWrapper()
{
	char fname[MAX_PATH];

	while (openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);


		Mat result = fraquencyProcess(src);

		imshow("input image", src);
		imshow("Fourier Filtered Image", result);
		waitKey();
	}
}
void convolutionModded(Mat_<float> &filter, Mat_<uchar> &img, Mat_<int> &output) {

	output.create(img.size());
	output.setTo(0);

	float scalingCoeff = 1;
	float additionFactor = 0;

	//TODO: decide if the filter is low pass or high pass and compute the scaling coefficient and the addition factor
	// low pass if all elements >= 0
	// high pass has elements < 0
	int pos_elem = 0;
	int neg_elem = 0;
	float pos_sum = 0;
	float neg_sum = 0;
	for (int i = 0; i < filter.rows; i++) {
		for (int j = 0; j < filter.cols; j++) {
			if (filter.at<float>(i, j) >= 0) {
				pos_elem++;
				pos_sum += filter.at<float>(i, j);
			}
			else {
				neg_elem++;
				neg_sum += filter.at<float>(i, j);
			}
		}
	}

	// compute scaling coefficient and addition factor for low pass and high pass
	// low pass: additionFactor = 0, scalingCoeff = sum of all elements
	// high pass: formula 9.20
	if (pos_elem == filter.rows*filter.rows) { //low pass
		additionFactor = 0;
		scalingCoeff = pos_sum + neg_sum;
	}
	else { // highpass
		if (pos_sum > abs(neg_sum)) {
			scalingCoeff = 2 * pos_sum;
		}
		else {
			scalingCoeff = 2 * abs(neg_sum);
		}
		additionFactor = 127;
	}


	// TODO: implement convolution operation (formula 9.2)
	// do not forget to divide with the scaling factor and add the addition factor in order to have values between [0, 255]

	int di[9] = { -1,-1,-1,0,0,0,1,1,1 };
	int dj[9] = { -1,0,1,-1,0,1,-1,0,1 };


	for (int i = filter.rows / 2; i < img.rows - filter.rows / 2; i++) {
		for (int j = filter.rows / 2; j < img.cols - filter.rows / 2; j++) {
			float sum = 0;
			for (int k = 0; k < filter.rows; k++) {
				for (int l = 0; l < filter.cols; l++) {
					sum += img(i + k - filter.rows / 2, j + l - filter.cols / 2) * filter(k, l);
				}
			}

			output(i, j) = sum;
		}
	}

	std::cout << "\n\tscaling = 1/" << scalingCoeff << "  addition = " << additionFactor << "\n";

}

void convolution2(Mat_<float> &filter, Mat_<uchar> &img, Mat_<uchar> &output) {

	output.create(img.size());
	memcpy(output.data, img.data, img.rows * img.cols * sizeof(uchar));

	float scalingCoeff = 1;
	float additionFactor = 0;

	//TODO: decide if the filter is low pass or high pass and compute the scaling coefficient and the addition factor
	// low pass if all elements >= 0
	// high pass has elements < 0
	int pos_elem = 0;
	int neg_elem = 0;
	float pos_sum = 0;
	float neg_sum = 0;
	for (int i = 0; i < filter.rows; i++) {
		for (int j = 0; j < filter.cols; j++) {
			if (filter.at<float>(i, j) >= 0) {
				pos_elem++;
				pos_sum += filter.at<float>(i, j);
			}
			else {
				neg_elem++;
				neg_sum += filter.at<float>(i, j);
			}
		}
	}

	// compute scaling coefficient and addition factor for low pass and high pass
	// low pass: additionFactor = 0, scalingCoeff = sum of all elements
	// high pass: formula 9.20
	if (pos_elem == filter.rows*filter.rows) { //low pass
		additionFactor = 0;
		scalingCoeff = pos_sum + neg_sum;
	}
	else { // highpass
		if (pos_sum > abs(neg_sum)) {
			scalingCoeff = 2 * pos_sum;
		}
		else {
			scalingCoeff = 2 * abs(neg_sum);
		}
		additionFactor = 127;
	}


	// TODO: implement convolution operation (formula 9.2)
	// do not forget to divide with the scaling factor and add the addition factor in order to have values between [0, 255]

	int di[9] = { -1,-1,-1,0,0,0,1,1,1 };
	int dj[9] = { -1,0,1,-1,0,1,-1,0,1 };


	for (int i = filter.rows / 2; i < img.rows - filter.rows / 2; i++) {
		for (int j = filter.rows / 2; j < img.cols - filter.rows / 2; j++) {
			float sum = 0;
			for (int k = 0; k < filter.rows; k++) {
				for (int l = 0; l < filter.cols; l++) {
					sum += img(i + k - filter.rows / 2, j + l - filter.cols / 2) * filter(k, l);
				}
			}

			sum = sum / scalingCoeff + additionFactor;
			output(i, j) = sum;
		}
	}

	std::cout << "\n\tscaling = 1/" << scalingCoeff << "  addition = " << additionFactor << "\n";

}

Mat Sobelx(Mat src)
{
	Mat_<float> m = (Mat_<double>(3, 3) << -1, 0, 1, -2, 0, 2, -1, 0, 1);
	Mat_<int> ret;
	convolutionModded(m, (Mat_<uchar>)src, ret);
	return ret;
}

Mat Sobely(Mat src)
{
	Mat_<float> m = (Mat_<double>(3, 3) << 1, 2, 1, 0, 0, 0, -1, -2, -1);
	Mat_<int> ret;
	convolutionModded(m, (Mat_<uchar>)src, ret);
	return ret;
}
void HorisontalVerticalComponents()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat x = Sobelx(img);
		Mat y = Sobely(img);
		imshow("x", x);
		imshow("y", y);
		waitKey(0);
	}
}

Mat getEdges()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat_<uchar> src = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		Mat_<uchar> aux = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

		//canny edge
		//gaussianFilter
		float gaussianFilterData[9] = { 1, 2, 1, 2, 4, 2, 1, 2, 1 };
		Mat_<float> gaussianFilter(3, 3, gaussianFilterData);
		convolution2(gaussianFilter, src, aux);

		Mat_<int> finSobelX(aux.rows, aux.cols),
			finSobelY(aux.rows, aux.cols);
		Mat_<float> magn(src.rows, src.cols),
			orientation(src.rows, src.cols);
		Mat_<uchar> magnNorm(src.rows, src.cols),
			fin(src.rows, src.cols);

		//canny edge
		//Sobel fillter
		finSobelX = Sobelx(aux);
		finSobelY = Sobely(aux);

		for (int i = 0; i < src.rows; i++) {
			for (int j = 0; j < src.cols; j++) {
				magn.at<float>(i, j) = sqrt(pow((float)finSobelX.at<int>(i, j), 2) +
					pow((float)finSobelY.at<int>(i, j), 2)) / sqrt(2.0);
				orientation.at<float>(i, j) = atan2((float)finSobelY.at<int>(i, j),
					(float)finSobelX.at<int>(i, j));
				if (orientation(i, j) < 0)
					orientation(i, j) = orientation(i, j) + CV_PI;
			}
		}
		//cv::normalize(magn, magnNorm, 0, 255, NORM_MINMAX, CV_8UC1);


		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				magnNorm(i, j) = magn(i, j) / (4 * sqrt(2));
			}
		}
		imshow("magnNorm", magnNorm);
		magnNorm.copyTo(fin);
		//cannyEdge
		//non-maxima suppression operation
		for (int i = 1; i < src.rows - 1; i++) {
			for (int j = 1; j < src.cols - 1; j++) {
				if ((orientation(i, j) < CV_PI / 8) && (orientation(i, j) >= 7 * CV_PI / 8) && orientation(i, j) > 0) {
					if (magnNorm(i, j) < magnNorm(i, j - 1) || (magnNorm(i, j) < magnNorm(i, j + 1)))
						fin(i, j) = 0;
				}

				if ((orientation(i, j) < 3 * CV_PI / 8) && (orientation(i, j) >= CV_PI / 8)) {
					if (magnNorm(i, j) < magnNorm(i - 1, j + 1) || (magnNorm(i, j) < magnNorm(i + 1, j - 1)))
						fin(i, j) = 0;
				}

				if ((orientation(i, j) < 5 * CV_PI / 8) && (orientation(i, j) >= 3 * CV_PI / 8)) {
					if (magnNorm(i, j) < magnNorm(i - 1, j) || (magnNorm(i, j) < magnNorm(i + 1, j)))
						fin(i, j) = 0;
				}

				if ((orientation(i, j) < 7 * CV_PI / 8) && (orientation(i, j) >= 5 * CV_PI / 8)) {
					if (magnNorm(i, j) < magnNorm(i - 1, j - 1) || (magnNorm(i, j) < magnNorm(i + 1, j + 1)))
						fin(i, j) = 0;
				}
			}
		}


		imshow("Final", fin);

		return fin;
	}
}

void edgeExtension() {
	Mat_<uchar> edges = getEdges();
	Mat_<uchar> fin(edges.rows, edges.cols);
	fin.setTo(0);

	int hist[256];

	for (int i = 0; i < 256; i++) hist[i] = 0;
	std::cout << edges.cols << "  " << edges.rows << "\n";
	for (int i = 0; i < edges.rows; i++) {
		for (int j = 0; j < edges.cols; j++) {
			uchar intensity = edges(i, j);
			hist[intensity] += 1;
		}
	}

	int nonEdgePixels = (1 - 10 / 100.0)*((edges.rows - 2)*(edges.cols - 2) - hist[0]);

	int sum = 0;
	int index = 0;
	while (sum < nonEdgePixels) {
		index++;
		sum += hist[index];
	}

	int ThresholdHigh = index;
	int ThresholdLow = 0.4*ThresholdHigh;

	std::cout << "\n\n\t high " << ThresholdHigh << "    low " << ThresholdLow << "     non edge " << nonEdgePixels << "\n";

	for (int i = 0; i < edges.rows; i++) {
		for (int j = 0; j < edges.cols; j++) {
			if (edges(i, j) > ThresholdHigh) fin(i, j) = 255;
			else if (edges(i, j) < ThresholdLow) fin(i, j) = 0;
			else fin(i, j) = 128;
		}
	}

	imshow("thresholded", fin);

	fin.copyTo(edges);
	//edges is now no, weak or strong

	std::queue<Point2i> Q;

	for (int i = 0; i < edges.rows; i++) { //found first string edge point
		for (int j = 0; j < edges.cols; j++) {
			if (edges(i, j) == 255) {
				Q.push(Point2i(i, j));
				i = edges.rows + 1;
				j = edges.cols + 1;
			}
		}
	}

	int di[8] = { -1, -1, 1,  0, 0,  1, 1, 1 };
	int dj[8] = { -1,  0, 0, -1, 1, -1, 0, 1 };

	while (!Q.empty()) {
		Point2i point = Q.front();
		Q.pop();
		for (int k = 0; k < 8; k++) {

			int ci = point.x + di[k];
			int cj = point.y + dj[k];

			if (ci >= 0 && cj >= 0 && ci < edges.rows && cj < edges.cols) {
				if (edges(ci, cj) == 128) { //if weak edge
					edges(ci, cj) = 255; //make strong edge
					Q.push(Point2i(ci, cj));
				}
			}
		}
	}

	imshow("before weak edge elimination", edges);
	//eliminate remaining weak edges
	for (int i = 0; i < edges.rows; i++) { //found first string edge point
		for (int j = 0; j < edges.cols; j++) {
			if (edges(i, j) == 128) {
				edges(i, j) = 0;
			}
		}
	}

	imshow("final edges", edges);
	waitKey(0);

}


//IN MAIN

			case 44:
				fourierTransformWrapper();
				break;

			case 45:
				//3x3 kernel
				//Mat kernel3 = createKernel("MEAN", 3);

				//5x5 kernel
				//Mat kernel5 = createKernel("MEAN", 5);
				//median_filter(kernel5);
				break;

			case 46:
				float sigma42;
				std::cout << "sigma = ";
				std::cin >> sigma42;
				lgaussian_1x2D(sigma42);
				break;

			case 47:
				float sigma;
				std::cout << "sigma = ";
				std::cin >> sigma;
				lgaussian_2x1D(sigma);
				break;

		case 49:
			HorisontalVerticalComponents();
			break;
		case 50:
			getEdges();
			waitKey(0);
			break;
		case 51:
			edgeExtension();