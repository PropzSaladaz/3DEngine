// matrix_tests.cpp
#include <gtest/gtest.h>
#include <cmath>
#include <math/math.hpp> // your header from the prompt

using namespace mgl::math;

// ------------------------------ basics ------------------------------

TEST(MatrixTest, StaticShapeTraits) {
    static_assert(mat2f::rows == 2 && mat2f::cols == 2);
    static_assert(mat3f::rows == 3 && mat3f::cols == 3);
    static_assert(mat4f::rows == 4 && mat4f::cols == 4);
    static_assert(mat2x3f::rows == 2 && mat2x3f::cols == 3);
    static_assert(mat3x2f::rows == 3 && mat3x2f::cols == 2);
}

TEST(MatrixTest, DefaultConstructorProducesIdentity_Float) {
    const mat2f I2;
    const mat3f I3;
    const mat4f I4;

    for (int r = 0; r < 2; ++r)
        for (int c = 0; c < 2; ++c)
            EXPECT_FLOAT_EQ(I2(r,c), (r==c)?1.0f:0.0f);

    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            EXPECT_FLOAT_EQ(I3(r,c), (r==c)?1.0f:0.0f);

    for (int r = 0; r < 4; ++r)
        for (int c = 0; c < 4; ++c)
            EXPECT_FLOAT_EQ(I4(r,c), (r==c)?1.0f:0.0f);
}

TEST(MatrixTest, DefaultConstructorProducesIdentity_Int) {
    const mat2i I2;
    const mat3i I3;

    for (int r = 0; r < 2; ++r)
        for (int c = 0; c < 2; ++c)
            EXPECT_EQ(I2(r,c), (r==c)?1:0);

    for (int r = 0; r < 3; ++r)
        for (int c = 0; c < 3; ++c)
            EXPECT_EQ(I3(r,c), (r==c)?1:0);
}

TEST(MatrixTest, RowMajorConstruction3x3) {
    const mat3f A(1,2,3,
                  4,5,6,
                  7,8,9);

    EXPECT_FLOAT_EQ(A(0,0), 1.0f);
    EXPECT_FLOAT_EQ(A(0,2), 3.0f);
    EXPECT_FLOAT_EQ(A(2,1), 8.0f);
}

TEST(MatrixTest, RowMajorConstruction_Rectangular) {
    const mat2x3f A(1,2,3,
                    4,5,6);
    EXPECT_FLOAT_EQ(A(0,0), 1.0f);
    EXPECT_FLOAT_EQ(A(0,2), 3.0f);
    EXPECT_FLOAT_EQ(A(1,1), 5.0f);

    const mat3x2f B(7,8,
                    9,10,
                    11,12);
    EXPECT_FLOAT_EQ(B(0,0), 7.0f);
    EXPECT_FLOAT_EQ(B(2,1), 12.0f);
}

// ------------------------------ transpose ------------------------------

TEST(MatrixTest, Transpose_Square) {
    const mat3f A(0,1,2,
                  3,4,5,
                  6,7,8);
    const mat3f expected(0,3,6,
                         1,4,7,
                         2,5,8);

    const auto T = transpose(A);
    EXPECT_TRUE(approx_equal(T, expected));
    EXPECT_TRUE(transpose(T) == A); // double transpose
}

TEST(MatrixTest, Transpose_Rectangular) {
    const mat2x3f A(1,2,3,
                    4,5,6);
    const mat3x2f expected(1,4,
                           2,5,
                           3,6);

    const auto T = transpose(A);
    EXPECT_TRUE(approx_equal(T, expected));
    EXPECT_TRUE(transpose(T) == A);
}

// ------------------------------ multiplication ------------------------------

TEST(MatrixTest, Multiply_Square3x3) {
    const mat3f A(1,2,3,
                  0,1,4,
                  5,6,0);
    const mat3f B(-2,1,0,
                  3,0,2,
                  4,-1,5);

    const mat3f expected(16,-2,19,
                         19,-4,22,
                          8, 5,12);

    const auto M = A * B;
    EXPECT_TRUE(approx_equal(M, expected));
}

TEST(MatrixTest, Multiply_Rectangular_3x2_2x4_To_3x4) {
    const mat3x2f A(1,3,
                    5,2,
                    0,4);
    const mat2x4f B(3,6,9,4,
                    2,7,8,3);
    const mat3x4f expected( 9,27,33,13,
                           19,44,61,26,
                            8,28,32,12);

    const auto M = A * B;
    EXPECT_TRUE(approx_equal(M, expected));
}

TEST(MatrixTest, Multiply_AllCoreRectangularCombos) {
    const mat2x3f A23(1,2,3, 
                      4,5,6);

    const mat3x2f B32(7,8, 
                      9,10, 
                      11,12);

    const mat2x4f A24(1,2,3,4, 
                      5,6,7,8);

    const mat4x2f B42(1,2, 
                      3,4, 
                      5,6, 
                      7,8);

    const mat3x4f A34( 1, 2, 3, 4,
                       5, 6, 7, 8,
                       9,10,11,12 );

    const mat4x3f B43( 1, 2, 3,
                       4, 5, 6,
                       7, 8, 9,
                      10,11,12 );

    // (2x3)*(3x2)->(2x2)
    {
        const mat2f expected(58,64,
                             139,154);
        EXPECT_TRUE(A23 * B32 == expected);
    }

    // (2x3)*(3x4)->(2x4)
    {
        const mat2x4f expected(38,44,50,56,
                               83,98,113,128);
        EXPECT_TRUE(A23 * A34 == expected);
    }

    // (3x2)*(2x3)->(3x3)
    {
        const mat3f expected(39,54,69,
                             49,68,87,
                             59,82,105);
        EXPECT_TRUE(B32 * A23 == expected);
    }

    // (3x2)*(2x4)->(3x4)
    {
        const mat3x4f expected(47,62,77,92,
                               59,78,97,116,
                               71,94,117,140);
        EXPECT_TRUE(B32 * A24 == expected);
    }

    // (3x4)*(4x2)->(3x2)
    {
        const mat3x2f expected(50,60,
                               114,140,
                               178,220);
        EXPECT_TRUE(A34 * B42 == expected);
    }

    // (3x4)*(4x3)->(3x3)
    {
        const mat3f expected(70,80,90,
                             158,184,210,
                             246,288,330);
        EXPECT_TRUE(A34 * B43 == expected);
    }

    // (4x2)*(2x3)->(4x3)
    {
        const mat4x3f expected(9,12,15,
                               19,26,33,
                               29,40,51,
                               39,54,69);
        EXPECT_TRUE(B42 * A23 == expected);
    }

    // (4x2)*(2x4)->(4x4)
    {
        const mat4f expected(11,14,17,20,
                             23,30,37,44,
                             35,46,57,68,
                             47,62,77,92);
        EXPECT_TRUE(B42 * A24 == expected);
    }

    // (4x3)*(3x2)->(4x2)
    {
        const mat4x2f expected(58,64,
                               139,154,
                               220,244,
                               301,334);
        EXPECT_TRUE(B43 * B32 == expected);
    }

    // (4x3)*(3x4)->(4x4)
    {
        const mat4f expected(38,44,50,56,
                             83,98,113,128,
                             128,152,176,200,
                             173,206,239,272);
        EXPECT_TRUE(B43 * A34 == expected);
    }

    // (2x4)*(4x2)->(2x2)
    {
        const mat2f expected(50,60,
                             114,140);
        EXPECT_TRUE(A24 * B42 == expected);
    }

    // (2x4)*(4x3)->(2x3)
    {
        const mat2x3f expected(70,80,90,
                               158,184,210);
        EXPECT_TRUE(A24 * B43 == expected);
    }
}

// ------------------------------ algebraic properties ------------------------------

TEST(MatrixTest, AlgebraicProperties_2x2) {
    const mat2f A(1,3,
                  0,4);

    const mat2f B(3,6,
                  2,7);

    const mat2f C(-1,0,
                   5,3);

    // Non-commutativity
    EXPECT_FALSE((A*B) == (B*A));

    // Associativity (addition and multiplication)
    EXPECT_TRUE(( (A+B)+C ) == ( A+(B+C) ));
    EXPECT_TRUE( (A*B)*C == A*(B*C) );

    // Distributivity
    EXPECT_TRUE( A*(B+C) == (A*B) + (A*C) );
}

// ------------------------------ transpose product rule ------------------------------

TEST(MatrixTest, TransposeOfProduct) {
    const mat2x3f A(1,3,5,
                    2,4,6);

    const mat3x2f B( 2, 1,
                    -1, 0,
                     3, 2);

    // AB is 2x2
    const auto AB = A * B;
    // transpose(AB) == transpose(B) * transpose(A)
    EXPECT_TRUE(transpose(AB) == transpose(B) * transpose(A));

    // Double transpose is identity
    EXPECT_TRUE(transpose(transpose(A)) == A);
    EXPECT_TRUE(transpose(transpose(B)) == B);
}

// ------------------------------ approx equal behavior ------------------------------

TEST(MatrixTest, ApproxEqualBehavior) {
    const mat3f I(1,0,0,
                  0,1,0,
                  0,0,1);

    const mat3f J(1 + 1e-7f, 0, 0,
                  0, 1 - 1e-7f, 0,
                  0, 0, 1 + 5e-7f);

    // exact fails, approx passes
    EXPECT_FALSE(I == J);
    EXPECT_TRUE(approx_equal(I, J, 1e-6f, 1e-5f));
}

// ------------------------------ mixed scalar types ------------------------------

TEST(MatrixTest, MixedScalarMultiplication_PromotesToCommonType) {
    const mat2x3i    Ai(1,2,3, 4,5,6);
    const mat3x2d    Bd(1.5, 0.0,
                        -2.0, 3.0,
                        1.0, 2.0);
    // cast to larger type (double)
    const mat2d expected( 1*1.5 + 2*(-2.0) + 3*1.0,   1*0.0 + 2*3.0 + 3*2.0,
                          4*1.5 + 5*(-2.0) + 6*1.0,   4*0.0 + 5*3.0 + 6*2.0 );

    // If you switch to common_type, replace the type here with mat<2,2,double> and use EXPECT_TRUE.
    const auto M = Ai * Bd;
    EXPECT_TRUE(M == expected);
}
