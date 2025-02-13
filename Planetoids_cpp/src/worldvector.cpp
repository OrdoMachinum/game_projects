#include "worldvector.h"

WorldVector operator+(const WorldVector& a, const WorldVector& b)
{
    return WorldVector{
        .q1 = a.q1 + b.q1,
        .q2 = a.q2 + b.q2,
        .m_coordType = VectorCoordinateSystem::Cartesian2d
    };
}
WorldVector operator-(const WorldVector& lhs, const WorldVector& rhs)
{
    return WorldVector{
        .q1 = lhs.q1 - rhs.q1,
        .q2 = lhs.q2 - rhs.q2
    };
}

WorldVector operator*(const LENGTH_TYPE & scalar, const WorldVector & vector)
{
    return WorldVector{
        .q1 = vector.q1 * scalar,
        .q2 = vector.q2 * scalar,
        .m_coordType = VectorCoordinateSystem::Cartesian2d
    };
}

LENGTH_TYPE operator*(const WorldVector& vector_a, const WorldVector& vector_b)
{
    return  vector_a.q1 * vector_b.q1 + vector_a.q2 * vector_b.q2;
}


LENGTH_TYPE WorldVector::length() const
{
    switch(m_coordType){
    case VectorCoordinateSystem::Polar2d:
        return q1;
        break;
    case VectorCoordinateSystem::Cartesian2d:
    default:
        return (LENGTH_TYPE)sqrt(this->q1 * this->q1 + this->q2 * this->q2);
        break;
    }
}

LENGTH_TYPE WorldVector::theta() const
{
    switch(m_coordType){
    case VectorCoordinateSystem::Polar2d:
        return q2;
        break;
    case VectorCoordinateSystem::Cartesian2d:
    default:
        return (LENGTH_TYPE)atan2(this->q2, this->q1);
        break;
    }
}

WorldVector WorldVector::toPolar2d() const
{
    return WorldVector{
        .q1 = this->length(),
        .q2 = (LENGTH_TYPE)atan2(this->q2, this->q1),
        .m_coordType = VectorCoordinateSystem::Polar2d
    };
}

WorldVector WorldVector::toCartesian() const
{
    return WorldVector{
        .q1 = this->length() * cos(this->q2),
        .q2 = this->length() * sin(this->q2),
        .m_coordType = VectorCoordinateSystem::Cartesian2d
    };
}


WorldVector VecVecSum(const std::vector<WorldVector> & vv)
{
    WorldVector sum{0., 0.};

    for (size_t i = 0u; i < vv.size(); ++i) {
        sum = sum + vv[i];
    }
    return sum;
}
