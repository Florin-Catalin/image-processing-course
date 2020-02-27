#pragma once
#include "common.h"
#include "Atomic.h"
#include <fstream>

class EncodedImage
{
public:
	Mat *src;
	vector<McuRle> y_mcu_rles;
	vector<McuRle> cr_mcu_rles;
	vector<McuRle> cb_mcu_rles;

	EncodedImage(Mat *src);

	void write_to_file(char *fname);
	void write_to_file_one_channel(vector<McuRle> mcu_rles, ofstream& ofs);

	~EncodedImage() = default;
};

