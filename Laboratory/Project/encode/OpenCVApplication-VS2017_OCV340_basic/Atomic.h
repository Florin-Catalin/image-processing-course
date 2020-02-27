#pragma once
#include "common.h"
#include "McuRle.h"
#include "RleElement.h"

#pragma region Constants
#define MCU_X 8
#define MCU_Y 8
#define MIDDLE_LEVEL 128


static uchar standard_quant[MCU_Y][MCU_X] = {
	{16,11,10,16,24,40,51,61},
	{12,12,14,19,26,58,60,55},
	{14,13,16,24,40,57,69,56},
	{14,17,22,29,51,87,80,62},
	{18,22,37,56,68,109,103,77},
	{24,35,55,64,81,104,113,92},
	{49,64,78,87,103,121,120,101},
	{72,92,95,98,112,100,103,99},
};
#pragma endregion

#pragma region Functions
Mat apply_shift_to_zero(Mat *src);
Mat apply_dct_transform(Mat *src);
Mat apply_standard_quantization(Mat *src);
Mat bw_apply_shift_to_zero(Mat *src);
Mat bw_apply_dct_transform(Mat *src);
Mat bw_apply_standard_quantization(Mat *src);
std::vector<Mat> convert_rgb_to_ycrcb_chan(Mat *src);
Mat convert_ycrcb_chan_to_rgb(std::vector<Mat> chan);
std::vector<std::vector<Mat>> divide_channel_into_mcu(Mat *src);
std::vector<std::vector<Mat>> forward_transform_mcus(std::vector<std::vector<Mat>> mcus);
std::vector<std::vector<Mat>> backward_transform_mcus(std::vector<std::vector<Mat>> mcus);
void print_division2(std::vector<std::vector<Mat>> mcus);
std::vector<RleElement> apply_rle(Mat *mcu);
Mat bw_apply_rle(std::vector<RleElement> rle);
Mat assemble_mcu_into_channel(std::vector<std::vector<Mat>> mcus);
std::vector<McuRle> rl_encode_all(std::vector<std::vector<Mat>> mcus);
std::vector<std::vector<Mat>> rl_decode_all(std::vector<McuRle> mcu_rles, int row_cnt, int col_cnt);
float calculate_compression(char *fname1, char *fname2);

#pragma endregion

