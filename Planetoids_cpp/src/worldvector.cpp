#include "worldvector.h"

WorldVector operator+(const WorldVector& a, const WorldVector& b)
{
    return WorldVector{
        .q1 = a.q1 + b.q1,
        .q2 = a.q2 + b.q2
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
        .q2 = vector.q2 * scalar
    };
}

LENGTH_TYPE operator*(const WorldVector& vector_a, const WorldVector& vector_b)
{
    return  vector_a.q1 * vector_b.q1 + vector_a.q2 * vector_b.q2;
}

bool operator<(const WorldVector& a, const WorldVector& b)
{
    return a.length() < b.length();
}


LENGTH_TYPE WorldVector::length() const
{
    return (LENGTH_TYPE)sqrt(this->q1 * this->q1 + this->q2 * this->q2);
}

LENGTH_TYPE WorldVector::theta() const
{
        return (LENGTH_TYPE)atan2(this->q2, this->q1);
}

WorldVector WorldVector::toPolar2d() const
{
    return WorldVector{
        .q1 = this->length(),
        .q2 = (LENGTH_TYPE)atan2(this->q2, this->q1)
    };
}

WorldVector WorldVector::toCartesian() const
{
    return WorldVector{
        .q1 = this->length() * cos(this->q2),
        .q2 = this->length() * sin(this->q2),
    };
}

WorldVector WorldVector::rotate(LENGTH_TYPE phiDEG) const
{
    return WorldVector{.q1{this->q1*cosf(phiDEG) - this->q2*sinf(phiDEG)},
                       .q2{this->q1*sinf(phiDEG) + this->q2*cosf(phiDEG)}};
}

WorldVector VecVecSum(const std::vector<WorldVector> & vv)
{
    WorldVector sum{0., 0.};

    for (size_t i = 0u; i < vv.size(); ++i) {
        sum = sum + vv[i];
    }
    return sum;
}

WorldVector VecVecSum(const std::vector<WorldVector>& vv, int & indexOfBiggest)
{
    WorldVector sum{0., 0.};
    WorldVector biggest{0., 0.};

    for (size_t i = 0u; i < vv.size(); ++i) {

        const WorldVector& current{vv[i]};
        sum = sum + current;

        if(biggest < current) {
            indexOfBiggest = i;
            biggest = current;
        }
    }
    return sum;
}