#ifndef MGL_CAMERA_HPP
#define MGL_CAMERA_HPP

#include <glm/glm.hpp>
#include <mgl/mglTransform.hpp>

namespace mgl {

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

	class Camera : Transform {
	public:
		Camera(GLuint bindingPoint);

		static void orthoResize(OrthoParams* params, GLuint winx, GLuint winy);
		static void perspectiveResize(PerspectiveParams* params, GLuint winx, GLuint winy);

		void lookAtFrom(const Transform* target, const glm::vec3& position);
		void lookAtFrom(const Transform* target, const Transform* _position);
		void lookAt(const Transform* target);
		void setPosition(GLfloat x, GLfloat y, GLfloat z);
		void setPosition(const glm::vec3& newPos);
		void translate(GLfloat x, GLfloat y, GLfloat z);
		void translate(const glm::vec3& translateDir);

		virtual void zoom(GLfloat step) = 0;
		void pitch(GLfloat degrees);
		void yaw(GLfloat degrees);
		void setActive();

		glm::vec3 getFrontV() const;
		glm::vec3 getRightV() const;
		glm::vec3 getUpV() const;
		glm::vec3 getPosition() const;
		glm::mat4 getViewMatrix() const;

		virtual void updateScreenRatio(GLuint width, GLuint height) = 0;

	protected:

		GLuint uboId;
		GLuint bindingPoint;

		glm::mat4 viewMatrix;
		glm::mat4 projectionMatrix;

		void updateViewMatrix();
		void computeViewMatrix();
		void setViewMatrix(const glm::mat4& mat);
		void setViewMatrix();
		virtual void updateProjectionMatrix();
		virtual void computeProjectionMatrix() = 0;
		void setProjectionMatrix();
	};

}

#endif