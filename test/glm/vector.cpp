#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

TEST(VectorTest, VectorAddition)
{
    // if
    glm::vec3 vec(2.0f, 3.0f, 1.0f);
	glm::vec3 vec2(3.0f, 4.0f, 1.0f);
    // when
	glm::vec3 sum = vec + vec2;
    // then 
    glm::vec3 result(5.0f, 7.0f, 2.0f);
    EXPECT_EQ(sum, result);
}

TEST(VectorTest, DotProduct)
{
    // if
    glm::vec3 u(1.0f, 0.0f, 0.0f);
	glm::vec3 v(0.5f, 0.5f, 0.0f);
    // when
	GLfloat dotProd = glm::dot(u, v);
    GLfloat dotProd2 = glm::length(u) * glm::length(v) * glm::cos(glm::radians(45.0f));
    // then 
    ASSERT_NEAR(dotProd, dotProd2, 1e-6);
}

TEST(VectorTest, CrossProduct)
{
    // if
    glm::vec3 u(1.0f, 0.0f, 0.0f);
	glm::vec3 v(0.5f, 0.5f, 0.0f);
    glm::vec3 w(0.0f, 1.0f, 0.5f);
    GLfloat c = 5.0f;

    // NO COMMUTATIVITY PROPERTY
    // when
	glm::vec3 crossProd         = glm::cross(u, v);
    glm::vec3 invertedCrossProd = glm::cross(v, u);
    // then
    ASSERT_EQ(crossProd, -invertedCrossProd);

    // FORMULA EQUIVALENCE
    // when
	GLfloat crossProdNorm  = glm::length(glm::cross(u, v));
    GLfloat crossProdNorm2 = glm::length(u) * glm::length(v) * glm::sin(glm::radians(45.0f));
    // then 
    ASSERT_NEAR(crossProdNorm, crossProdNorm2, 1e-6);

    // DISTRIBUTIVE PROPERTY
    // when
    glm::vec3 distributive = glm::cross(u, (v + w));
    glm::vec3 distributive2 = glm::cross(u, v) + glm::cross(u, w);
    // then
    ASSERT_EQ(distributive, distributive2);

    // ASSOCIATIVE PROPERTY
    //when
    glm::vec3 assoc  = glm::cross(u, c * v);
    glm::vec3 assoc2 = c * glm::cross(u, v);
    glm::vec3 assoc3 = glm::cross(c * u, v);
    //then
    ASSERT_EQ(assoc, assoc2);
    ASSERT_EQ(assoc, assoc3);

}