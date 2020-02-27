// OpenCVApplication.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "common.h"
#include "Atomic.h"
#include "Utils.h"
#include "EncodedImage.h"
#include "DecodedImage.h"


using namespace std;

#pragma region Test
void OpenTest()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		cout << "Original image: " << endl;
		cout << img << endl;

		Mat shifted = apply_shift_to_zero(&img);
		cout << "After shifting: " << endl;
		cout << shifted << endl;

		Mat dct = apply_dct_transform(&shifted);
		cout << "After dct: " << endl;
		cout << dct << endl;

		Mat quant = apply_standard_quantization(&dct);
		cout << "After quant :" << endl;
		cout << quant << endl;
		waitKey(0);

		Mat bquant = bw_apply_standard_quantization(&quant);
		cout << "After bw quant :" << endl;
		cout << bquant << endl;

		Mat bdct = bw_apply_dct_transform(&bquant);
		cout << "After bw dct :" << endl;
		cout << bdct << endl;

		Mat bshift = bw_apply_shift_to_zero(&bdct);
		cout << "After bw shft :" << endl;
		cout << bshift << endl;

		write_to_new_file(&bshift);
	}
}

Mat CreateTest8x8()
{
	uchar mat[MCU_X][MCU_Y] = {
		{52,55,61,66,70,61,64,73},
		{63,59,55,90,109,85,69,72},
		{62,59,68,113,144,104,66,73},
		{63,58,71,122,154,106,70,69},
		{67,61,68,104,126,88,68,70},
		{79,65,60,70,77,68,58,75},
		{85,71,64,59,55,61,65,83},
		{87,79,69,68,65,76,78,94}
	};

	Mat img = Mat(MCU_X, MCU_Y, CV_8UC1);
	for (int i = 0; i < MCU_Y; i++)
	{
		for (int j = 0; j < MCU_X; j++)
		{
			img.at<uchar>(i, j) = mat[i][j];
		}
	}
	return img;
}

void SaveTest()
{
	Mat img = CreateTest8x8();
	write_to_new_file(&img);
}

void TestConvertYCbCrAndBack()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		imshow("src", src);
		vector<Mat> chan = convert_rgb_to_ycrcb_chan(&src);
		imshow("Y", chan.at(0));
		imshow("Cr", chan.at(1));
		imshow("Cb", chan.at(2));
		Mat dst = convert_ycrcb_chan_to_rgb(chan);
		imshow("dst", dst);
		waitKey(0);
	}
}

void TestSmallDivide()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_COLOR);
		imshow("img", img);
		vector<Mat> chan = convert_rgb_to_ycrcb_chan(&img);
		imshow("Y", chan.at(0));
		imshow("Cr", chan.at(1));
		imshow("Cb", chan.at(2));
		vector<vector<Mat>> y_mcus = divide_channel_into_mcu(&chan.at(0));
		print_division2(y_mcus);
		Mat assemble = assemble_mcu_into_channel(y_mcus);
		cout << assemble << endl;
		waitKey(0);
	}
}

void TestRle()
{
	char fname[MAX_PATH];
	while (openFileDlg(fname))
	{
		Mat img = imread(fname, CV_LOAD_IMAGE_GRAYSCALE);
		cout << "Original image: " << endl;
		cout << img << endl;

		Mat shifted = apply_shift_to_zero(&img);
		cout << "After shifting: " << endl;
		cout << shifted << endl;

		Mat dct = apply_dct_transform(&shifted);
		cout << "After dct: " << endl;
		cout << dct << endl;

		Mat quant = apply_standard_quantization(&dct);
		cout << "After quant :" << endl;
		cout << quant << endl;

		vector<RleElement> rle = apply_rle(&quant);
		for (RleElement rleE : rle)
		{
			cout << rleE << endl;
		}

		Mat rle_back = bw_apply_rle(rle);
		cout << rle_back << endl;

		waitKey(0);
	}
}

void TestConstr()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		Mat src = imread(fname, CV_LOAD_IMAGE_COLOR);
		imshow("src", src);
		EncodedImage eim = EncodedImage(&src);
		for (RleElement rleE : eim.y_mcu_rles.at(0).ac_coeffs)
		{
			cout << rleE << endl;
		}
		open_new_file(fname);
		eim.write_to_file(fname);
	}
}

void TestRead()
{
	char fname[MAX_PATH];
	while(openFileDlg(fname))
	{
		DecodedImage dec = DecodedImage(fname);
		write_to_new_file(&dec.src);
		imshow("image", dec.src);
		waitKey(0);
	}
}

void TestCompressionCalculation()
{
	char fname1[MAX_PATH];
	char fname2[MAX_PATH];
	openFileDlg(fname1);
	openFileDlg(fname2);

	float rate = calculate_compression(fname1, fname2);
	cout << rate << ":1" << endl;
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
		printf(" 1 - OpenTest\n\n");
		printf(" 2 - Save CreateTest8x8\n\n");
		printf(" 3 - TestConvertYCbCrAndBack\n\n");
		printf(" 4 - TestDiv\n\n");
		printf(" 5 - TestRle\n\n");
		printf(" 6 - TestConstr\n\n");
		printf(" 7 - TestRead\n\n");
		printf(" 8 - CompressionRate\n\n");
		printf("Option: ");
		scanf("%d", &op);
		//op = 7;
		switch (op)
		{
		case 1:
			OpenTest();
			break;
		case 2:
			SaveTest();
			break;
		case 3:
			TestConvertYCbCrAndBack();
			break;
		case 4:
			TestSmallDivide();
			break;
		case 5:
			TestRle();
			break;
		case 6:
			TestConstr();
			break;
		case 7:
			TestRead();
			break;
		case 8:
			TestCompressionCalculation();
			break;
		default:
			system("cls");
			break;
		}
	} while (op != 0);
	return 0;
}