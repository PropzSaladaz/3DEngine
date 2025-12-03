#ifndef MGL_ORBIT_CAM_CONTROLLER_HPP
#define MGL_ORBIT_CAM_CONTROLLER_HPP

#include <mgl/camera/mglCameraController.hpp>
#include <memory>

namespace mgl {

class OrbitCamController : public ICameraController {
public:

    OrbitCamController(Camera* camera);
    OrbitCamController(Camera* camera, const math::vec3& centerP, f32 _radius);
    
    void setCenter(const math::vec3& centerP);
    void setRadius(f32 radius);
    void setMouseSensitivity(f32 sensitivity);

    //void zoom(float step) {

    //}
protected:
    void handleContinuousInput(f32 deltatime) override;
    void registerDiscreteInputHandler() override;
private:
    math::quat rotation = math::quat::fromAxisAngle(mgl::YY, 0.0f);
    f32 radius = 1.0f;
    std::unique_ptr<Transform> center;
    f32 mouseSensitivity = 4.0f;

    void rotateCamera(f32 degrees, const math::vec3& axis);
};

}

#endif
