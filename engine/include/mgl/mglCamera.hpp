#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <glm/glm.hpp>
#include <mgl/mglTransform.hpp>

namespace mgl {

enum CAMERA_TYPE { PERSPECTIVE, ORTHOGRAPHIC, N_TYPES};

struct PerspectiveParams {
	GLfloat fov;
	GLfloat screenRatio;
	GLfloat near;
	GLfloat far;
};

struct OrthoParams {
	GLfloat left;
	GLfloat right;
	GLfloat bottom;
	GLfloat top;
	GLfloat near;
	GLfloat far;
};

class Camera : public Transform {
public:
	Camera(GLuint bindingPoint);

	static void orthoResize(OrthoParams* params, 
		GLuint winx, GLuint winy);
	static void perspectiveResize(PerspectiveParams* params,
		GLuint winx, GLuint winy);

	void lookAtFrom(const Transform* target, const glm::vec3& position);
	void setPerspective(const PerspectiveParams& params);
	void setPerspective(GLfloat fov, GLfloat screen_ratio, 
								GLfloat near, GLfloat far);
	void setOrtho(const OrthoParams& params);
	void setOrtho(GLfloat left, GLfloat right, GLfloat bottom,
						GLfloat top, GLfloat near, GLfloat far);

	// overrides from Transform
	Transform* lookAtFrom(const Transform* target, const Transform* _position);
	Transform* lookAt(const Transform* target);
	Transform* setPosition(GLfloat x, GLfloat y, GLfloat z);
	Transform* setPosition(const glm::vec3& newPos);
	Transform* translate(GLfloat x, GLfloat y, GLfloat z);
	Transform* translate(const glm::vec3& translateDir);

	void zoom(GLfloat step);
	void pitch(GLfloat degrees);
	void yaw(GLfloat degrees);
	void setActive();

	void updateScreenRatio(GLuint width, GLuint height);

private:

	GLuint uboId;
	GLuint bindingPoint;

	glm::mat4 viewMatrix;
	glm::mat4 projectionMatrix;
	
	// projection types
	GLuint cameraType;
	bool IS_ACTIVE[N_TYPES];
	PerspectiveParams perspectiveParams;
	OrthoParams orthoParams;

	void setViewMatrix(const glm::mat4& matrix);
	void setViewMatrix();
	void setProjectionMatrix(const glm::mat4& matrix);
	void setProjectionMatrix();
};

}

#endif
