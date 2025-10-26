// vec_tests.cpp
#include <gtest/gtest.h>
#include <cmath>
#include <array>
#include <math/vector.hpp>
#include <math/matrix.hpp>

using namespace mgl::math;

// ---------------------------- construction ----------------------------

TEST(VecConstruction, DefaultZero) {
    vec2f a;
    vec3f b;
    vec4f c;
    EXPECT_EQ(a, vec2f(0.0f, 0.0f));
    EXPECT_EQ(b, vec3f(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(c, vec4f(0.0f, 0.0f, 0.0f, 0.0f));
}

TEST(VecConstruction, SplatCtor) {
    vec3f v(2.5f);
    EXPECT_EQ(v, vec3f(2.5f, 2.5f, 2.5f));
}

TEST(VecConstruction, FromArray) {
    std::array<float,3> arr{1.0f, -2.0f, 3.5f};
    vec3f v(arr);
    EXPECT_EQ(v, vec3f(1.0f, -2.0f, 3.5f));
}

TEST(VecConstruction, MixedTypeCtor) {
    vec4d v(1, 2.5f, 3.0, -4);
    EXPECT_EQ(v, (vec4d(1.0, 2.5, 3.0, -4.0)));
}

// ------------------------ accessors & indexing ------------------------

TEST(VecAccess, ComponentAccessAndMutation) {
    vec4f v(1.0f, 2.0f, 3.0f, 4.0f);
    EXPECT_FLOAT_EQ(v.x(), 1.0f);
    EXPECT_FLOAT_EQ(v.y(), 2.0f);
    EXPECT_FLOAT_EQ(v.z(), 3.0f);
    EXPECT_FLOAT_EQ(v.w(), 4.0f);

    v[2] = 9.0f;     // z
    v.w() = -5.0f;   // w
    EXPECT_FLOAT_EQ(v.z(), 9.0f);
    EXPECT_FLOAT_EQ(v.w(), -5.0f);
}

TEST(VecAccess, DataPointerIsContiguous) {
    vec3f v(1.0f, 2.0f, 3.0f);
    float* p = v.data();
    ASSERT_NE(p, nullptr);
    EXPECT_FLOAT_EQ(p[0], 1.0f);
    EXPECT_FLOAT_EQ(p[1], 2.0f);
    EXPECT_FLOAT_EQ(p[2], 3.0f);
}

// ---------------------------- unary / eq ------------------------------

TEST(VecUnary, UnaryPlusMinusAndEquality) {
    const vec3f a(1.0f, -2.0f, 0.5f);
    const vec3f plus = +a;
    const vec3f minus = -a;
    EXPECT_EQ(plus, a);
    EXPECT_EQ(minus, vec3f(-1.0f, 2.0f, -0.5f));

    EXPECT_TRUE(a == plus);
    EXPECT_TRUE(a != minus);
}

// ------------------------ element-wise ops ---------------------------

TEST(VecArithmetic, ElementWiseOps) {
    const vec3f a(1.0f, 2.0f, 3.0f);
    const vec3f b(-4.0f, 5.0f, 6.0f);

    const vec3f sum = a + b;
    const vec3f diff = a - b;
    const vec3f hadamard = a * b;
    const vec3f divided = b / 2.0f;

    EXPECT_TRUE(approx_equal(sum, vec3f(-3.0f, 7.0f, 9.0f)));
    EXPECT_TRUE(approx_equal(diff, vec3f(5.0f, -3.0f, -3.0f)));
    EXPECT_TRUE(approx_equal(hadamard, vec3f(-4.0f, 10.0f, 18.0f)));
    EXPECT_TRUE(approx_equal(divided, vec3f(-2.0f, 2.5f, 3.0f)));
}

TEST(VecArithmetic, ScalarOpsBothSides) {
    const vec3f a(1.0f, -2.0f, 4.0f);

    const vec3f left = 2.0f * a;
    const vec3f right = a * 2.0f;
    const vec3f div = a / 2.0f;

    EXPECT_TRUE(approx_equal(left, vec3f(2.0f, -4.0f, 8.0f)));
    EXPECT_TRUE(approx_equal(right, vec3f(2.0f, -4.0f, 8.0f)));
    EXPECT_TRUE(approx_equal(div, vec3f(0.5f, -1.0f, 2.0f)));
}

TEST(VecArithmetic, CompoundAssignments) {
    vec3f v(1.0f, 2.0f, 3.0f);
    v += vec3f(4.0f, -2.0f, 1.0f);   // -> (5,0,4)
    v -= vec3f(1.0f, 1.0f, 2.0f);    // -> (4,-1,2)
    v *= 2.0f;                       // -> (8,-2,4)
    v /= 4.0f;                       // -> (2,-0.5,1)
    EXPECT_TRUE(approx_equal(v, vec3f(2.0f, -0.5f, 1.0f)));
}

// ----------------------- dot / length / normalize --------------------

TEST(VecAlgebra, DotAndLength) {
    const vec3f u(1.0f, -2.0f, 4.0f);
    const vec3f v(0.5f, 0.5f, 0.0f);

    const float expectedDot = 1.0f * 0.5f + (-2.0f) * 0.5f + 4.0f * 0.0f;
    EXPECT_FLOAT_EQ(dot(u, v), expectedDot);
    EXPECT_FLOAT_EQ(u.length2(), 1.0f + 4.0f + 16.0f);
    EXPECT_NEAR(u.length(), std::sqrt(21.0f), 1e-6f);
}

TEST(VecAlgebra, NormalizedUnitAndZeroSafe) {
    const vec3f a(3.0f, 0.0f, 4.0f);
    const vec3f unit = a.normalized();
    EXPECT_NEAR(unit.length(), 1.0f, 1e-6f);
    EXPECT_TRUE(approx_equal(unit, vec3f(0.6f, 0.0f, 0.8f)));

    const vec3f zero{};
    const vec3f safe = zero.normalized();
    EXPECT_EQ(safe, vec3f(0.0f, 0.0f, 0.0f));
}

// ----------------------------- cross ---------------------------------

TEST(VecCross, CrossProduct3DRightHandRule) {
    const vec3f ex(1.0f, 0.0f, 0.0f);
    const vec3f ey(0.0f, 1.0f, 0.0f);
    const vec3f ez = cross(ex, ey);
    EXPECT_TRUE(approx_equal(ez, vec3f(0.0f, 0.0f, 1.0f)));
    EXPECT_FLOAT_EQ(dot(ez, ex), 0.0f);
    EXPECT_FLOAT_EQ(dot(ez, ey), 0.0f);

    // anti-commutative: a×b = - (b×a)
    EXPECT_TRUE(approx_equal(cross(ey, ex), vec3f(0.0f, 0.0f, -1.0f)));
}

TEST(VecCross, CrossZForVec2Matches3DInterpretation) {
    const vec2f u(2.0f, 0.0f);
    const vec2f v(0.0f, 3.0f);
    // z-component should be 2*3 - 0*0 = 6
    EXPECT_FLOAT_EQ(cross_z(u, v), 6.0f);

    const vec2f a(1.0f, 2.0f);
    const vec2f b(4.0f, -3.0f);
    EXPECT_FLOAT_EQ(cross_z(a, b), 1.0f * (-3.0f) - 2.0f * 4.0f); // -11
}

// --------------------------- integer types ---------------------------

TEST(VecInteger, BasicOps) {
    vec3i a(1, 2, 3);
    vec3i b(4, -5, 6);

    EXPECT_EQ(a + b, vec3i(5, -3, 9));
    EXPECT_EQ(a - b, vec3i(-3, 7, -3));
    EXPECT_EQ(a * b, vec3i(4, -10, 18));
}

TEST(VecInteger, MixedCommonType) {
    vec3i ai(1, 2, 3);
    vec3d bd(0.5, 0.25, -1.0);

    // dot should return common_type<int,double> = double
    double d = dot(vec<3,double>(ai[0], ai[1], ai[2]), bd);
    EXPECT_NEAR(d, 1.0*0.5 + 2.0*0.25 + 3.0*(-1.0), 1e-12);
}

// ----------------------------- aliases -------------------------------

TEST(VecAliases, SizesAndTypes) {
    static_assert(vec2f::size == 2, "vec2f size mismatch");
    static_assert(vec3f::size == 3, "vec3f size mismatch");
    static_assert(vec4f::size == 4, "vec4f size mismatch");

    vec2u uu(1u, 2u);
    EXPECT_EQ(uu, (vec2u{1u, 2u}));

    vec4d dd(1.0, 2.0, 3.0, 4.0);
    EXPECT_EQ(dd, (vec4d{1.0, 2.0, 3.0, 4.0}));
}

// ----------------------- mat × vec smoke test ------------------------
// Keep this if your mat<> is available and constructed row-major.
TEST(VecMatMul, MatVecMultiplicationProducesExpected) {
    using mat3f = mat<3,3,float>;
    // Assuming your mat<3,3,T> supports a 9-scalar row-major constructor:
    const mat3f A( 1.0f, 2.0f, 3.0f,
                   4.0f, 5.0f, 6.0f,
                   7.0f, 8.0f, 9.0f );
    const vec3f x(1.0f, 0.0f, -1.0f);

    const vec3f y = A * x; // expected: (-2,-2,-2)
    EXPECT_TRUE(approx_equal(y, vec3f(-2.0f, -2.0f, -2.0f)));
}

// ------------------------- approx_equal edge -------------------------

TEST(VecApproxEqual, HandlesLargeAndTinyValues) {
    // Relative tolerance dominates for large magnitude
    vec3f a(1e6f, -1e6f, 1e6f);
    vec3f b = a * (1.0f + 8e-6f); // within rel_eps=1e-5
    EXPECT_TRUE(approx_equal(a, b, 1e-6f, 1e-5f));

    // Absolute tolerance dominates near zero
    vec3f c(1e-8f, -2e-8f, 0.0f);
    vec3f d(3e-7f, -1e-7f, 1e-7f);
    EXPECT_TRUE(approx_equal(c, d, 1e-6f, 1e-5f));
}
