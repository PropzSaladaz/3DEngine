#include <mgl/mglTransform.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/mglSimulation.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/vector_angle.inl>
#include <iostream>

namespace mgl {

// deal with length 0 vectors
glm::vec3 normalize(const glm::vec3& vec) {
    if (glm::length(vec) > glm::epsilon<float>()) {
        return glm::normalize(vec);
    }
    return {0.0f, 0.0f, 0.0f};
}

glm::vec2 normalize(const glm::vec2& vec) {
    if (glm::length(vec) > glm::epsilon<float>()) {
        return glm::normalize(vec);
    }
    return { 0.0f, 0.0f };
}

// deal with length 0 vectors & make it order-dependent
GLfloat angle(const glm::vec2& vec, const glm::vec2& vec2) {
    if (glm::length(vec) < glm::epsilon<float>() || 
        glm::length(vec2) < glm::epsilon<float>()) {
        return 0;
    }
    glm::vec3 cross = glm::cross(glm::vec3(vec, 0.0f), glm::vec3(vec2, 0.0f));
    return cross.z > 0 ? glm::angle(vec, vec2) : -glm::angle(vec, vec2);
}

GLfloat angle(const glm::vec3& vec, const glm::vec3& vec2) {
    if (glm::length(vec) < glm::epsilon<float>() ||
        glm::length(vec2) < glm::epsilon<float>()) {
        return 0;
    }
    return glm::angle(vec, vec2);
}

////////////////////////////////////////////////////////////////// Transform
Transform::Transform() : ITimeUpdateable() {
    setPosition(0, 0, 0);
}
Transform::Transform(const glm::vec3& _up, const  glm::vec3& _right, 
                                const  glm::vec3& _front) : Transform() {
    up = _up;
    right = _right;
    front = _front;
    targetPoint = positionV + front; // look at front
}

Transform::Transform(GLfloat x, GLfloat y, GLfloat z) 
    : Transform(glm::vec3(x, y, z)) {}

Transform::Transform(const glm::vec3 &center) : Transform() {
    targetPoint = positionV + front; // look at front
    setPosition(center);
}

Transform::Transform(const Transform* t) : Transform() {
    setTransform(t);
}

Transform::~Transform() {}

void Transform::resetTransform() {
    transformMatrix = glm::mat4(1.0f);
    positionV = ORIGIN;
    up    = YY;
    right = -XX;
    front = ZZ;
    glm::quat rotation = { 0.0f , 0.0f, 0.0f, 0.0f };
    glm::vec3 scaleV = { 1, 1, 1 };
    glm::vec3 translationV = { 0, 0, 0 };
}

/**
    Setting transform from another does not set the targetTransform, 
    nor the tracking target state
*/
void Transform::setTransform(const Transform* t) {
    positionV   = t->getPosition();
    up          = t->getUpV();
    right       = t->getRightV();
    front       = t->getFrontV();
    rotation    = t->getRotationQuat();
    scaleV      = t->getScale();
    targetPoint = t->getTargetPoint();
    computeTransformMatrix();
}

////////////////////////////////////////////////////////////////// Getters
const GLfloat* Transform::getTransformMatrixPtr() const {
    return glm::value_ptr(transformMatrix);
}

const glm::mat4 Transform::getTransformMatrix() const {
    return transformMatrix;
}

const glm::vec3 Transform::getPosition() const {
    return positionV;
}
const glm::vec3 Transform::getUpV() const {
    return up;
}
const glm::vec3 Transform::getRightV() const {
    return right;
}
const glm::vec3 Transform::getFrontV() const {
    return front;
}

const glm::vec3 Transform::getTargetPoint() const {
    return targetPoint;
}

const glm::vec3 Transform::getScale() const {
    return scaleV;
}

const glm::quat Transform::getRotationQuat() const {
    return rotation;
}

//////////////////////////////////////////////////////////////// Track Target

Transform* Transform::lookAt(const Transform* target) {
    glm::vec3 newFront = glm::normalize(target->getPosition() - positionV);
    glm::vec3 newRight;

    // If target coincides with YY axis, set right vector manually
    // Since we compute right vector against YY, if newFront is in YY, 
    // cross product will be 0 so we set right vector manually for such cases
    if (glm::all(glm::equal(newFront, -YY, FLOAT_THREASHOLD)) ||
        glm::all(glm::equal(newFront,  YY, FLOAT_THREASHOLD))) {
        newRight = front.z > 0 ? glm::vec3(-1.0f, 0.0f, 0.0f) : 
            glm::vec3(1.0f, 0.0f, 0.0f);
    }
    else newRight = normalize(glm::cross(newFront, YY));
    
    // OpenGL Z positive direction is up and X is left (if we look from above xz plane)
    // thus we have to invert x to make it to the right (as a normal xy plane would)
    glm::vec2 xzSrc = { -front.x, front.z };
    glm::vec2 xzSrcVec = (glm::length(xzSrc) > glm::epsilon<float>()) ?
        normalize(xzSrc) : normalize(glm::vec2(up.x, up.z));
    
    glm::vec2 xzTrgt = { -newFront.x, newFront.z };
    glm::vec2 xzTrgtVec = normalize(xzTrgt);
    GLfloat angleYY = angle(xzSrcVec, xzTrgtVec);
    rotateRad(angleYY, YY);

    // front is updated already from previous rotation
    // Take into account if we are now looking uppward - angle is positive
    // if we are looking downward - angle is negative
    GLfloat angleRight = (newFront.y > front.y) ?
        angle(newFront, front) : -angle(newFront, front);
    rotateRad(angleRight, newRight); // pitch

    return this;
}


Transform* Transform::lookAtFrom(const Transform* target, const Transform* source) {
    setPosition(source->getPosition());
    return lookAt(target);
}


//////////////////////////////////////////////////////////////////////////// Private/Protected

void Transform::computeTransformMatrix() {
    glm::mat4 scaleM     = glm::scale(I, scaleV);
    glm::mat4 translateM = glm::translate(I, positionV);
    glm::mat4 rotationM  = glm::toMat4(rotation);
    transformMatrix = translateM * rotationM * scaleM;
}

void Transform::normalizeTransform() {
    up    = glm::normalize(up);
    right = glm::normalize(right);
    front = glm::normalize(front);
}

//////////////////////////////////////////////////////////////////////////// Rotation
Transform* Transform::rotate2D(GLfloat angleDegree) {
    return rotate(angleDegree, ZZ);
}

Transform* Transform::rotate(GLfloat angleDegrees, glm::vec3 rotationAxis) {
    // needed to force CC rotation, else it represents the rotation
    // with a clockwise rot quaternion
    if (angleDegrees == 180.0f) angleDegrees -= FLOAT_THREASHOLD;
    return rotateRad(glm::radians(angleDegrees), rotationAxis);
}

Transform* Transform::setRotationQuat(const glm::quat& rot) {
    rotation = rot;
    glm::quat newUp = rot * glm::quat(0.0f, YY) * glm::conjugate(rot);
    glm::quat newRight = rot * glm::quat(0.0f, -XX) * glm::conjugate(rot);
    glm::quat newFront = rot * glm::quat(0.0f, ZZ) * glm::conjugate(rot);
    // TODO store initial XX, YY, ZZ - if camera is initialized with -ZZ, then on new setrotation, the ZZ will not be good
    computeTransformMatrix();
    return this;
}

// used internally for efficiency
Transform* Transform::rotateRad(GLfloat angleRads, glm::vec3 rotationAxis) {
    if (angleRads == 0) return this;
    
    glm::quat rot = glm::angleAxis(angleRads, rotationAxis);

    // update directional vectors
    glm::quat newUp = rot * glm::quat(0.0f, up) * glm::conjugate(rot);
    glm::quat newRight = rot * glm::quat(0.0f, right) * glm::conjugate(rot);
    glm::quat newFront = rot * glm::quat(0.0f, front) * glm::conjugate(rot);
    // convert target point to directional vector
    glm::quat newTargetPoint = rot * glm::quat(0.0f, targetPoint - positionV) * glm::conjugate(rot);
    up    = glm::vec3(newUp.x, newUp.y, newUp.z);
    right = glm::vec3(newRight.x, newRight.y, newRight.z);
    front = glm::vec3(newFront.x, newFront.y, newFront.z);
    // add the directional vector to position to get the target
    targetPoint = glm::vec3(newTargetPoint.x, newTargetPoint.y, newTargetPoint.z) + positionV;
    // update rotation quat & transformMatrix
    rotation = rot * rotation;
    computeTransformMatrix();
    return this;
}



//////////////////////////////////////////////////////////////////////////// Scaling
Transform* Transform::scale2D(GLfloat scale) {
    return scale2D(glm::vec2(scale));
}

Transform* Transform::scale2D(glm::vec2 scale_v) {
    return scale(glm::vec3(scale_v, 1.0f));
}
Transform* Transform::scale(GLfloat scale_factor) {
    return scale(glm::vec3(scale_factor));
}
Transform* Transform::setScale(const glm::vec3& scale) {
    scaleV = scale;
    computeTransformMatrix();
    return this;
}
Transform* Transform::scale(const glm::vec3 &scale_v) {
    scaleV *= scale_v;
    computeTransformMatrix();
    return this;
}

//////////////////////////////////////////////////////////////////////////// Translating

Transform* Transform::setPosition(GLfloat x, GLfloat y, GLfloat z) {
    return setPosition(glm::vec3(x, y, z));
}
Transform* Transform::setPosition(const glm::vec3& newPos) {
    return translate(newPos - positionV);
}

Transform* Transform::translate2D(GLfloat x, GLfloat y){
    return translate(glm::vec3(x, y, 0.0f));
}

Transform* Transform::translate(GLfloat x, GLfloat y, GLfloat z) {
    return translate(glm::vec3(x, y, z));
}

Transform* Transform::translate(const glm::vec3& translateVec) {
    positionV += translateVec;
    // set new translation in transform matrix directly
    // since translation is the last transform
    transformMatrix[3][0] = positionV.x;
    transformMatrix[3][1] = positionV.y;
    transformMatrix[3][2] = positionV.z;
    if (!trackingEnabled) targetPoint += translateVec;
    return this;
}

/////////////////////////////////////////////////////////////////// TimeUpdateable

void Transform::update(GLfloat deltaTime) {
    lookAt(targetTransform);
}

void Transform::trackTarget(const Transform* target) {
    if (target->getPosition() == positionV) {
        std::cout << "TRANSFORM::TRACK_TARGET::WARNING" << std::endl;
        std::cout << "Tracking transforms with the exact same position" <<
            "as the tracker results in undefined behavior" << std::endl;
        exit(EXIT_FAILURE);
    }
    targetTransform = target;
    trackingEnabled = true;
    Simulation::getInstance().trackUpdateable(getId(), this);
}
void Transform::disableTargetTracking() {
    targetTransform = nullptr;
    trackingEnabled = false;
    Simulation::getInstance().stopTracking(getId());
}

}