#pragma once
#include "common.h"
#include "RleElement.h"
#include <ostream>
#include <iterator>

using namespace std;

class McuRle
{
public:
	int dc_coeff;
	vector<RleElement> ac_coeffs;
	McuRle(int dc, vector<RleElement> ac);
	McuRle() = default;
	~McuRle() = default;

};

