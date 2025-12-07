////////////////////////////////////////////////////////////////////////////////
//
// Texture Sampler
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SAMPLER_HPP
#define MGL_SAMPLER_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace mgl {

enum class TextureWrap : GLint {
  Repeat = GL_REPEAT,
  ClampToEdge = GL_CLAMP_TO_EDGE,
  MirroredRepeat = GL_MIRRORED_REPEAT,
  ClampToBorder = GL_CLAMP_TO_BORDER
};

enum class TextureFilter : GLint {
  Nearest = GL_NEAREST,
  Linear = GL_LINEAR,
  NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
  NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
  LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
  LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR
};

////////////////////////////////////////////////////////////////////////////////

class Sampler {
protected:
  GLuint _samplerId;

public:
  Sampler(TextureWrap wrapS = TextureWrap::Repeat, TextureWrap wrapT = TextureWrap::Repeat,
          TextureWrap wrapR = TextureWrap::Repeat);
  ~Sampler();
  Sampler(const Sampler&) = delete;
  Sampler& operator=(const Sampler&) = delete;
  Sampler(Sampler&& other) noexcept;
  Sampler& operator=(Sampler&& other) noexcept;
  void create(TextureFilter minFilter = TextureFilter::Linear, TextureFilter magFilter = TextureFilter::Linear);
  void setWrap(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR = TextureWrap::Repeat);
  void setFilters(TextureFilter minFilter, TextureFilter magFilter);
  void setAnisotropy(float amount, bool clampToMax = true);
  void bind(GLuint unit);
  void unbind(GLuint unit);
};

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl

#endif /* MGL_SAMPLER_HPP */
