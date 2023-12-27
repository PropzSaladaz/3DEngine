#ifndef MGL_SKYBOX_HPP
#define MGL_SKYBOX_HPP

#include <mgl/scene/mglSceneObject.hpp>

namespace mgl {

class Skybox : public SceneObject {
    Skybox(const std::string& folder, const Mesh* mesh, const ShaderProgram* shaders);
};

}

#endif