//
// Created by Christoffer Wärnbring on 2019-01-14.
//

#ifndef IMU_VECTOR_H
#define IMU_VECTOR_H
#include <stdexcept>
#include <array>


class vector {
public:
	vector(const std::array<double, 3> arr);
	vector() :x(0), y(0), z(0) {};
	operator const std::array<double, 3>&();
	double X() { return x; };
	double Y() { return y; };
	double Z() { return z; };

	double & operator[](int pos) {
		switch (pos)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			throw std::invalid_argument("Pos out of range");
		}
	}
	double at(int pos) { return operator[](pos); }
	//double at(int pos) { return operator[](pos); }
	friend bool operator==(const vector & lhs,const vector & rhs) {
		return (
			lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z
			);
	}

private:
    double x; // north
    double y; // altitude
    double z; // east
};


#endif //IMU_VECTOR_H
