////////////////////////////////////////////////////////////////////////////////
//
// Shader Naming Conventions
// 
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_CONVENTIONS_HPP
#define MGL_CONVENTIONS_HPP

#define FLOAT_THREASHOLD static_cast<float>(1.0e-5)

#include <string>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

const enum ATTRIB_POS { POSITION_IDX, COLOR_IDX };

const char MODEL_MATRIX[] = "ModelMatrix";
const char NORMAL_MATRIX[] = "NormalMatrix";
const char VIEW_MATRIX[] = "ViewMatrix";
const char PROJECTION_MATRIX[] = "ProjectionMatrix";
const char TEXTURE_MATRIX[] = "TextureMatrix";
const char CAMERA_BLOCK[] = "Camera";

const char SKYBOX[] = "Skybox";

const char POSITION_ATTRIBUTE[] = "inPosition";
const char NORMAL_ATTRIBUTE[] = "inNormal";
const char TEXCOORD_ATTRIBUTE[] = "inTexcoord";
const char TANGENT_ATTRIBUTE[] = "inTangent";
const char BITANGENT_ATTRIBUTE[] = "inBitangent";
const char COLOR_ATTRIBUTE[] = "inColor";

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_CONVENTIONS_HPP */
