// vec_tests.cpp
#include <gtest/gtest.h>
#include <cmath>
#include <array>
#include <math/vector.hpp>
#include <math/matrix.hpp>

using namespace mgl::math;

// ---------------------------- construction ----------------------------

TEST(VecConstruction, DefaultZero) {
    vec2 a;
    vec3 b;
    vec4 c;
    EXPECT_EQ(a, vec2(0.0f, 0.0f));
    EXPECT_EQ(b, vec3(0.0f, 0.0f, 0.0f));
    EXPECT_EQ(c, vec4(0.0f, 0.0f, 0.0f, 0.0f));
}

TEST(VecConstruction, SplatCtor) {
    vec3 v(2.5f);
    EXPECT_EQ(v, vec3(2.5f, 2.5f, 2.5f));
}

TEST(VecConstruction, FromArray) {
    std::array<float,3> arr{1.0f, -2.0f, 3.5f};
    vec3 v(arr);
    EXPECT_EQ(v, vec3(1.0f, -2.0f, 3.5f));
}

TEST(VecConstruction, MixedTypeCtor) {
    vec4d v(1, 2.5f, 3.0, -4);
    EXPECT_EQ(v, (vec4d(1.0, 2.5, 3.0, -4.0)));
}

// ------------------------ accessors & indexing ------------------------

TEST(VecAccess, ComponentAccessAndMutation) {
    vec4 v(1.0f, 2.0f, 3.0f, 4.0f);
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
    vec3 v(1.0f, 2.0f, 3.0f);
    float* p = v.data();
    ASSERT_NE(p, nullptr);
    EXPECT_FLOAT_EQ(p[0], 1.0f);
    EXPECT_FLOAT_EQ(p[1], 2.0f);
    EXPECT_FLOAT_EQ(p[2], 3.0f);
}

// ---------------------------- unary / eq ------------------------------

TEST(VecUnary, UnaryPlusMinusAndEquality) {
    const vec3 a(1.0f, -2.0f, 0.5f);
    const vec3 plus = +a;
    const vec3 minus = -a;
    EXPECT_EQ(plus, a);
    EXPECT_EQ(minus, vec3(-1.0f, 2.0f, -0.5f));

    EXPECT_TRUE(a == plus);
    EXPECT_TRUE(a != minus);
}

// ------------------------ element-wise ops ---------------------------

TEST(VecArithmetic, ElementWiseOps) {
    const vec3 a(1.0f, 2.0f, 3.0f);
    const vec3 b(-4.0f, 5.0f, 6.0f);

    const vec3 sum = a + b;
    const vec3 diff = a - b;
    const vec3 hadamard = a * b;
    const vec3 divided = b / 2.0f;

    EXPECT_TRUE(approxEqual(sum, vec3(-3.0f, 7.0f, 9.0f)));
    EXPECT_TRUE(approxEqual(diff, vec3(5.0f, -3.0f, -3.0f)));
    EXPECT_TRUE(approxEqual(hadamard, vec3(-4.0f, 10.0f, 18.0f)));
    EXPECT_TRUE(approxEqual(divided, vec3(-2.0f, 2.5f, 3.0f)));
}

TEST(VecArithmetic, ScalarOpsBothSides) {
    const vec3 a(1.0f, -2.0f, 4.0f);

    const vec3 left = 2.0f * a;
    const vec3 right = a * 2.0f;
    const vec3 div = a / 2.0f;

    EXPECT_TRUE(approxEqual(left, vec3(2.0f, -4.0f, 8.0f)));
    EXPECT_TRUE(approxEqual(right, vec3(2.0f, -4.0f, 8.0f)));
    EXPECT_TRUE(approxEqual(div, vec3(0.5f, -1.0f, 2.0f)));
}

TEST(VecArithmetic, CompoundAssignments) {
    vec3 v(1.0f, 2.0f, 3.0f);
    v += vec3(4.0f, -2.0f, 1.0f);   // -> (5,0,4)
    v -= vec3(1.0f, 1.0f, 2.0f);    // -> (4,-1,2)
    v *= 2.0f;                       // -> (8,-2,4)
    v /= 4.0f;                       // -> (2,-0.5,1)
    EXPECT_TRUE(approxEqual(v, vec3(2.0f, -0.5f, 1.0f)));
}

// ----------------------- dot / length / normalize --------------------

TEST(VecAlgebra, DotAndLength) {
    const vec3 u(1.0f, -2.0f, 4.0f);
    const vec3 v(0.5f, 0.5f, 0.0f);

    const float expectedDot = 1.0f * 0.5f + (-2.0f) * 0.5f + 4.0f * 0.0f;
    EXPECT_FLOAT_EQ(dot(u, v), expectedDot);
    EXPECT_FLOAT_EQ(u.length2(), 1.0f + 4.0f + 16.0f);
    EXPECT_NEAR(u.length(), std::sqrt(21.0f), 1e-6f);
}

TEST(VecAlgebra, NormalizedUnitAndZeroSafe) {
    const vec3 a(3.0f, 0.0f, 4.0f);
    const vec3 unit = a.normalized();
    EXPECT_NEAR(unit.length(), 1.0f, 1e-6f);
    EXPECT_TRUE(approxEqual(unit, vec3(0.6f, 0.0f, 0.8f)));

    const vec3 zero{};
    const vec3 safe = zero.normalized();
    EXPECT_EQ(safe, vec3(0.0f, 0.0f, 0.0f));
}

// ----------------------------- cross ---------------------------------

TEST(VecCross, CrossProduct3DRightHandRule) {
    const vec3 ex(1.0f, 0.0f, 0.0f);
    const vec3 ey(0.0f, 1.0f, 0.0f);
    const vec3 ez = cross(ex, ey);
    EXPECT_TRUE(approxEqual(ez, vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_FLOAT_EQ(dot(ez, ex), 0.0f);
    EXPECT_FLOAT_EQ(dot(ez, ey), 0.0f);

    // anti-commutative: a×b = - (b×a)
    EXPECT_TRUE(approxEqual(cross(ey, ex), vec3(0.0f, 0.0f, -1.0f)));
}

TEST(VecCross, CrossZForVec2Matches3DInterpretation) {
    const vec2 u(2.0f, 0.0f);
    const vec2 v(0.0f, 3.0f);
    // z-component should be 2*3 - 0*0 = 6
    EXPECT_FLOAT_EQ(crossZ(u, v), 6.0f);

    const vec2 a(1.0f, 2.0f);
    const vec2 b(4.0f, -3.0f);
    EXPECT_FLOAT_EQ(crossZ(a, b), 1.0f * (-3.0f) - 2.0f * 4.0f); // -11
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
    double d = dot(vec3d(ai[0], ai[1], ai[2]), bd);
    EXPECT_NEAR(d, 1.0*0.5 + 2.0*0.25 + 3.0*(-1.0), 1e-12);
}

// ----------------------------- aliases -------------------------------

TEST(VecAliases, SizesAndTypes) {
    static_assert(vec2::size == 2, "vec2 size mismatch");
    static_assert(vec3::size == 3, "vec3 size mismatch");
    static_assert(vec4::size == 4, "vec4 size mismatch");

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
    const vec3 x(1.0f, 0.0f, -1.0f);

    const vec3 y = A * x; // expected: (-2,-2,-2)
    EXPECT_TRUE(approxEqual(y, vec3(-2.0f, -2.0f, -2.0f)));
}

// ------------------------- approxEqual edge -------------------------

TEST(VecApproxEqual, HandlesLargeAndTinyValues) {
    // Relative tolerance dominates for large magnitude
    vec3 a(1e6f, -1e6f, 1e6f);
    vec3 b = a * (1.0f + 8e-6f); // within rel_eps=1e-5
    EXPECT_TRUE(approxEqual(a, b, 1e-6f, 1e-5f));

    // Absolute tolerance dominates near zero
    vec3 c(1e-8f, -2e-8f, 0.0f);
    vec3 d(3e-7f, -1e-7f, 1e-7f);
    EXPECT_TRUE(approxEqual(c, d, 1e-6f, 1e-5f));
}
