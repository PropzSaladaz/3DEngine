#ifndef MGL_ORBIT_CAM_CONTROLLER_HPP
#define MGL_ORBIT_CAM_CONTROLLER_HPP

#include <mgl/camera/mglCameraController.hpp>

namespace mgl {

class OrbitCamController : public ICameraController {
public:

    OrbitCamController(Camera* camera);
    OrbitCamController(Camera* camera, const glm::vec3& centerP, GLfloat _radius);
    
    void setCenter(const glm::vec3& centerP);
    void setRadius(GLfloat radius);
    void setMouseSensitivity(GLfloat sensitivity);

    //void zoom(float step) {

    //}
protected:
    void handleContinuousInput(GLfloat deltatime) override;
    void registerDiscreteInputHandler() override;
private:
    glm::quat rotation = glm::angleAxis(0.0f, mgl::YY);
    GLfloat radius = 1.0f;
    Transform* center;
    GLfloat mouseSensitivity = 4.0f;

    void rotateCamera(float degrees, const glm::vec3& axis);
};

}

#endif