// mat_t.hpp — row×col wrapper, scalar-type templated
#pragma once
#include <glm/glm.hpp>
#include <type_traits>
#include "config.hpp"
#include <string>
#include <typeinfo>

namespace mgl::math {

template<int R, int C, typename T = scalar>
struct mat_t {
    static_assert(R > 0 && C > 0, "matrix: R and C must be positive");
    using scalar_type = T;
    static constexpr int rows = R;
    static constexpr int cols = C;

    using backend_t = glm::mat<C, R, T, glm::defaultp>; // GLM is (Cols, Rows, T)
    backend_t m;

    // --------------------------------
    // constructors
    // --------------------------------

    constexpr mat_t() : m(1) {}
    explicit constexpr mat_t(const backend_t& g) : m(g) {}
    
    // converting type constructor
    template<typename U>
    explicit constexpr mat_t(const mat_t<R, C, U>& other) : m(typename mat_t<R, C, T>::backend_t(other.m)) {}

    explicit constexpr mat_t(const backend_t&& g) : m(std::move(g)) {}


    // construct from R*C scalars in row-major order
    template<typename... Args,
             typename = std::enable_if_t<(sizeof...(Args) == R*C)
                   && (std::conjunction_v<std::is_convertible<Args, T>...>)>>
    explicit constexpr mat_t(Args... vals) : m(0) {
        const T a[] = { static_cast<T>(vals)... }; // row-major input
        for (int c = 0; c < C; ++c)
            for (int r = 0; r < R; ++r)
                m[c][r] = a[r*C + c];              // store column-major
    }

    // --------------------------------
    // access (row-major)
    // --------------------------------

    constexpr const backend_t& backend() const noexcept { return m; }

    constexpr       T& operator()(int r, int c)       noexcept { return m[c][r]; }
    constexpr const T& operator()(int r, int c) const noexcept { return m[c][r]; }

    // data pointer (column-major contiguous)
    constexpr       T* data()       noexcept { return &m[0][0]; }
    constexpr const T* data() const noexcept { return &m[0][0]; }

    /**
     * String representation for debugging purposes.
     */
    constexpr std::string to_string() const noexcept {
        std::string s = "mat_t" + std::to_string(R) + "x" + std::to_string(C) + "<" + typeid(T).name() + ">:\n";
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
    friend constexpr bool operator==(const mat_t& a, const mat_t& b) noexcept {
        for (int r=0;r<R;++r) for (int c=0;c<C;++c)
            if (!(a(r,c) == b(r,c))) return false;
        return true;
    }
    friend constexpr bool operator!=(const mat_t& a, const mat_t& b) noexcept { return !(a==b); }

    /**
     * Stream output for debugging purposes.
     */
    friend std::ostream& operator<<(std::ostream& os, const mat_t<R,C,T>& M) {
        os << M.to_string();
        return os;
    }

    static constexpr mat_t identity() noexcept {
        return mat_t{ glm::mat<C, R, T, glm::defaultp>(1) };
    }

};

// transpose: mat_t<R,C,T> -> mat_t<C,R,T>
template<int R, int C, typename T>
constexpr mat_t<C,R,T> transpose(const mat_t<R,C,T>& A) {
    return mat_t<C,R,T>{ glm::transpose(typename mat_t<R,C,T>::backend_t(A.m)) };
}

// ------------------------------------------------
// Operator overloads
// ------------------------------------------------

// (R×K) * (K×C) -> (R×C), promote to common scalar
template<int R, int K, int C, typename T1, typename T2>
constexpr mat_t<R,C,std::common_type_t<T1,T2>> operator*(const mat_t<R,K,T1>& A, const mat_t<K,C,T2>& B)
{
    using CT = std::common_type_t<T1,T2>;
    const mat_t<R,K,CT> Ac(A);   // upcast
    const mat_t<K,C,CT> Bc(B);   // upcast
    return mat_t<R,C,CT>{ Ac.m * Bc.m };
}

// sum: (R×K) + (R×K) -> (R×K)
template<int R, int C, typename T1, typename T2>
constexpr mat_t<R,C,std::common_type_t<T1,T2>> operator+(const mat_t<R,C,T1>& A, const mat_t<R,C,T2>& B) {
    return mat_t<R,C,std::common_type_t<T1,T2>>{ A.m + B.m };
}

// difference: (R×K) - (R×K) -> (R×K)
template<int R, int C, typename T1, typename T2>
constexpr mat_t<R,C,std::common_type_t<T1,T2>> operator-(const mat_t<R,C,T1>& A, const mat_t<R,C,T2>& B) {
    return mat_t<R,C,std::common_type_t<T1,T2>>{ A.m - B.m };
}

// ------------------------------------------------
// Equality helpers
// ------------------------------------------------

// approximate equality for matrices (element-wise, abs + relative tolerance)
template<int R, int C, typename T>
inline bool approxEqual(const mat_t<R,C,T>& a, const mat_t<R,C,T>& b,
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
// Define type aliases
// ====================================================

using mat2 = mat_t<2,2,float>;
using mat3 = mat_t<3,3,float>;
using mat4 = mat_t<4,4,float>;
using mat2x3 = mat_t<2,3,float>;
using mat3x2 = mat_t<3,2,float>;
using mat3x4 = mat_t<3,4,float>;
using mat4x3 = mat_t<4,3,float>;
using mat2x4 = mat_t<2,4,float>;
using mat4x2 = mat_t<4,2,float>;


using mat2d = mat_t<2,2,double>;
using mat3d = mat_t<3,3,double>;
using mat4d = mat_t<4,4,double>;
using mat2x3d = mat_t<2,3,double>;
using mat3x2d = mat_t<3,2,double>;
using mat3x4d = mat_t<3,4,double>;
using mat4x3d = mat_t<4,3,double>;
using mat2x4d = mat_t<2,4,double>;
using mat4x2d = mat_t<4,2,double>;


using mat2i = mat_t<2,2,int>;
using mat3i = mat_t<3,3,int>;
using mat4i = mat_t<4,4,int>;
using mat2x3i = mat_t<2,3,int>;
using mat3x2i = mat_t<3,2,int>;
using mat3x4i = mat_t<3,4,int>;
using mat4x3i = mat_t<4,3,int>;
using mat2x4i = mat_t<2,4,int>;
using mat4x2i = mat_t<4,2,int>;


using mat2u = mat_t<2,2,unsigned>;
using mat3u = mat_t<3,3,unsigned>;
using mat4u = mat_t<4,4,unsigned>;
using mat2x3u = mat_t<2,3,unsigned>;
using mat3x2u = mat_t<3,2,unsigned>;
using mat3x4u = mat_t<3,4,unsigned>;
using mat4x3u = mat_t<4,3,unsigned>;
using mat2x4u = mat_t<2,4,unsigned>;
using mat4x2u = mat_t<4,2,unsigned>;


using mat2ui = mat_t<2,2,std::uint32_t>;
using mat3ui = mat_t<3,3,std::uint32_t>;
using mat4ui = mat_t<4,4,std::uint32_t>;
using mat2x3ui = mat_t<2,3,std::uint32_t>;
using mat3x2ui = mat_t<3,2,std::uint32_t>;
using mat3x4ui = mat_t<3,4,std::uint32_t>;
using mat4x3ui = mat_t<4,3,std::uint32_t>;
using mat2x4ui = mat_t<2,4,std::uint32_t>;
using mat4x2ui = mat_t<4,2,std::uint32_t>;
} // namespace mgl::math
