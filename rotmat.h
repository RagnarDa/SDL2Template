// Rotation matrix representation
//
// Created by Christoffer Wärnbring on 2019-01-13.
//

#ifndef IMU_ROTMAT_H
#define IMU_ROTMAT_H
#include <array>
#include "vector.h"

class rotmat {
public:
	rotmat(rotmat & copy);
    rotmat(double heading, double pitch, double roll);

    rotmat(const std::array<double, 3> &x, const std::array<double, 3> &y, const std::array<double, 3> &z);
	rotmat(
		double Xx, double Xy, double Xz
		, double Yx, double Yy, double Yz
		, double Zx, double Zy, double Zz);
	rotmat(const vector X, const vector Y, const vector Z);

    const std::array<double,3> &getX() const;
    const std::array<double,3> &getY() const;
    const std::array<double,3> &getZ() const;

    const double getHeading();
    const double getPitch();
    const double getRoll();

	// Rotates the matrix around any world-relative axis
	rotmat turnAroundY(double angle); // Heading
	rotmat turnAroundZ(double angle); // "Pitch"
	rotmat turnAroundX(double angle); // "Roll"


	// Rotates the matrix around the X axis (nose)?
	rotmat turnYaw(double yaw);
	rotmat turnPitch(double pitch);
	rotmat turnRoll(double roll);


    // Keeps current roll
    rotmat turnRollStabilized(double headingchange, double climbchange);

    void CalculateEulerAngles();

    // Alternative when value can't be determined
    double getHeading2() const;
    // Alternative when value can't be determined
    double getPitch2() const;
    // Alternative when value can't be determined
    double getRoll2() const;


    rotmat operator*(double mult);
    rotmat * operator*=(double mult);
    rotmat operator+(rotmat addition);
    rotmat operator-(rotmat subtraction);
	rotmat operator*(rotmat multiplication);

    bool operator==(const rotmat &rhs) const;

    bool operator!=(const rotmat &rhs) const;

    void normalize();
private:
    std::array<double,3> x;
    std::array<double,3> y;
    std::array<double,3> z;
	//vector x;
	//vector y;
	//vector z;
    double heading;
    double pitch;
    double roll;
    /* Alternatives */
    double heading2;
    double pitch2;
    double roll2;

	// Private default constructor
	rotmat() {};
};


#endif //IMU_ROTMAT_H
