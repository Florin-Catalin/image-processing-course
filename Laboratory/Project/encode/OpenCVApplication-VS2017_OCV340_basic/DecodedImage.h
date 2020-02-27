#pragma once
#include "common.h"
#include "McuRle.h"

class DecodedImage
{
public:
	Mat src;
	vector<McuRle> y_mcu_rles;
	vector<McuRle> cr_mcu_rles;
	vector<McuRle> cb_mcu_rles;


	DecodedImage(char *fname);
	~DecodedImage() = default;
};

