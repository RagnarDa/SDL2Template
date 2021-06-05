//
// Created by Christoffer Wärnbring on 2019-01-13.
//

#include "rotmat.h"
#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI       3.14159265358979323846   // pi
#endif
//#include "gtest/gtest.h"
//#include "gmock/gmock.h"

rotmat::rotmat(vector X, vector Y, vector Z) {

}


/** this conversion uses NASA standard aeroplane conventions as described on page:
*   https://www.euclideanspace.com/maths/geometry/rotations/euler/index.htm
*   Coordinate System: right hand
*   Positive angle: right hand
*   Order of euler angles: heading first, then attitude, then bank
*   http://www.euclideanspace.com/maths/geometry/rotations/conversions/eulerToMatrix/index.htm */

/*  NOTE: I am switch Z axis to be out the right wing
 *  beacuse thats what I am used to. Pitch positive is nose up,
 *  heading is clockwise, and roll positive is right wing down. */

rotmat::rotmat(rotmat & copy):x(copy.x),y(copy.y),z(copy.z)
{
}

rotmat::rotmat(const double heading, const double pitch, const double roll) {
    double ch = std::cos(-heading);
    double sh = std::sin(-heading);
    double ca = std::cos(-pitch);
    double sa = std::sin(-pitch);
    double cb = std::cos(-roll);
    double sb = std::sin(-roll);

    x[0] = ch * ca;
    x[1] = sh*sb - ch*sa*cb;
    x[2] = -(ch*sa*sb + sh*cb);
    y[0] = sa;
    y[1] = ca*cb;
    y[2] = (-ca*sb);
    z[0] = sh*ca;
    z[1] = sh*sa*cb + ch*sb;
    z[2] = (sh*sa*sb + ch*cb);

    this->roll = roll;
    this->roll2 = roll;
    this->pitch = pitch;
    this->pitch2 = pitch;
    this->heading = heading;
    this->heading2 = heading;
    // Alternativelye: CalculateEulerAngles();

}

rotmat::rotmat(const std::array<double, 3> &x, const std::array<double, 3> &y, const std::array<double, 3> &z) : x(x),
                                                                                                                 y(y),
                                                                                                                 z(z) {
    this->CalculateEulerAngles();
}

rotmat::rotmat(double Xx, double Xy, double Xz, double Yx, double Yy, double Yz, double Zx, double Zy, double Zz)
{
	x = std::array<double, 3> {Xx, Xy, Xz};
	y = std::array<double, 3> {Yx, Yy, Yz};
	z = std::array<double, 3> {Zx, Zy, Zz};
	CalculateEulerAngles();
}

//rotmat(const vector X, const vector Y, const vector Z)
//{
//
//}

void rotmat::CalculateEulerAngles() {/*
     * http://www.gregslabaugh.net/publications/euler.pdf
     * ψ = roll
     * θ = heading
     * φ = pitch
     */

    if (z[0] != 1.0 && z[0] != -1.0)
    {
        heading = -asin(y[0]);
        heading2 = M_PI - heading;
        roll = -atan2(z[1] / cos(heading), z[2] / cos(heading));
        roll2 = atan2(z[1] / cos(heading2), z[2] / cos(heading2));
        pitch = -atan2(y[0] / cos(heading), x[0] / cos(heading));
        pitch2 = atan2(y[0] / cos(heading2), x[0] / cos(heading2));
    } else {
        pitch = 0;
        if (z[0] == -1.0)
        {
            heading = M_PI / 2.0;
            roll = pitch + atan2(x[1], x[2]);
        } else {
            heading = -M_PI / 2.0;
            roll = -pitch + atan2(-x[1], -x[2]);
        }
        pitch2 = pitch;
        heading2 = heading;
        roll2 = roll;
    }
}

const std::array<double,3> &rotmat::getX() const {
    return x;
}

const std::array<double,3> &rotmat::getY() const {
    return y;
}

const std::array<double,3> &rotmat::getZ() const {
    return z;
}

const double rotmat::getHeading() {
    return heading;
    //return std::atan2(this->getX()[2],this->getX()[0]);
}

const double rotmat::getPitch() {
    return pitch;
    //return -std::asin(this->getY()[0]);
}

const double rotmat::getRoll() {
    return roll;
    //return -std::atan2(this->getZ()[1],this->getZ()[2]);
}

// Relative to world (heading)
rotmat rotmat::turnAroundY(double angle)
{
	rotmat rtrn(*this);
	rtrn.x[0] = (this->x[0] * std::cos(angle)) - (this->x[2] * std::sin(angle));
	rtrn.x[2] = (this->x[0] * std::sin(angle)) + (this->x[2] * std::cos(angle));

	rtrn.y[0] = (this->y[0] * std::cos(angle)) - (this->y[2] * std::sin(angle));
	rtrn.y[2] = (this->y[0] * std::sin(angle)) + (this->y[2] * std::cos(angle));

	rtrn.z[0] = (this->z[0] * std::cos(angle)) - (this->z[2] * std::sin(angle));
	rtrn.z[2] = (this->z[0] * std::sin(angle)) + (this->z[2] * std::cos(angle));
	return rtrn;
}

// Relative to world ("pitch")
rotmat rotmat::turnAroundZ(double angle)
{
	rotmat rtrn(*this);
	rtrn.x[0] = (this->x[0] * std::cos(angle)) - (this->x[1] * std::sin(angle));
	rtrn.x[1] = (this->x[0] * std::sin(angle)) + (this->x[1] * std::cos(angle));

	rtrn.y[0] = (this->y[0] * std::cos(angle)) - (this->y[1] * std::sin(angle));
	rtrn.y[1] = (this->y[0] * std::sin(angle)) + (this->y[1] * std::cos(angle));

	rtrn.z[0] = (this->z[0] * std::cos(angle)) - (this->z[1] * std::sin(angle));
	rtrn.z[1] = (this->z[0] * std::sin(angle)) + (this->z[1] * std::cos(angle));
	return rtrn;
}

// Relative to world ("roll")
rotmat rotmat::turnAroundX(double angle)
{
	rotmat rtrn(*this);
	rtrn.x[1] = (this->x[1] * std::cos(angle)) - (this->x[2] * std::sin(angle));
	rtrn.x[2] = (this->x[1] * std::sin(angle)) + (this->x[2] * std::cos(angle));

	rtrn.y[1] = (this->y[1] * std::cos(angle)) - (this->y[2] * std::sin(angle));
	rtrn.y[2] = (this->y[1] * std::sin(angle)) + (this->y[2] * std::cos(angle));

	rtrn.z[1] = (this->z[1] * std::cos(angle)) - (this->z[2] * std::sin(angle));
	rtrn.z[2] = (this->z[1] * std::sin(angle)) + (this->z[2] * std::cos(angle));
	return rtrn;
}

rotmat rotmat::turnYaw(double yaw)
{
	rotmat yawmatrix = rotmat(yaw, 0.0, 0.0);
	return yawmatrix * *this;
}

rotmat rotmat::turnPitch(double pitch)
{
	rotmat pitchmatrix = rotmat(0.0, pitch, 0.0);
	return pitchmatrix * *this;
}

rotmat rotmat::turnRoll(double roll)
{
	rotmat rollmatrix = rotmat(0.0, 0.0, roll);
	return rollmatrix * *this;
}

rotmat rotmat::turnRollStabilized(double heading, double climb)
{
    // TODO! Doesnt work but is it really needed? 

    double oldroll = roll;
    rotmat Yrotated = this->turnAroundY(heading);
    Yrotated.CalculateEulerAngles();
    double newroll = Yrotated.getRoll();
    double pitchratio = std::cos(roll);
    double yawratio = -std::sin(roll);
    rotmat pitched = Yrotated.turnPitch(pitchratio*climb);
    rotmat yawed = pitched.turnYaw(yawratio*climb);
    return yawed;
}

double rotmat::getHeading2() const {
    return heading2;
}

double rotmat::getPitch2() const {
    return pitch2;
}

double rotmat::getRoll2() const {
    return roll2;
}

rotmat rotmat::operator*(double mult) {
    rotmat rtrn = *this;
    for (int axis = 0; axis < 3; ++axis) {
        rtrn.x[axis] *= mult;
        rtrn.y[axis] *= mult;
        rtrn.z[axis] *= mult;
    }
    return rtrn;
}

bool rotmat::operator==(const rotmat &rhs) const {
    // Only compare axes since eulers are unreliable
    return x == rhs.x &&
           y == rhs.y &&
           z == rhs.z/* &&
           heading == rhs.heading &&
           pitch == rhs.pitch &&
           roll == rhs.roll &&
           heading2 == rhs.heading2 &&
           pitch2 == rhs.pitch2 &&
           roll2 == rhs.roll2*/;
}

bool rotmat::operator!=(const rotmat &rhs) const {
    return !(rhs == *this);
}

rotmat * rotmat::operator*=(double mult) {
    *this = *this * mult;
    return this;
}

rotmat rotmat::operator+(rotmat addition) {
    rotmat rtrn = *this;
    for (int axis = 0; axis < 3; ++axis) {
        rtrn.x[axis] += addition.x[axis];
        rtrn.y[axis] += addition.y[axis];
        rtrn.z[axis] += addition.z[axis];
    }
    //rtrn.normalize();
    rtrn.CalculateEulerAngles();
    return rtrn;
}

rotmat rotmat::operator-(rotmat subtraction) {
    rotmat rtrn = *this;
    for (int axis = 0; axis < 3; ++axis) {
        rtrn.x[axis] -= subtraction.x[axis];
        rtrn.y[axis] -= subtraction.y[axis];
        rtrn.z[axis] -= subtraction.z[axis];
    }
    //rtrn.normalize();
    rtrn.CalculateEulerAngles();
    return rtrn;
}

rotmat rotmat::operator*(rotmat rhs)
{
	return rotmat(
		x[0] * rhs.x[0] + x[1] * rhs.y[0] + x[2] * rhs.z[0],
		x[0] * rhs.x[1] + x[1] * rhs.y[1] + x[2] * rhs.z[1],
		x[0] * rhs.x[2] + x[1] * rhs.y[2] + x[2] * rhs.z[2],
		y[0] * rhs.x[0] + y[1] * rhs.y[0] + y[2] * rhs.z[0],
		y[0] * rhs.x[1] + y[1] * rhs.y[1] + y[2] * rhs.z[1],
		y[0] * rhs.x[2] + y[1] * rhs.y[2] + y[2] * rhs.z[2],
		z[0] * rhs.x[0] + z[1] * rhs.y[0] + z[2] * rhs.z[0],
		z[0] * rhs.x[1] + z[1] * rhs.y[1] + z[2] * rhs.z[1],
		z[0] * rhs.x[2] + z[1] * rhs.y[2] + z[2] * rhs.z[2]
	);
};

void rotmat::normalize() {
    // I don't really need to do this for everyone
    double xlength = std::sqrt((this->x[0]*this->x[0])+(this->x[1]*this->x[1])+(this->x[2]*this->x[2]));
    double ylength = std::sqrt((this->y[0]*this->y[0])+(this->y[1]*this->y[1])+(this->y[2]*this->y[2]));
    double zlength = std::sqrt((this->z[0]*this->z[0])+(this->z[1]*this->z[1])+(this->z[2]*this->z[2]));
    //assert((xlength+ylength+zlength)/3.0<xlength*1.001);
    //assert((xlength+ylength+zlength)/3.0>zlength*0.999);
    double xratio = 1.0/xlength;
    double yratio = 1.0/ylength;
    double zratio = 1.0/zlength;

    for (int axis = 0; axis < 3; ++axis) {
        this->x[axis] *= xratio;
        this->y[axis] *= yratio;
        this->z[axis] *= zratio;
    }
}

