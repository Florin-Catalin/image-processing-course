#include "stdafx.h"
#include "DecodedImage.h"
#include <fstream>
#include "McuRle.h"
#include "Atomic.h"

int readInt(std::ifstream& ifs)
{
	int num = 0;
	ifs.read(reinterpret_cast<char*>(&num), sizeof(num));
	return num;
}

char readByte(std::ifstream& ifs)
{
	char num = 0;
	ifs.read(reinterpret_cast<char*>(&num), sizeof(num));
	return num;
}

std::vector<McuRle> read_mcu_rles(ifstream& ifs)
{
	std::vector<McuRle> mcu_rles;
	int size = readInt(ifs);

	for(int i = 0; i < size; i++)
	{
		McuRle mcu_rle;
		mcu_rle.dc_coeff = readInt(ifs);
		int ac_coeff_size = readInt(ifs);
		for(int j = 0; j < ac_coeff_size; j++)
		{
			RleElement rleE;
			rleE.zeros_before = readByte(ifs);
			rleE.number = readByte(ifs);
			mcu_rle.ac_coeffs.push_back(rleE);
		}
		mcu_rles.push_back(mcu_rle);
	}

	return mcu_rles;
}

DecodedImage::DecodedImage(char *fname)
{
	std::ifstream ifs;
	ifs.open(fname, std::ios::in | std::ios::binary);

	int height = readInt(ifs);
	int width = readInt(ifs);
	std::cout << height << std::endl;
	std::cout << width << std::endl;

	this->y_mcu_rles = read_mcu_rles(ifs);
	this->cr_mcu_rles = read_mcu_rles(ifs);
	this->cb_mcu_rles = read_mcu_rles(ifs);

	int row_cnt = (height % MCU_Y == 0) ? height / MCU_Y : height / MCU_Y + 1;
	int col_cnt = (width % MCU_X == 0) ? width / MCU_X : width / MCU_X + 1;

	vector<vector<Mat>> y_mcus = rl_decode_all(y_mcu_rles, row_cnt, col_cnt);
	vector<vector<Mat>> cr_mcus = rl_decode_all(cr_mcu_rles, row_cnt, col_cnt);
	vector<vector<Mat>> cb_mcus = rl_decode_all(cb_mcu_rles, row_cnt, col_cnt);


	vector<vector<Mat>> y_mcus2 = backward_transform_mcus(y_mcus);
	cout << "END YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY" << endl;
	vector<vector<Mat>> cr_mcus2 = backward_transform_mcus(cr_mcus);
	vector<vector<Mat>> cb_mcus2 = backward_transform_mcus(cb_mcus);


	Mat y_chan = assemble_mcu_into_channel(y_mcus2);
	Mat cr_chan = assemble_mcu_into_channel(cr_mcus2);
	Mat cb_chan = assemble_mcu_into_channel(cb_mcus2);

	vector<Mat> chan;
	chan.push_back(y_chan);
	chan.push_back(cr_chan);
	chan.push_back(cb_chan);

	Mat img = convert_ycrcb_chan_to_rgb(chan);
	Rect ROI(0, 0, width, height);
	this->src = img(ROI);
}




