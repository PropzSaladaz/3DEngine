#include <mgl/mglCamera.hpp>
#include <mgl/mglInputManager.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
namespace mgl {

//////////////////////////////////////////////////////////////////////////// Camera
Camera::Camera(GLuint _bindingPoint) : Transform(YY, XX, -ZZ), 
	bindingPoint(_bindingPoint), cameraType(PERSPECTIVE) {
	glGenBuffers(1, &uboId);
	glBindBuffer(GL_UNIFORM_BUFFER, uboId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	viewMatrix = glm::mat4(1.0f);
	setViewMatrix(glm::lookAt(positionV, targetPoint, up));
	projectionMatrix = glm::mat4(1.0f);
	setProjectionMatrix(projectionMatrix);
	
	// register resize callback
	InputManager::getInstance().registerWindowSizeCallback([this](GLuint width, GLuint height) {
		updateScreenRatio(width, height);
		});
}

void Camera::orthoResize(OrthoParams* params,
	GLuint winx, GLuint winy) {
	GLfloat screenRation = static_cast<float>(winx) / winy;
	GLfloat verticalExtent = (params->top - params->bottom) / 2.0f;
	GLfloat horizontalExtent = screenRation * verticalExtent;
	params->left = -horizontalExtent;
	params->right = horizontalExtent;
}
void Camera::perspectiveResize(PerspectiveParams* params,
	GLuint winx, GLuint winy) {
	GLfloat screenRation = static_cast<float>(winx) / winy;
	params->screenRatio = screenRation;
}

void Camera::setActive() {
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, uboId);
	setViewMatrix();
	setProjectionMatrix();
}

void Camera::pitch(GLfloat degrees) {
	rotate(degrees, getRightV());
	setViewMatrix(glm::lookAt(positionV, targetPoint, up)); // TODO refactor & place all the setviewmatrixes in 1 single method
}

void Camera::yaw(GLfloat degrees) {
	rotate(degrees, mgl::YY);
	setViewMatrix(glm::lookAt(positionV, targetPoint, up));
}

/////////////////////////////////////////////////////////////////////////// Position

void Camera::lookAtFrom(const Transform* target, const glm::vec3& _position) {
	Transform::setPosition(_position);
	lookAt(target);
}

Transform* Camera::lookAtFrom(const Transform* target, const Transform* _position) {
	Transform::setPosition(_position->getPosition());
	lookAt(target);
	return this;
}

Transform* Camera::lookAt(const Transform* target) {
	Transform::lookAt(target); // update up, front, right vectors
	setViewMatrix(glm::lookAt(positionV, targetPoint, up));
	return this;
}


Transform* Camera::setPosition(GLfloat x, GLfloat y, GLfloat z) {
	setPosition(glm::vec3(x, y, z));
	return this;
}

Transform* Camera::setPosition(const glm::vec3& newPos) {
	Transform::setPosition(newPos); // update up, front, right vectors
	setViewMatrix(glm::lookAt(positionV, targetPoint, up));
	return this;
}

void Camera::zoom(GLfloat step) {
	translate(front * step);
	setViewMatrix(glm::lookAt(positionV, targetPoint, up));
}

Transform* Camera::translate(GLfloat x, GLfloat y, GLfloat z) {
	return translate(glm::vec3(x, y, z));
}
Transform* Camera::translate(const glm::vec3& translateDir) {
	Transform::translate(translateDir);
	setViewMatrix(glm::lookAt(positionV, targetPoint, up));
	return this;
}

/////////////////////////////////////////////////////////////////////////// Projections

void Camera::setPerspective(const PerspectiveParams& params) {
	setPerspective(params.fov, params.screenRatio, params.near, params.far);
}

void Camera::setPerspective(GLfloat fov, GLfloat screen_ratio, GLfloat near, GLfloat far) {
	IS_ACTIVE[cameraType] = false;
	cameraType = PERSPECTIVE;
	IS_ACTIVE[cameraType] = true;
	perspectiveParams = { fov, screen_ratio, near, far };
	setProjectionMatrix(glm::perspective(glm::radians(fov), screen_ratio, near, far));
}

void Camera::setOrtho(const OrthoParams& params) {
	setOrtho(params.left, params.right, params.bottom, params.top, params.near, params.far);
}

void Camera::setOrtho(GLfloat left, GLfloat right, GLfloat bottom, 
					  GLfloat top, GLfloat near, GLfloat far) {
	IS_ACTIVE[cameraType] = false;
	cameraType = ORTHOGRAPHIC;
	IS_ACTIVE[cameraType] = true;
	orthoParams = { left, right, bottom, top, near, far };
	setProjectionMatrix(glm::ortho(left, right, bottom, top, near, far));
}

void Camera::updateScreenRatio(GLuint width, GLuint height) {
	GLfloat screenRation = static_cast<float>(width) / height;
	switch (cameraType) {
		case PERSPECTIVE:
			Camera::perspectiveResize(&perspectiveParams, width, height);
			setPerspective(perspectiveParams);
			break;
		case ORTHOGRAPHIC:
			Camera::orthoResize(&orthoParams, width, height);
			setOrtho(orthoParams);
			break;
		default:
			break;
	}
}

/////////////////////////////////////////////////////////////////////////// OpenGL Shaders

void Camera::setViewMatrix(const glm::mat4& matrix) {
	viewMatrix = matrix;
	setViewMatrix();
}

void Camera::setViewMatrix() {
	glBindBuffer(GL_UNIFORM_BUFFER, uboId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4),
		glm::value_ptr(viewMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void Camera::setProjectionMatrix(const glm::mat4& matrix) {
	projectionMatrix = matrix;
	setProjectionMatrix();
}

void Camera::setProjectionMatrix() {
	glBindBuffer(GL_UNIFORM_BUFFER, uboId);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4),
		glm::value_ptr(projectionMatrix));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
}