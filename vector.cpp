//
// Created by Christoffer Wärnbring on 2019-01-14.
//

#include "vector.h"
#include <array>

vector::vector(const std::array<double, 3> arr)
{
	x = arr.at(0);
	y = arr.at(1);
	z = arr.at(2);
}

//vector::operator const std::array<double, 3>&()
//{
//	std::array<double, 3> rtrn;
//	rtrn[0] = this->x;
//	rtrn[1] = this->y;
//	rtrn[2] = this->z;
//	return rtrn; // ERROR: Reference to stack memory returned
//}
