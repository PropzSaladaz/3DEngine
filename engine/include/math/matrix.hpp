// mat.hpp — row×col wrapper, scalar-type templated
#pragma once
#include <glm/glm.hpp>
#include <type_traits>
#include "config.hpp"
#include <string>
#include <typeinfo>

namespace mgl::math {

template<int R, int C, typename T = scalar>
struct mat {
    static_assert(R > 0 && C > 0, "matrix: R and C must be positive");
    using scalar_type = T;
    static constexpr int rows = R;
    static constexpr int cols = C;

    using backend_t = glm::mat<C, R, T, glm::defaultp>; // GLM is (Cols, Rows, T)
    backend_t m;

    // --------------------------------
    // constructors
    // --------------------------------

    constexpr mat() : m(1) {}
    explicit constexpr mat(const backend_t& g) : m(g) {}
    template<typename U>

    // converting type constructor
    explicit constexpr mat(const mat<R, C, U>& other) : m(typename mat<R, C, T>::backend_t(other.m)) {}


    // construct from R*C scalars in row-major order
    template<typename... Args,
             typename = std::enable_if_t<(sizeof...(Args) == R*C)
                   && (std::conjunction_v<std::is_convertible<Args, T>...>)>>
    constexpr mat(Args... vals) : m(0) {
        const T a[] = { static_cast<T>(vals)... }; // row-major input
        for (int c = 0; c < C; ++c)
            for (int r = 0; r < R; ++r)
                m[c][r] = a[r*C + c];              // store column-major
    }

    // --------------------------------
    // access (row-major)
    // --------------------------------

    constexpr       T& operator()(int r, int c)       noexcept { return m[c][r]; }
    constexpr const T& operator()(int r, int c) const noexcept { return m[c][r]; }

    // data pointer (column-major contiguous)
    constexpr       T* data()       noexcept { return &m[0][0]; }
    constexpr const T* data() const noexcept { return &m[0][0]; }

    /**
     * String representation for debugging purposes.
     */
    constexpr std::string to_string() const noexcept {
        std::string s = "mat" + std::to_string(R) + "x" + std::to_string(C) + "<" + typeid(T).name() + ">:\n";
        for (int r = 0; r < R; ++r) {
            s += "  [ ";
            for (int c = 0; c < C; ++c) {
                s += std::to_string((*this)(r,c));
                if (c < C-1) s += ", ";
            }
            s += " ]\n";
        }
        return s;
    }

    // exact equality (element-wise)
    friend constexpr bool operator==(const mat& a, const mat& b) noexcept {
        for (int r=0;r<R;++r) for (int c=0;c<C;++c)
            if (!(a(r,c) == b(r,c))) return false;
        return true;
    }
    friend constexpr bool operator!=(const mat& a, const mat& b) noexcept { return !(a==b); }

    /**
     * Stream output for debugging purposes.
     */
    friend std::ostream& operator<<(std::ostream& os, const mat<R,C,T>& M) {
        os << M.to_string();
        return os;
    }

};

// transpose: mat<R,C,T> -> mat<C,R,T>
template<int R, int C, typename T>
constexpr mat<C,R,T> transpose(const mat<R,C,T>& A) {
    return mat<C,R,T>{ glm::transpose(typename mat<R,C,T>::backend_t(A.m)) };
}

// ------------------------------------------------
// Operator overloads
// ------------------------------------------------

// (R×K) * (K×C) -> (R×C), promote to common scalar
template<int R, int K, int C, typename T1, typename T2>
constexpr mat<R,C,std::common_type_t<T1,T2>> operator*(const mat<R,K,T1>& A, const mat<K,C,T2>& B)
{
    using CT = std::common_type_t<T1,T2>;
    const mat<R,K,CT> Ac(A);   // upcast
    const mat<K,C,CT> Bc(B);   // upcast
    return mat<R,C,CT>{ Ac.m * Bc.m };
}

// sum: (R×K) + (R×K) -> (R×K)
template<int R, int C, typename T1, typename T2>
constexpr mat<R,C,std::common_type_t<T1,T2>> operator+(const mat<R,C,T1>& A, const mat<R,C,T2>& B) {
    return mat<R,C,std::common_type_t<T1,T2>>{ A.m + B.m };
}

// difference: (R×K) - (R×K) -> (R×K)
template<int R, int C, typename T1, typename T2>
constexpr mat<R,C,std::common_type_t<T1,T2>> operator-(const mat<R,C,T1>& A, const mat<R,C,T2>& B) {
    return mat<R,C,std::common_type_t<T1,T2>>{ A.m - B.m };
}

// ------------------------------------------------
// Equality helpers
// ------------------------------------------------

// approximate equality for matrices (element-wise, abs + relative tolerance)
template<int R, int C, typename T>
inline bool approxEqual(const mat<R,C,T>& a, const mat<R,C,T>& b,
                         T abs_eps = mgl::math::abs_eps, T rel_eps = mgl::math::rel_eps) noexcept
{
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            const T x = a(r,c);
            const T y = b(r,c);
            const T diff = std::abs(x - y);
            const T tol  = std::max(abs_eps, rel_eps * std::max(std::abs(x), std::abs(y)));
            if (diff > tol) return false;
        }
    }
    return true;
}

// ====================================================
// Define rectangular matrix type aliases
// ====================================================

template<typename T = scalar>
using mat2x3 = mat<2,3,T>;
template<typename T = scalar>
using mat3x2 = mat<3,2,T>;
template<typename T = scalar>
using mat3x4 = mat<3,4,T>;
template<typename T = scalar>
using mat4x3 = mat<4,3,T>;
template<typename T = scalar>
using mat2x4 = mat<2,4,T>;
template<typename T = scalar>
using mat4x2 = mat<4,2,T>;

// Square matrices
template<typename T = scalar>
using mat2 = mat<2,2,T>;
template<typename T = scalar>
using mat3 = mat<3,3,T>;
template<typename T = scalar>
using mat4 = mat<4,4,T>;


// ====================================================
// Define type aliases
// ====================================================

using mat2f = mat2<float>;
using mat3f = mat3<float>;
using mat4f = mat4<float>;
using mat2x3f = mat2x3<float>;
using mat3x2f = mat3x2<float>;
using mat3x4f = mat3x4<float>;
using mat4x3f = mat4x3<float>;
using mat2x4f = mat2x4<float>;
using mat4x2f = mat4x2<float>;


using mat2d = mat2<double>;
using mat3d = mat3<double>;
using mat4d = mat4<double>;
using mat2x3d = mat2x3<double>;
using mat3x2d = mat3x2<double>;
using mat3x4d = mat3x4<double>;
using mat4x3d = mat4x3<double>;
using mat2x4d = mat2x4<double>;
using mat4x2d = mat4x2<double>;


using mat2i = mat2<int>;
using mat3i = mat3<int>;
using mat4i = mat4<int>;
using mat2x3i = mat2x3<int>;
using mat3x2i = mat3x2<int>;
using mat3x4i = mat3x4<int>;
using mat4x3i = mat4x3<int>;
using mat2x4i = mat2x4<int>;
using mat4x2i = mat4x2<int>;


using mat2u = mat2<unsigned>;
using mat3u = mat3<unsigned>;
using mat4u = mat4<unsigned>;
using mat2x3u = mat2x3<unsigned>;
using mat3x2u = mat3x2<unsigned>;
using mat3x4u = mat3x4<unsigned>;
using mat4x3u = mat4x3<unsigned>;
using mat2x4u = mat2x4<unsigned>;
using mat4x2u = mat4x2<unsigned>;


using mat2ui = mat2<std::uint32_t>;
using mat3ui = mat3<std::uint32_t>;
using mat4ui = mat4<std::uint32_t>;
using mat2x3ui = mat2x3<std::uint32_t>;
using mat3x2ui = mat3x2<std::uint32_t>;
using mat3x4ui = mat3x4<std::uint32_t>;
using mat4x3ui = mat4x3<std::uint32_t>;
using mat2x4ui = mat2x4<std::uint32_t>;
using mat4x2ui = mat4x2<std::uint32_t>;
} // namespace mgl::math
