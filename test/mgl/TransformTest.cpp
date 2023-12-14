#include <mgl/mglTransform.hpp>
#include <glm/glm.hpp>
#include <gtest/gtest.h>
#include <glm/gtx/string_cast.hpp> 

#define THREASHOLD static_cast<float>(1.0e-5)

TEST(TransformTest, Id)
{
    mgl::Transform* t1 = new mgl::Transform();
    mgl::Transform* t2 = new mgl::Transform();

    ASSERT_NE(t1->getId(), t2->getId());
    ASSERT_EQ(t2->getId(), t1->getId() + 1);
}

TEST(TransformTest, SetPosition)
{
    mgl::Transform* t = (new mgl::Transform())
        ->translate(0.0f, -1.0f, 0.0f);

    t->setPosition(1.0f, 0.0f, 0.0f);

    const glm::mat4 transformMat = t->getTransformMatrix();

    glm::vec4 pi = { 0.0f, 0.0f, 0.0f, 1.0f };
    glm::vec3 exp = { 1.0f, 0.0f, 0.0f };

    glm::vec3 pos = t->getPosition();
    ASSERT_TRUE(glm::all(glm::equal(pos, exp, THREASHOLD)));

    glm::vec3 computed = transformMat * pi;
    ASSERT_TRUE(glm::all(glm::equal(computed, exp, THREASHOLD)));
}

TEST(TransformTest, UpVector)
{
    mgl::Transform t = mgl::Transform();
    glm::vec3 up = t.getUpV();

    glm::vec3 exp = { 0.0f, 1.0f, 0.0f };
    ASSERT_TRUE(glm::all(glm::equal(up, exp, THREASHOLD)));

    t.rotate(90.0f, mgl::XX);
    up = t.getUpV();
    exp = { 0.0f, 0.0f, 1.0f };
    ASSERT_TRUE(glm::all(glm::equal(up, exp, THREASHOLD)));

    t.scale(5.0f);
    up = t.getUpV();
    ASSERT_TRUE(glm::all(glm::equal(up, exp, THREASHOLD)));

    t.translate({1.0f, 5.0f, 3.0f});
    up = t.getUpV();
    ASSERT_TRUE(glm::all(glm::equal(up, exp, THREASHOLD)));

    float unit = glm::length(up);
    ASSERT_NEAR(unit, 1.0f, THREASHOLD);
}

TEST(TransformTest, RightVector)
{
    mgl::Transform t = mgl::Transform();
    glm::vec3 right = t.getRightV();

    glm::vec3 exp = { -1.0f, 0.0f, 0.0f };
    ASSERT_TRUE(glm::all(glm::equal(right, exp, THREASHOLD)));

    t.rotate(90.0f, mgl::YY);
    right = t.getRightV();
    exp = { 0.0f, 0.0f, 1.0f };
    ASSERT_TRUE(glm::all(glm::equal(right, exp, THREASHOLD)));

    t.scale(5.0f);
    right = t.getRightV();
    ASSERT_TRUE(glm::all(glm::equal(right, exp, THREASHOLD)));

    t.translate({ 1.0f, 5.0f, 3.0f });
    right = t.getRightV();
    ASSERT_TRUE(glm::all(glm::equal(right, exp, THREASHOLD)));

    float unit = glm::length(right);
    ASSERT_NEAR(unit, 1.0f, THREASHOLD);
}

TEST(TransformTest, FrontVector)
{
    mgl::Transform t = mgl::Transform();
    glm::vec3 front = t.getFrontV();

    glm::vec3 exp = { 0.0f, 0.0f, 1.0f };
    ASSERT_TRUE(glm::all(glm::equal(front, exp, THREASHOLD)));

    t.rotate(90.0f, mgl::XX);
    front = t.getFrontV();
    exp = { 0.0f, -1.0f, 0.0f };
    ASSERT_TRUE(glm::all(glm::equal(front, exp, THREASHOLD)));

    t.scale(5.0f);
    front = t.getFrontV();
    ASSERT_TRUE(glm::all(glm::equal(front, exp, THREASHOLD)));

    t.translate({ 1.0f, 5.0f, 3.0f });
    front = t.getFrontV();
    ASSERT_TRUE(glm::all(glm::equal(front, exp, THREASHOLD)));

    float unit = glm::length(front);
    ASSERT_NEAR(unit, 1.0f, THREASHOLD);
}

TEST(TransformTest, RotateTranslate)
{
    mgl::Transform* t = (new mgl::Transform())
        ->rotate(90.0f, mgl::YY)
        ->translate(1.0f, 1.0f, 1.0f);

    const glm::mat4 transformMat = t->getTransformMatrix();
    glm::vec4 pi = { 1.0f, 0.0f, 0.0f, 1.0f };
    glm::vec3 exp = { 1.0f, 1.0f, 0.0f };

    glm::vec3 computed = transformMat * pi;
    ASSERT_TRUE(glm::all(glm::equal(computed, exp, THREASHOLD)));

    glm::vec3 pos = t->getPosition();
    glm::vec3 exp_pos = { 1.0f, 1.0f, 1.0f };
    ASSERT_TRUE(glm::all(glm::equal(pos, exp_pos, THREASHOLD)));
}


TEST(TransformTest, LookAt)
{
    // GIVEN
    // looking at (0, 0, 1)
    mgl::Transform t = mgl::Transform();
    mgl::Transform *target = new mgl::Transform();
    target->translate(0.0f, -1.0f, 0.0f);

    // WHEN - Looking down the YY axis
    t.lookAt(target);

    // THEN
    glm::vec3 frontExp  = { 0.0f, -1.0f, 0.0f };
    glm::vec3 upExp     = { 0.0f,  0.0f, 1.0f };
    glm::vec3 rightExp  = { -1.0f,  0.0f, 0.0f };

    glm::vec3 front = t.getFrontV();
    glm::vec3 up    = t.getUpV();
    glm::vec3 right = t.getRightV();
    glm::mat4 transformMat = t.getTransformMatrix();
    glm::vec4 pi = { 0.0f, 0.0f, 1.0f, 1.0f };
    glm::vec3 frontMatComputed = transformMat * pi;

    ASSERT_TRUE(glm::all(glm::equal(front, frontExp, THREASHOLD)));
    ASSERT_TRUE(glm::all(glm::equal(up, upExp, THREASHOLD)));
    ASSERT_TRUE(glm::all(glm::equal(right, rightExp, THREASHOLD)));
    ASSERT_TRUE(glm::all(glm::equal(frontMatComputed, frontExp, THREASHOLD)));

    // WHEN - Looking from -YY to left to positive XX axis
    target->setPosition(1.0f, 0.0f, 0.0f);
    t.lookAt(target);

    // THEN
    frontExp = { 1.0f, 0.0f, 0.0f };
    upExp    = { 0.0f, 1.0f, 0.0f };
    rightExp = { 0.0f, 0.0f, 1.0f };

    front = t.getFrontV();
    up    = t.getUpV();
    right = t.getRightV();
    transformMat = t.getTransformMatrix();
    frontMatComputed = transformMat * pi;

    ASSERT_TRUE(glm::all(glm::equal(front, frontExp, THREASHOLD)));
    ASSERT_TRUE(glm::all(glm::equal(up, upExp, THREASHOLD)));
    ASSERT_TRUE(glm::all(glm::equal(right, rightExp, THREASHOLD)));
    ASSERT_TRUE(glm::all(glm::equal(frontMatComputed, frontExp, THREASHOLD)));
}