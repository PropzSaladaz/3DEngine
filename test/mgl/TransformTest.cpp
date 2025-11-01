#include <mgl/mgl.hpp>
#include <gtest/gtest.h>

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

    const mgl::mat4 transformMat = t->getTransformMatrix();

    mgl::vec4 pi { 0.0f, 0.0f, 0.0f, 1.0f };
    mgl::vec3 exp { 1.0f, 0.0f, 0.0f };

    mgl::vec3 pos = t->getPosition();
    ASSERT_TRUE(mgl::equal(pos, exp));

    mgl::vec3 computed = transformMat * pi;
    ASSERT_TRUE(mgl::equal(computed, exp));
}

TEST(TransformTest, UpVector)
{
    mgl::Transform t = mgl::Transform();
    mgl::vec3 up = t.getUpV();

    mgl::vec3 exp { 0.0f, 1.0f, 0.0f };
    ASSERT_TRUE(mgl::equal(up, exp));

    t.rotate(90.0f, mgl::XX);
    up = t.getUpV();
    exp = mgl::vec3(0.0f, 0.0f, 1.0f);
    ASSERT_TRUE(mgl::equal(up, exp));

    t.scale(5.0f);
    up = t.getUpV();
    ASSERT_TRUE(mgl::equal(up, exp));

    t.translate(mgl::vec3(1.0f, 5.0f, 3.0f));
    up = t.getUpV();
    ASSERT_TRUE(mgl::equal(up, exp));

    float unit = up.length();
    ASSERT_NEAR(unit, 1.0f, mgl::epsilon<float>());
}

TEST(TransformTest, RightVector)
{
    mgl::Transform t = mgl::Transform();
    mgl::vec3 right = t.getRightV();

    mgl::vec3 exp { -1.0f, 0.0f, 0.0f };
    ASSERT_TRUE(mgl::equal(right, exp));

    t.rotate(90.0f, mgl::YY);
    right = t.getRightV();
    exp = mgl::vec3(0.0f, 0.0f, 1.0f);
    ASSERT_TRUE(mgl::equal(right, exp));

    t.scale(5.0f);
    right = t.getRightV();
    ASSERT_TRUE(mgl::equal(right, exp));

    t.translate(mgl::vec3(1.0f, 5.0f, 3.0f));
    right = t.getRightV();
    ASSERT_TRUE(mgl::equal(right, exp));

    float unit = right.length();
    ASSERT_NEAR(unit, 1.0f, mgl::epsilon<float>());
}

TEST(TransformTest, FrontVector)
{
    mgl::Transform t = mgl::Transform();
    mgl::vec3 front = t.getFrontV();

    mgl::vec3 exp { 0.0f, 0.0f, 1.0f };
    ASSERT_TRUE(mgl::equal(front, exp));

    t.rotate(90.0f, mgl::XX);
    front = t.getFrontV();
    exp = mgl::vec3(0.0f, -1.0f, 0.0f);
    ASSERT_TRUE(mgl::equal(front, exp));

    t.scale(5.0f);
    front = t.getFrontV();
    ASSERT_TRUE(mgl::equal(front, exp));

    t.translate(mgl::vec3(1.0f, 5.0f, 3.0f));
    front = t.getFrontV();
    ASSERT_TRUE(mgl::equal(front, exp));

    float unit = front.length();
    ASSERT_NEAR(unit, 1.0f, mgl::epsilon<float>());
}

TEST(TransformTest, RotateTranslate)
{
    mgl::Transform* t = (new mgl::Transform())
        ->rotate(90.0f, mgl::YY)
        ->translate(1.0f, 1.0f, 1.0f);

    const mgl::mat4 transformMat = t->getTransformMatrix();
    mgl::vec4 pi { 1.0f, 0.0f, 0.0f, 1.0f };
    mgl::vec3 exp { 1.0f, 1.0f, 0.0f };

    mgl::vec3 computed = transformMat * pi;
    ASSERT_TRUE(mgl::equal(computed, exp));

    mgl::vec3 pos = t->getPosition();
    mgl::vec3 exp_pos { 1.0f, 1.0f, 1.0f };
    ASSERT_TRUE(mgl::equal(pos, exp_pos));
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
    mgl::vec3 frontExp  { 0.0f, -1.0f, 0.0f };
    mgl::vec3 upExp     { 0.0f,  0.0f, 1.0f };
    mgl::vec3 rightExp  { -1.0f,  0.0f, 0.0f };

    mgl::vec3 front = t.getFrontV();
    mgl::vec3 right = t.getRightV();
    ASSERT_TRUE(mgl::equal(front, frontExp));

    // WHEN - Looking from -YY to left to positive XX axis
    target->setPosition(1.0f, 0.0f, 0.0f);
    t.lookAt(target);

    // THEN
    frontExp = mgl::vec3{ 1.0f, 0.0f, 0.0f };
    upExp    = mgl::vec3{ 0.0f, 1.0f, 0.0f };
    rightExp = mgl::vec3{ 0.0f, 0.0f, 1.0f };

    front = t.getFrontV();
    right = t.getRightV();
    ASSERT_TRUE(mgl::equal(front, frontExp));
}
