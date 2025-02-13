#ifndef WORLDVECTOR_H
#define WORLDVECTOR_H

#include <cmath>
#include <vector>

#define LENGTH_TYPE    double


enum class VectorCoordinateSystem
{
    Undefined,
    Cartesian2d,
    Polar2d
};


// TODO: template this!
struct WorldVector
{

    LENGTH_TYPE q1{0.};     // x in Euclid, r in polar2d
    LENGTH_TYPE q2{0.};     // y in Euclid, theta in polar2d
    VectorCoordinateSystem m_coordType{VectorCoordinateSystem::Undefined};

    LENGTH_TYPE length() const;
    LENGTH_TYPE theta() const;

    WorldVector toPolar2d() const;
    WorldVector toCartesian() const;
    WorldVector rotate(LENGTH_TYPE phiDEG) const;

};

WorldVector operator+(const WorldVector& a, const WorldVector& b);
WorldVector operator-(const WorldVector& left, const WorldVector& right);
WorldVector operator*(const LENGTH_TYPE& scalar, const WorldVector& vector);
LENGTH_TYPE operator*(const WorldVector& vector_a, const WorldVector& vector_b);

bool operator<(const WorldVector& a, const WorldVector& b);


WorldVector VecVecSum(const std::vector<WorldVector>& vv);
WorldVector VecVecSum(const std::vector<WorldVector>& vv, int & indexOfBiggest);

#endif // WORLDVECTOR_H
