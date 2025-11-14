#ifndef MGL_HPP
#define MGL_HPP

// shaders
#include <mgl/shaders/ShaderBuilder.hpp>
#include <mgl/shaders/ShaderProgram.hpp>

#include <mgl/mglApp.hpp>
#include <mgl/mglConventions.hpp>
#include <mgl/mglError.hpp>
#include <mgl/mglTransform.hpp>
#include <mgl/mglSimulation.hpp>
#include <mgl/mglAnimation.hpp>
#include <mgl/mglInputManager.hpp>
#include <mgl/mglManager.hpp>
#include <mgl/mglShaderManager.hpp>

// scene
#include <mgl/scene/mglSceneGraph.hpp>
#include <mgl/scene/mglSceneObject.hpp>
#include <mgl/scene/mglPointLight.hpp>
#include <mgl/scene/mglDirectionalLight.hpp>
#include <mgl/scene/mglSpotLight.hpp>

// models
#include <mgl/models/materials/mglMaterial.hpp>
#include <mgl/models/materials/mglMaterialManager.hpp>
#include <mgl/models/materials/mglBasicMaterial.hpp>
#include <mgl/models/materials/mglPhongMaterial.hpp>
#include <mgl/models/meshes/mglMesh.hpp>
#include <mgl/models/meshes/mglMeshManager.hpp>
#include <mgl/models/textures/mglSampler.hpp>
#include <mgl/models/textures/mglTexture.hpp>
#include <mgl/models/textures/mglTextureManager.hpp>

// camera
#include <mgl/camera/mglCamera.hpp>
#include <mgl/camera/mglCameraController.hpp>
#include <mgl/camera/mglFPSCamController.hpp>
#include <mgl/camera/mglOrbitCamController.hpp>
#include <mgl/camera/mglPerspectiveCamera.hpp>

// using namespace math
namespace mgl {
    using namespace math;
}

#endif /* MGL_HPP */
