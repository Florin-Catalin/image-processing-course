#pragma once
#include <ostream>

class RleElement
{
public:
	char zeros_before;
	char number;
	RleElement(char zb, char nb);
	RleElement() = default;
	~RleElement() = default;
	friend std::ostream& operator<<(std::ostream& os, const RleElement& rleE);
};

