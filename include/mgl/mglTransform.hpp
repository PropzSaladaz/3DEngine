#ifndef MGL_TRANSFORM_HPP
#define MGL_TRANSFORM_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <mgl/mglSimulation.hpp>

namespace mgl {

const glm::mat4 I(1.0f);

const glm::vec3 XX = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 YY = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 ZZ = glm::vec3(0.0f, 0.0f, 1.0f);

const glm::vec3 ORIGIN = glm::vec3(0.0f, 0.0f, 0.0f);

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
    Transform(const glm::vec3 &center);
    Transform(const glm::vec3 &up, const  glm::vec3 &right, const  glm::vec3 &front);
    ~Transform();
    Transform* rotate2D(GLfloat angleDegree);
    Transform* rotate(GLfloat angleDegrees, glm::vec3 rotationAxis);
    Transform* setRotationQuat(const glm::quat& rot);

    Transform* scale2D(GLfloat scale);
    Transform* scale2D(glm::vec2 scale_v);
    Transform* scale(GLfloat scale);
    Transform* scale(const glm::vec3 &scale_v);
    Transform* setScale(const glm::vec3 &scale);

    Transform* translate2D(GLfloat x, GLfloat y);
    Transform* translate(GLfloat x, GLfloat y, GLfloat z);
    Transform* translate(const glm::vec3& translateDir);

    Transform* setPosition(GLfloat x, GLfloat y, GLfloat z);
    Transform* setPosition(const glm::vec3& newPos);

    Transform* lookAt(const Transform* target);
    Transform* lookAtFrom(const Transform* target, const Transform* source);

    void trackTarget(const Transform* target);
    void disableTargetTracking();

    void resetTransform();

    void setTransform(const Transform* t);

    const glm::quat getRotationQuat() const;

    void update(GLfloat deltaTime) override;

    const GLfloat* getTransformMatrixPtr() const;
    const glm::mat4 getTransformMatrix() const;
    const glm::vec3 getPosition() const;
    const glm::vec3 getScale() const;
    const glm::vec3 getUpV() const;
    const glm::vec3 getRightV() const;
    const glm::vec3 getFrontV() const;
    const glm::vec3 getTargetPoint() const;

    // TODO should have a lookAt method here
private:
    void computeTransformMatrix();
    Transform* rotateRad(GLfloat angleRads, glm::vec3 rotationAxis);

protected:
    // basic orientation parameters
    glm::vec3 up    = YY;
    glm::vec3 right = -XX;
    glm::vec3 front = ZZ;
    // basic transform parameters
    glm::quat rotation = glm::angleAxis(glm::radians(0.0f), YY);
    glm::vec3 scaleV    = { 1, 1, 1 };
    glm::vec3 positionV = { 0, 0, 0 }; // represents the translation
    // transform matrix
    glm::mat4 transformMatrix = glm::mat4(1.0f);
    // trageting
    bool trackingEnabled = false;
    glm::vec3 targetPoint = { 0.0f, 0.0f, 0.0f };
    const Transform* targetTransform = nullptr;

    void updateTransform();
    void normalizeTransform();

};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_TRANSFORM_HPP */
