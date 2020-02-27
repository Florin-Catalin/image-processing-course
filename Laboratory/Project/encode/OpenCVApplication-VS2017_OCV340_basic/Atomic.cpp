#include "stdafx.h"
#include "Atomic.h"
#include "Utils.h"
#include <fstream>

using namespace cv;

#pragma region Atomic
#pragma region AtomicForward

Mat apply_shift_to_zero(Mat *src)
{
	Mat dst = Mat(MCU_Y, MCU_X, CV_8SC1);
	for (int i = 0; i < MCU_Y; i++)
	{
		for (int j = 0; j < MCU_X; j++)
		{
			dst.at<char>(i, j) = src->at<uchar>(i, j) - MIDDLE_LEVEL;
		}
	}
	return dst;
}

Mat apply_dct_transform(Mat *src)
{
	Mat dst = Mat(MCU_Y, MCU_X, CV_32FC1);
	for (int u = 0; u < MCU_Y; u++)
	{
		for (int v = 0; v < MCU_X; v++)
		{
			float sum = 0;
			for (int x = 0; x < MCU_Y; x++)
			{
				for (int y = 0; y < MCU_X; y++)
				{
					sum += src->at<char>(x, y) * cos((2 * x + 1) * u * CV_PI / 16) * cos((2 * y + 1) * v * CV_PI / 16);
				}
			}
			sum = (u == 0) ? sum / sqrt(2) : sum;
			sum = (v == 0) ? sum / sqrt(2) : sum;
			dst.at<float>(u, v) = sum / 4;
		}
	}
	return dst;
}

Mat apply_standard_quantization(Mat *src)
{
	Mat dst = Mat(MCU_Y, MCU_X, CV_32SC1);
	for (int i = 0; i < MCU_Y; i++)
	{
		for (int j = 0; j < MCU_X; j++)
		{
			dst.at<int>(i, j) = nearbyint(src->at<float>(i, j) / standard_quant[i][j]);
		}
	}
	return dst;
}

std::vector<RleElement> apply_rle(Mat* mcu)
{
	//Traversing in zigzag starting with the first AC element, at (0, 1)
	int height = mcu->rows;
	int width = mcu->cols;
	int i = 0;
	int j = 1;
	bool going_down = true;
	int zero_cnt = 0;
	std::vector<RleElement> rle;
	while (i < height && j < width)
	{
		//Computations on current index 
		if (mcu->at<int>(i, j) == 0)
		{
			zero_cnt++;
		}
		else
		{
			rle.push_back(RleElement(clamp_to_char(zero_cnt), clamp_to_char(mcu->at<int>(i, j))));
			zero_cnt = 0;
		}
		//Determine next step
		//Next step E2
		if (i == height - 1 && j % 2 == 0)
		{
			j++;
			continue;
		}
		//Next step NE2
		if (i == height - 1 && j % 2 == 1)
		{
			j++;
			i--;
			going_down = false;
			continue;
		}
		//Next step S2
		if (j == width - 1 && i % 2 == 1)
		{
			i++;
			continue;
		}
		//Next step SW2
		if (j == width - 1 && i % 2 == 0)
		{
			j--;
			i++;
			going_down = true;
			continue;
		}
		//Next step SW
		if (i == 0 && j % 2 == 1)
		{
			i++;
			j--;
			going_down = true;
			continue;
		}
		//Next step S
		if (j == 0 && i % 2 == 1)
		{
			i++;
			continue;
		}
		//Next step NE
		if (j == 0 && i % 2 == 0)
		{
			j++;
			i--;
			going_down = false;
			continue;
		}
		//Next step E1
		if (i == 0 && j % 2 == 0)
		{
			j++;
			continue;
		}

		//If no direction change needs te be made
		//Going down
		if (going_down)
		{
			i++;
			j--;
		}
		else
		{
			i--;
			j++;
		}
	}

	return rle;
}

std::vector<McuRle> rl_encode_all(std::vector<std::vector<Mat>> mcus)
{
	std::vector<McuRle> mcu_rle;
	for (std::vector<Mat> row : mcus)
	{
		for (Mat mcu : row)
		{
			mcu_rle.push_back(McuRle(mcu.at<int>(0, 0), apply_rle(&mcu)));
		}
	}
	return mcu_rle;
}
#pragma endregion


#pragma region AtomicBackward
Mat bw_apply_shift_to_zero(Mat *src)
{
	Mat dst = Mat(MCU_Y, MCU_X, CV_8UC1);
	for (int i = 0; i < MCU_Y; i++)
	{
		for (int j = 0; j < MCU_X; j++)
		{
			dst.at<uchar>(i, j) = src->at<char>(i, j) + MIDDLE_LEVEL;
		}
	}
	return dst;
}

Mat bw_apply_dct_transform(Mat *src)
{
	Mat dst = Mat(MCU_Y, MCU_X, CV_8SC1);
	for (int x = 0; x < MCU_Y; x++)
	{
		for (int y = 0; y < MCU_X; y++)
		{
			float sum = 0;
			for (int u = 0; u < MCU_Y; u++)
			{
				for (int v = 0; v < MCU_X; v++)
				{
					float psum = src->at<int>(u, v) * cos((2 * x + 1) * u * CV_PI / 16) * cos((2 * y + 1) * v * CV_PI / 16);
					psum = (u == 0) ? psum / sqrt(2) : psum;
					psum = (v == 0) ? psum / sqrt(2) : psum;
					sum += psum;
				}
			}
			dst.at<char>(x, y) = clamp_to_char(nearbyint(sum / 4));
		}
	}
	return dst;
}

Mat bw_apply_standard_quantization(Mat *src)
{
	Mat dst = Mat(MCU_Y, MCU_X, CV_32SC1);
	for (int i = 0; i < MCU_Y; i++)
	{
		for (int j = 0; j < MCU_X; j++)
		{
			dst.at<int>(i, j) = src->at<int>(i, j) * standard_quant[i][j];
		}
	}
	return dst;
}

std::vector<Mat> convert_rgb_to_ycrcb_chan(Mat* src)
{
	Mat ycrcb;
	Mat chan[3];
	cvtColor(*src, ycrcb, CV_BGR2YCrCb);
	split(ycrcb, chan);
	std::vector<Mat> mats;
	mats.push_back(chan[0]);
	mats.push_back(chan[1]);
	mats.push_back(chan[2]);
	return mats;
}

Mat convert_ycrcb_chan_to_rgb(std::vector<Mat> chan)
{
	Mat ycrcb, rgb;
	merge(chan, ycrcb);
	cvtColor(ycrcb, rgb, CV_YCrCb2BGR);
	return rgb;
}

std::vector<std::vector<Mat>> divide_channel_into_mcu(Mat* src)
{
	std::vector<std::vector<Mat>> divided;
	int rowi = 0;
	int coli = 0;
	while (rowi < src->rows)
	{
		std::vector<Mat> row_of_mats;
		coli = 0;
		while (coli < src->cols)
		{
			Mat element = Mat(MCU_Y, MCU_X, CV_8UC1);
			for (int i = 0; i < MCU_Y; i++)
			{
				for (int j = 0; j < MCU_X; j++)
				{
					int ni = rowi + i;
					int nj = coli + j;
					element.at<uchar>(i, j) = (ni >= src->rows || nj >= src->cols) ? 0 : src->at<uchar>(ni, nj);
				}
			}
			row_of_mats.push_back(element);
			coli += MCU_X;
		}
		divided.push_back(row_of_mats);
		rowi += MCU_Y;
	}

	return divided;
}



std::vector<std::vector<Mat>> forward_transform_mcus(std::vector<std::vector<Mat>> mcus)
{
	std::vector<std::vector<Mat>> ret_mcus;
	for (std::vector<Mat> row_of_mcu : mcus)
	{
		std::vector<Mat> row;
		for (Mat mcu : row_of_mcu)
		{
			Mat rmcu1 = apply_shift_to_zero(&mcu);
			Mat rmcu2 = apply_dct_transform(&rmcu1);
			Mat rmcu3 = apply_standard_quantization(&rmcu2);
			row.push_back(rmcu3);
		}
		ret_mcus.push_back(row);
	}
	return ret_mcus;
}

std::vector<std::vector<Mat>> backward_transform_mcus(std::vector<std::vector<Mat>> mcus)
{
	std::vector<std::vector<Mat>> ret_mcus;
	int ki = 0;
	for (std::vector<Mat> row_of_mcu : mcus)
	{
		std::vector<Mat> row;
		int kj = 0;
		for (Mat mcu : row_of_mcu)
		{
			Mat rmcu1 = bw_apply_standard_quantization(&mcu);
			Mat rmcu2 = bw_apply_dct_transform(&rmcu1);
			Mat rmcu3 = bw_apply_shift_to_zero(&rmcu2);

			if (ki == 0 && kj == 1)
			{
				cout << endl;
				cout << ki << ',' << kj << endl;
				cout << rmcu1 << endl;
				cout << rmcu2 << endl;
				cout << rmcu3 << endl;
				
				//cout << (int)rmcu1.at<int>(1, 0) << endl;
				//cout << (int)rmcu2.at<char>(1, 0) << endl;
				//cout << (int)rmcu3.at<uchar>(1, 0) << endl;
			}
			row.push_back(rmcu3);
			kj++;
		}
		ret_mcus.push_back(row);
		ki++;
	}

	return ret_mcus;
}


void print_division2(std::vector<std::vector<Mat>> mcus)
{
	int i = 0, j = 0;
	for (std::vector<Mat> row_of_mats : mcus)
	{
		for (Mat mcu : row_of_mats)
		{
			std::cout << i << " " << j << std::endl;
			std::cout << mcu << std::endl;
			j += MCU_X;
		}
		i += MCU_Y;
	}
}

Mat bw_apply_rle(std::vector<RleElement> rle)
{
	Mat mcu = Mat::zeros(MCU_Y, MCU_X, CV_32SC1);

	int height = mcu.rows;
	int width = mcu.cols;
	int i = 0;
	int j = 1;
	bool going_down = true;
	int zero_cnt = 0;
	int i_rle = 0;
	RleElement current = rle.at(i_rle);
	while (i_rle < rle.size())
	{
		//Computations on current index 
		if (current.zeros_before == 0)
		{
			mcu.at<int>(i, j) = current.number;
			i_rle++;
			if (i_rle < rle.size())
			{
				current = rle.at(i_rle);
			}
		}
		else
		{
			current.zeros_before--;
		}
		//Determine next step
		//Next step E2
		if (i == height - 1 && j % 2 == 0)
		{
			j++;
			continue;
		}
		//Next step NE2
		if (i == height - 1 && j % 2 == 1)
		{
			j++;
			i--;
			going_down = false;
			continue;
		}
		//Next step S2
		if (j == width - 1 && i % 2 == 1)
		{
			i++;
			continue;
		}
		//Next step SW2
		if (j == width - 1 && i % 2 == 0)
		{
			j--;
			i++;
			going_down = true;
			continue;
		}
		//Next step SW
		if (i == 0 && j % 2 == 1)
		{
			i++;
			j--;
			going_down = true;
			continue;
		}
		//Next step S
		if (j == 0 && i % 2 == 1)
		{
			i++;
			continue;
		}
		//Next step NE
		if (j == 0 && i % 2 == 0)
		{
			j++;
			i--;
			going_down = false;
			continue;
		}
		//Next step E1
		if (i == 0 && j % 2 == 0)
		{
			j++;
			continue;
		}

		//If no direction change needs te be made
		//Going down
		if (going_down)
		{
			i++;
			j--;
		}
		else
		{
			i--;
			j++;
		}
	}

	return mcu;
}

Mat assemble_mcu_into_channel(std::vector<std::vector<Mat>> mcus)
{
	int row_cnt = mcus.size();
	int col_cnt = mcus.at(0).size();
	Mat dst = Mat(row_cnt * MCU_Y, col_cnt * MCU_Y, CV_8UC1);
	for (int i = 0; i < row_cnt; i++)
	{
		for (int j = 0; j < col_cnt; j++)
		{
			Mat mcu = mcus.at(i).at(j);
			mcu.copyTo(dst(Rect(j * MCU_X, i * MCU_Y, MCU_X, MCU_Y)));
		}
	}
	return dst;
}


std::vector<std::vector<Mat>> rl_decode_all(std::vector<McuRle> mcu_rles, int row_cnt, int col_cnt)
{
	std::vector<std::vector<Mat>> mcus;
	int cnt = 0;
	for (int i = 0; i < row_cnt; i++)
	{
		std::vector<Mat> row;
		for (int j = 0; j < col_cnt; j++)
		{
			McuRle mcu_rle = mcu_rles.at(cnt);
			cnt++;
			Mat mcu;
			if (!mcu_rle.ac_coeffs.empty())
			{
				mcu = bw_apply_rle(mcu_rle.ac_coeffs);
			}
			else
			{
				mcu = Mat::zeros(MCU_Y, MCU_X, CV_32SC1);
			}
			mcu.at<int>(0, 0) = mcu_rle.dc_coeff;
			row.push_back(mcu);
		}
		mcus.push_back(row);
	}
	return mcus;
}



#pragma endregion
float calculate_compression(char* fname1, char* fname2)
{
	float f1 = get_file_size(fname1);
	float f2 = get_file_size(fname2);

	return (f1 > f2) ? f1 / f2 : f2 / f1;
}
#pragma endregion

