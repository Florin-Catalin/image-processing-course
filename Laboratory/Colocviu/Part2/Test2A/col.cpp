int height_rect;
int width_rect;

Mat create_kernel_matrix(Mat src)
{
	Mat_<float> kernel_mat(src.rows, src.cols);


	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (i > src.rows / 2 - height_rect / 2
				&& j > src.cols / 2 - width_rect / 2
				&& i < src.rows / 2 + height_rect / 2
				&& j < src.cols / 2 + width_rect / 2
				)
				kernel_mat(i, j) = 1;
			else kernel_mat(i, j) = 0;
		}


	imshow("kernel_matrix", kernel_mat);
	waitKey(0);

	return kernel_mat;
}

void centering_transform(Mat img){
	//expects floating point image
	for (int i = 0; i < img.rows; i++){
		for (int j = 0; j < img.cols; j++){
			img.at<float>(i, j) = ((i + j) & 1) ? -img.at<float>(i, j) : img.at<float>(i, j);
		}
	}
}

Mat generic_frequency_domain_filter(Mat src){
	//convert input image to float image
	Mat srcf;
	src.convertTo(srcf, CV_32FC1);
	//centering transformation
	centering_transform(srcf);


	Mat fourier;
	dft(srcf, fourier, DFT_COMPLEX_OUTPUT);
	//split into real and imaginary channels
	Mat channels[] = { Mat::zeros(src.size(), CV_32F), Mat::zeros(src.size(), CV_32F) };
	split(fourier, channels); // channels[0] = Re(DFT(I)), channels[1] = Im(DFT(I))
	//calculate magnitude and phase in floating point images mag and phi
	Mat mag, phi;
	magnitude(channels[0], channels[1], mag);
	phase(channels[0], channels[1], phi);
	//display the phase and magnitude images here
	// ......
	//insert filtering operations on Fourier coefficients here
	Mat kernel_matrix =  create_kernel_matrix(src);

	imshow("Original Magnitude", mag);
	waitKey(0);

	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (kernel_matrix.at<float>(i, j) == 1)
			{
				mag.at<float>(i, j) = 0;
			}
		}

	imshow("Filtered Magnitude", mag);
	waitKey(0);

	//store in real part in channels[0] and imaginary part in channels[1]
	
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			channels[0].at<float>(i, j) = mag.at<float>(i, j)  * cos(phi.at<float>(i, j));
			channels[1].at<float>(i, j) = mag.at<float>(i, j)  * sin(phi.at<float>(i, j));
		}


	//perform inverse transform and put results in dstf
	Mat dst, dstf;
	merge(channels, 2, fourier);
	dft(fourier, dstf, DFT_INVERSE | DFT_REAL_OUTPUT | DFT_SCALE);
	//inverse centering transformation
	centering_transform(dstf);
	//normalize the result and put in the destination image
	normalize(dstf, dst, 0, 255, NORM_MINMAX, CV_8UC1);
	//Note: normalizing distorts the resut while enhancing the image display in the range [0,255].
	//For exact results (see Practical work 3) the normalization should be replaced with convertion:
	//dstf.convertTo(dst, CV_8UC1);

	imshow("P1. Result Image", dst);
	waitKey(0);


	return dst;


}



// problem 2



Mat_<float> generate_diamond_kernel(int width)
{
	Mat_<float> kernel(2 * width+1 , 2 * width+1 );

	bool asc = true;
	int counter = 0;


	for (int i = 0; i < 2 * width + 1; i++)
	{
		for (int j = 0; j < 2 * width + 1; j++)
		{
			if (j == width - counter || j == width + counter)
			{
				kernel(i, j) = -1;
			}
			else kernel(i, j) = 0;

			
		}

		if (asc)
			counter++;
		else counter--;


		if (counter == width) asc = false;

	}
	kernel(width, width) = 8;

	return kernel;

}

bool isInside(Mat src, int i, int j)
{
	if (i >= 0 && j >= 0 && i < src.rows && j < src.cols) return true;
	else return false;
}

// return the coords for the closest pixel if outside
// return -1,-1 if inside
pair<int,int> isInsideProb3(Mat src, int i, int j)
{
	if (i >= 0 && j >= 0 && i < src.rows && j < src.cols) return pair<int,int>(i,j);
	
	int ii = i, jj = j;

	if (i < 0) ii = 0;
	if (j < 0) jj = 0;

	if (i >= src.rows) ii = src.rows - 1;
	if (j >= src.cols) jj = src.cols - 1;

    return  pair<int, int>(ii,jj);
}

#define MODE_PROB2 0
#define MODE_PROB3 1
#define MODE_PROB3C 2

Mat_<uchar> convolution(Mat_<uchar> src, int kernel_size, int MODE)
{
	// shrink kernel to speed up
	if (kernel_size != 1 && MODE == MODE_PROB3C) kernel_size/=2;

	cout << "Convolution with w=" << kernel_size << "\n";

	Mat_<float> kernel = generate_diamond_kernel(kernel_size);
    //cout << kernel;
	Mat_<uchar> dest(src.rows, src.cols);

	int sum_pos = 0;

	for (int u = 0; u < kernel.rows; u++)
		for (int v = 0; v < kernel.cols; v++)
		{
			if (kernel(u, v) > 0) sum_pos += kernel(u, v);

		}

	for (int i = 0; i < src.rows;i++)
		for (int j = 0; j < src.cols; j++)
		{
			float temp = 0;

			for (int u = 0; u < kernel.rows; u++)
				for (int v = 0; v < kernel.cols; v++)
				{
					if (MODE == 0)
					{
						if (isInside(src, i + u - kernel.rows / 2, j + v - kernel.cols / 2))
							temp += kernel(u, v) *   src(i + u - kernel.rows / 2, j + v - kernel.cols / 2);
					}
					else
					{
						pair<int, int> result = isInsideProb3(src, i + u - kernel.rows / 2, j + v - kernel.cols / 2);
						
				        temp += kernel(u, v) * src(result.first, result.second);						
					}
				}

			temp = abs(temp);
			temp = temp / sum_pos;

			dest(i, j) = temp;

		}

	return dest;
}

 

//problem 3 c
//return absolute pixel error
int compare(Mat_<uchar> src1, Mat_<uchar> src2)
{
	int error=0;
	for (int i = 0; i < src1.rows;i++)
		for (int j = 0; j < src1.cols; j++)
		{
			int a = src1(i, j);
			int b = src2(i, j);
			error += abs(a - b);
		}


	error = error / (src1.rows * src1.cols);
	return error;
}
cout << "Write rect height for problem 1\n";
	cin >> height_rect;

	cout << "Write rect width for problem 1\n";
	cin >> width_rect;


	Mat img = imread("Images/cameraman.bmp",CV_LOAD_IMAGE_GRAYSCALE);

	//problem 1 
	imshow("P1. Original img", img);
	waitKey();

	generic_frequency_domain_filter(img);

	//problem 2
	cout << generate_diamond_kernel(2);
	Mat_<uchar> result = convolution(img, 2,MODE_PROB2);

	imshow("Prob2 result", result);
	//problem 3
	//a
	convolution(img, 2, MODE_PROB3);
	//b
	convolution(img, 2, MODE_PROB3C);
	//c
	//compare function

	double t = (double)getTickCount();  
	Mat_<uchar> normal = convolution(img, 2, MODE_PROB3);
	t = ((double)getTickCount() - t) / getTickFrequency();
	// Display (in the console window) the processing time in [ms]
	printf("(normal)Time = %.3f [ms]\n", t * 1000);


    t = (double)getTickCount();
	Mat_<uchar> optimised = convolution(img, 2, MODE_PROB3C);
	t = ((double)getTickCount() - t) / getTickFrequency();
	// Display (in the console window) the processing time in [ms]
	printf("(optimised)Time = %.3f [ms]\n", t * 1000);

	cout << "Average absolute pixel error:" << compare(normal, optimised);
	waitKey(0);