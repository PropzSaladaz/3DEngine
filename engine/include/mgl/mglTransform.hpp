#ifndef MGL_TRANSFORM_HPP
#define MGL_TRANSFORM_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <mgl/mglSimulation.hpp>
#include "math/math.hpp"

namespace mgl {

const math::mat4 I = math::mat4::identity();

const math::vec3 XX = math::vec3(1.0f, 0.0f, 0.0f);
const math::vec3 YY = math::vec3(0.0f, 1.0f, 0.0f);
const math::vec3 ZZ = math::vec3(0.0f, 0.0f, 1.0f);

const math::vec3 ORIGIN = math::vec3(0.0f, 0.0f, 0.0f);

class Transform;

//////////////////////////////////////////////////////////////////////////// Transform
/*
    Represents transform over rigid body. Transformations are order-independent.
    Any order you specify, the transformation will be applied in this order:
    Translation * Rotation * Scaling -> Recall transformation is applied from right to left
*/
class Transform : public ITimeUpdateable {
public:
    Transform();
    Transform(const Transform* t);
    Transform(GLfloat x, GLfloat y, GLfloat z);
    Transform(const math::vec3 &center);
    Transform(const math::vec3 &up, const  math::vec3 &right, const  math::vec3 &front);
    ~Transform();
    Transform* rotate2D(GLfloat angleDegree);
    Transform* rotate(GLfloat angleDegrees, math::vec3 rotationAxis);
    Transform* setRotationQuat(const math::quat& rot);

    Transform* scale2D(GLfloat scale);
    Transform* scale2D(math::vec2 scale_v);
    Transform* scale(GLfloat scale);
    Transform* scale(const math::vec3 &scale_v);
    Transform* setScale(const math::vec3 &scale);

    Transform* translate2D(GLfloat x, GLfloat y);
    Transform* translate(GLfloat x, GLfloat y, GLfloat z);
    Transform* translate(const math::vec3& translateDir);

    Transform* setPosition(GLfloat x, GLfloat y, GLfloat z);
    Transform* setPosition(const math::vec3& newPos);

    Transform* lookAt(const Transform* target);
    Transform* lookAtFrom(const Transform* target, const Transform* source);

    void trackTarget(const Transform* target);
    void disableTargetTracking();

    void resetTransform();

    void setTransform(const Transform* t);

    const math::quat getRotationQuat() const;

    void update(GLfloat deltaTime) override;

    const GLfloat* getTransformMatrixPtr() const;
    const math::mat4 getTransformMatrix() const;
    const math::vec3 getPosition() const;
    const math::vec3 getScale() const;
    const math::vec3 getUpV() const;
    const math::vec3 getRightV() const;
    const math::vec3 getFrontV() const;
    const math::vec3 getTargetPoint() const;

    // TODO should have a lookAt method here
private:
    void computeTransformMatrix();
    Transform* rotateRad(GLfloat angleRads, math::vec3 rotationAxis);

protected:
    // basic orientation parameters - facing us
    math::vec3 up    = YY;
    math::vec3 right = -XX;
    math::vec3 front = ZZ;
    // basic transform parameters
    math::quat rotation = math::quat::fromY(0.0f);
    math::vec3 scaleV{ 1, 1, 1 };
    math::vec3 positionV{ 0, 0, 0 }; // represents the translation
    // transform matrix
    math::mat4 transformMatrix = I;
    // targeting
    bool trackingEnabled = false;
    math::vec3 targetPoint{ 0.0f, 0.0f, 0.0f };
    const Transform* targetTransform = nullptr;

    void updateTransform();
    void normalizeTransform();

};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_TRANSFORM_HPP */
