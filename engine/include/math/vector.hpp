// vec.hpp — fixed-size row-style vectors with GLM backend hidden
#pragma once
#include "config.hpp"
#include <glm/glm.hpp>
#include <type_traits>
#include <array>
#include <cmath>
#include <algorithm>

namespace mgl::math {

// Forward-declare matrix to enable mat*vec / vec*mat
template<int R, int C, typename T> struct mat;

// ==================================================
// Vector of size N with scalar type T (default: float)
// ==================================================

template<int N, typename T = scalar>
struct vec {
    static_assert(N > 0, "vector: N must be positive");
    using scalar_type = T;
    static constexpr int size = N;

    using backend_t = glm::vec<N, T, glm::defaultp>;
private:
    backend_t v;

public:
    // --- ctors ---
    constexpr vec() : v(0) {}                // zero initialize
    explicit constexpr vec(const backend_t& g) : v(g) {}

    // splat
    explicit constexpr vec(T s) : v(s) {}

    // N-scalar constructor (row order)
    template<typename... Args>
    requires (sizeof...(Args) == N) && (std::is_convertible_v<Args,T> && ...)
    constexpr vec(Args... vals) : v( static_cast<T>(vals)... ) {}

    // from array
    explicit constexpr vec(const std::array<T,N>& a) : v(0) {
        for (int i=0;i<N;++i) v[i] = a[i];
    }

    // access
    constexpr       T& operator[](int i)       noexcept { return v[i]; }
    constexpr const T& operator[](int i) const noexcept { return v[i]; }

    // named accessors (enabled only when valid)
    constexpr       T& x()       noexcept requires (N>=1) { return v[0]; }
    constexpr const T& x() const noexcept requires (N>=1) { return v[0]; }
    constexpr       T& y()       noexcept requires (N>=2) { return v[1]; }
    constexpr const T& y() const noexcept requires (N>=2) { return v[1]; }
    constexpr       T& z()       noexcept requires (N>=3) { return v[2]; }
    constexpr const T& z() const noexcept requires (N>=3) { return v[2]; }
    constexpr       T& w()       noexcept requires (N>=4) { return v[3]; }
    constexpr const T& w() const noexcept requires (N>=4) { return v[3]; }

    // data
    constexpr       T* data()       noexcept { return &v[0]; }
    constexpr const T* data() const noexcept { return &v[0]; }

    // unary
    constexpr vec operator+() const noexcept { return *this; }
    constexpr vec operator-() const noexcept {
        vec r;
        for (int i=0;i<N;++i) r[i] = -v[i];
        return r;
    }

    // element-wise +, -, *, /
    friend constexpr vec operator+(const vec& a, const vec& b) noexcept {
        vec r; for (int i=0;i<N;++i) r[i] = a[i] + b[i]; return r;
    }
    friend constexpr vec operator-(const vec& a, const vec& b) noexcept {
        vec r; for (int i=0;i<N;++i) r[i] = a[i] - b[i]; return r;
    }
    friend constexpr vec operator*(const vec& a, const vec& b) noexcept {
        vec r; for (int i=0;i<N;++i) r[i] = a[i] * b[i]; return r;
    }
    friend constexpr vec operator/(const vec& a, const vec& b) noexcept {
        vec r; for (int i=0;i<N;++i) r[i] = a[i] / b[i]; return r;
    }

    // scalar ops (vec ∘ scalar)
    friend constexpr vec operator*(const vec& a, T s) noexcept {
        vec r; for (int i=0;i<N;++i) r[i] = a[i] * s; return r;
    }
    friend constexpr vec operator*(T s, const vec& a) noexcept { return a * s; }
    friend constexpr vec operator/(const vec& a, T s) noexcept {
        vec r; for (int i=0;i<N;++i) r[i] = a[i] / s; return r;
    }

    // compound
    constexpr vec& operator+=(const vec& b) noexcept { for (int i=0;i<N;++i) v[i]+=b[i]; return *this; }
    constexpr vec& operator-=(const vec& b) noexcept { for (int i=0;i<N;++i) v[i]-=b[i]; return *this; }
    constexpr vec& operator*=(T s)          noexcept { for (int i=0;i<N;++i) v[i]*=s;   return *this; }
    constexpr vec& operator/=(T s)          noexcept { for (int i=0;i<N;++i) v[i]/=s;   return *this; }

    // exact equality
    friend constexpr bool operator==(const vec& a, const vec& b) noexcept {
        for (int i=0;i<N;++i) if (!(a[i] == b[i])) return false;
        return true;
    }
    friend constexpr bool operator!=(const vec& a, const vec& b) noexcept { return !(a==b); }

    // length squared
    constexpr T length2() const noexcept {
        T s = 0; 
        for (int i=0;i<N;++i) s += v[i]*v[i]; 
        return s;
    }

    T length() const noexcept { using std::sqrt; return sqrt(length2()); }

    vec normalized() const noexcept {
        T len = length();
        // Avoid div-by-zero: return zero if len == 0
        if (len == T(0)) return vec{};
        return *this / len;
    }

    // dot
    friend constexpr T dot(const vec& a, const vec& b) noexcept {
        T s = 0; for (int i=0;i<N;++i) s += a[i]*b[i]; return s;
    }

    // cross (only for N==3): standard 3D cross product
    friend constexpr vec<3,T> cross(const vec<3,T>& a, const vec<3,T>& b) noexcept
        requires (N==3)
    {
        return vec<3,T>(
            a[1]*b[2] - a[2]*b[1],
            a[2]*b[0] - a[0]*b[2],
            a[0]*b[1] - a[1]*b[0]
        );
    }

    // 2D "scalar cross": returns z-component of 3D cross with z=0
    friend constexpr T cross_z(const vec<2,T>& a, const vec<2,T>& b) noexcept
        requires (N==2)
    {
        return a[0]*b[1] - a[1]*b[0];
    }
};

// ------------------------- approx equality helper -------------------------

template<int N, typename T>
inline bool approx_equal(const vec<N,T>& a, const vec<N,T>& b,
                         T abs_eps = T(1e-6), T rel_eps = T(1e-5)) noexcept
{
    for (int i=0;i<N;++i) {
        const T x = a[i], y = b[i];
        const T diff = std::abs(x - y);
        const T tol  = std::max(abs_eps, rel_eps * std::max(std::abs(x), std::abs(y)));
        if (diff > tol) return false;
    }
    return true;
}

// -------------------------- mat × vec / vec × mat --------------------------
// (R×C) * (C) → (R)
template<int R, int C, typename Tm, typename Tv>
constexpr vec<R, std::common_type_t<Tm,Tv>>
operator*(const mat<R,C,Tm>& M, const vec<C,Tv>& x)
{
    using T = std::common_type_t<Tm,Tv>;
    vec<R,T> y;
    for (int r=0;r<R;++r) {
        T acc = 0;
        for (int c=0;c<C;++c) acc += static_cast<T>(M(r,c)) * static_cast<T>(x[c]);
        y[r] = acc;
    }
    return y;
}

// (R) * (R×C) → (C)   // row-vector times matrix
template<int R, int C, typename Tv, typename Tm>
constexpr vec<C, std::common_type_t<Tv,Tm>>
operator*(const vec<R,Tv>& x, const mat<R,C,Tm>& M)
{
    using T = std::common_type_t<Tv,Tm>;
    vec<C,T> y;
    for (int c=0;c<C;++c) {
        T acc = 0;
        for (int r=0;r<R;++r) acc += static_cast<T>(x[r]) * static_cast<T>(M(r,c));
        y[c] = acc;
    }
    return y;
}

// ------------------------------ handy aliases ------------------------------

template<typename T = scalar> using vec2 = vec<2,T>;
template<typename T = scalar> using vec3 = vec<3,T>;
template<typename T = scalar> using vec4 = vec<4,T>;

using vec2f = vec2<scalar>;
using vec3f = vec3<scalar>;
using vec4f = vec4<scalar>;

using vec2d = vec2<double>;
using vec3d = vec3<double>;
using vec4d = vec4<double>;

using vec2i = vec2<int>;
using vec3i = vec3<int>;
using vec4i = vec4<int>;

using vec2u = vec2<unsigned>;
using vec3u = vec3<unsigned>;
using vec4u = vec4<unsigned>;

using vec2ui = vec2<std::uint32_t>;
using vec3ui = vec3<std::uint32_t>;
using vec4ui = vec4<std::uint32_t>;

} // namespace mgl::math
