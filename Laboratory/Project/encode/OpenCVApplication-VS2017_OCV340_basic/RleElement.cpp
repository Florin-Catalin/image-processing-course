#include "stdafx.h"
#include "RleElement.h"


RleElement::RleElement(char zb, char nb)
{
	this->zeros_before = zb;
	this->number = nb;
}

std::ostream& operator<<(std::ostream& os, const RleElement& rleE)
{
	os << '(' << (int)rleE.zeros_before << ',' << (int)rleE.number << ')';
	return os;
}
