#include "stdafx.h"
#include "EncodedImage.h"


EncodedImage::EncodedImage(Mat *src)
{
	this->src = src;
	//Convert from BGR to YCrCb
	vector<Mat> chan = convert_rgb_to_ycrcb_chan(src);

	//Divide each channel into matrix of MCU
	vector<vector<Mat>> y_mcus = divide_channel_into_mcu(&chan.at(0));
	vector<vector<Mat>> cr_mcus = divide_channel_into_mcu(&chan.at(1));
	vector<vector<Mat>> cb_mcus = divide_channel_into_mcu(&chan.at(2));

	//Do the transformations for all of these
	vector<vector<Mat>> y_mcus2 = forward_transform_mcus(y_mcus);
	vector<vector<Mat>> cr_mcus2 = forward_transform_mcus(cr_mcus);
	vector<vector<Mat>> cb_mcus2 = forward_transform_mcus(cb_mcus);

	//Encode all MCUs
	this->y_mcu_rles = rl_encode_all(y_mcus2);
	this->cr_mcu_rles = rl_encode_all(cr_mcus2);
	this->cb_mcu_rles = rl_encode_all(cb_mcus2);
}

void EncodedImage::write_to_file(char* fname)
{
	ofstream ofs;
	ofs.open(fname, ios::out | ios::binary);
	//ofs << this->src->rows;
	ofs.write(reinterpret_cast<const char *>(&this->src->rows), sizeof(this->src->rows));
	//ofs << this->src->cols;
	ofs.write(reinterpret_cast<const char *>(&this->src->cols), sizeof(this->src->cols));
	
	write_to_file_one_channel(this->y_mcu_rles, ofs);
	write_to_file_one_channel(this->cr_mcu_rles, ofs);
	write_to_file_one_channel(this->cb_mcu_rles, ofs);

	ofs.close();
}

void EncodedImage::write_to_file_one_channel(vector<McuRle> mcu_rles, ofstream& ofs)
{
	int size = mcu_rles.size();
	ofs.write(reinterpret_cast<const char *>(&size), sizeof(size));
	//ofs << mcu_rles.size();
	for (McuRle mcu_rle : mcu_rles)
	{
		ofs.write(reinterpret_cast<const char *>(&mcu_rle.dc_coeff), sizeof(mcu_rle.dc_coeff));
		//ofs << mcu_rle.dc_coeff;
		int coeff_size = mcu_rle.ac_coeffs.size();
		ofs.write(reinterpret_cast<const char *>(&coeff_size), sizeof(coeff_size));
		//ofs << mcu_rle.ac_coeffs.size();
		for(RleElement pair : mcu_rle.ac_coeffs)
		{
			ofs.write(reinterpret_cast<const char *>(&pair.zeros_before), sizeof(pair.zeros_before));
			ofs.write(reinterpret_cast<const char *>(&pair.number), sizeof(pair.number));
			//ofs << pair.x << pair.y;
		}
	}
}


