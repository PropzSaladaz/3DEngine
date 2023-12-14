#ifndef MATH_VECTOR_HPP
#define MATH_VECTOR_HPP

#include <cmath>
#include <iostream>

#define MIN_VEC_PRECISION 1e-5

#define PI 3.14159265358979323846
namespace math {

template <typename T = float>
struct vector {
    union { T x, r; };
    union { T y, g; };
    union { T z, b; };
    union { T w, a; };

    vector(T _x = 0, T _y = 0, T _z = 0, T _w = 0)
        : x(_x), y(_y), z(_z), w(_w) {}

    friend std::ostream& operator<<(std::ostream& os, const math::vector<T>& vec) {
        os << "vector: { " << vec.x << ", " << vec.y << ", "
            << vec.z << ", " << vec.w << " }";
        return os;
    }

    // --------------- Algebraic Operators --------------- //
    template<typename C>
    vector<T> operator+(const vector<C>& v) const {
        return vector(x + v.x, y + v.y, z + v.z, w + v.w);
    }
    template<typename C>
    vector<T> operator-(const vector<C>& v) const {
        return vector(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    vector<T> operator-() const {
        return vector(-x, -y, -z, -w);
    }
    template<typename C>
    vector<T> operator*(const C& scalar) const {
        return vector(x * scalar, y * scalar, z * scalar, w * scalar);
    }
    template<typename C>
    vector<T> operator+(const C& scalar) const {
        return vector(x + scalar, y + scalar, z + scalar, w + scalar);
    }
    template<typename C>
    vector<T> operator-(const C& scalar) const {
        return vector(x - scalar, y - scalar, z - scalar, w - scalar);
    }
    template<typename C>
    vector operator/(const C& scalar) const {
        return vector(x / scalar, y / scalar, z / scalar, w / scalar);
    }

    // --------------- Vector operators --------------- //
    template<typename T>
    T dot(const vector<T>& v) const {
        return x * v.x + y * v.y + z * v.z + w * v.w;
    }
    template<typename C>
    vector<T> cross(const vector<C>& v) const {
        return vector(y * v.z - z * v.y, 
                      z * v.x - x * v.z,
                      x * v.y - y * v.x);
    }
    T length() const {
        return sqrt(x*x + y*y + z*z + w*w);
    }

    template<typename T>
    bool longer(const vector<T>& v) const {
        return (x * x + y * y + z * z + w * w) > 
            (v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    }


    // --------------- Relational Operators --------------- //
    template<typename C>
    bool operator==(const vector<C>& v) const {
        return x == v.x && y == v.y && z == v.z && w == v.w;
    }
    template<typename C>
    bool operator>(const vector<C>& v) const {
        return longer(v);
    }
    template<typename C>
    bool operator>=(const vector<C>& v) const {
        return longer(v) || this == v;
    }
    template<typename C>
    bool operator<(const vector<C>& v) const {
        return !longer(v);
    }
    template<typename C>
    bool operator<=(const vector<C>& v) const {
        return !longer(v) || this == v;
    }
};
template <typename T = float>
vector(T, T, T, T) -> vector<T>;

// --------------- Global methods --------------- //

template<typename T>
bool perpendicular(const vector<T>& a, const vector<T>& b) {
    T dot = a.dot(b);
    return dot < MIN_VEC_PRECISION && dot > -MIN_VEC_PRECISION;
}

template<typename T>
bool parallel(const vector<T>& a, const vector<T>& b) {
    exit(EXIT_FAILURE); // TODO
    return a.dot(b) < MIN_VEC_PRECISION;
}


// --------------- TYPEDEF --------------- //
typedef vector<float>        vec;
typedef vector<int>          veci;
typedef vector<unsigned int> vecui;
typedef vector<double>       vecd;



template <typename T, typename C>
vector<T> operator*(const C& scalar, const vector<T>& vec) {
    return vector<T>(vec.x * scalar, vec.y * scalar, vec.z * scalar, vec.w * scalar);
}


}

#endif