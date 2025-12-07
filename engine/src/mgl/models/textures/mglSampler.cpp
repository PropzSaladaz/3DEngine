#include <iostream>
#include <algorithm>

#include <mgl/models/textures/mglSampler.hpp>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

Sampler::Sampler(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR) {
  glGenSamplers(1, &_samplerId);
  setWrap(wrapS, wrapT, wrapR);
}

Sampler::~Sampler() {
  if (_samplerId != 0) {
    glDeleteSamplers(1, &_samplerId);
  }
}

Sampler::Sampler(Sampler&& other) noexcept {
  _samplerId = other._samplerId;
  other._samplerId = 0;
}

Sampler& Sampler::operator=(Sampler&& other) noexcept {
  if (this != &other) {
    if (_samplerId != 0) {
      glDeleteSamplers(1, &_samplerId);
    }
    _samplerId = other._samplerId;
    other._samplerId = 0;
  }
  return *this;
}

void Sampler::create(TextureFilter minFilter, TextureFilter magFilter) {
  setFilters(minFilter, magFilter);
}

void Sampler::setWrap(TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR) {
  glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapS));
  glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapT));
  glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrapR));
}

void Sampler::setFilters(TextureFilter minFilter, TextureFilter magFilter) {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minFilter));
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magFilter));
}

void Sampler::setAnisotropy(float amount, bool clampToMax) {
  if (clampToMax) {
    GLfloat max = 0.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
    amount = std::min(amount, max);
  }
  glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY, amount);
}

void Sampler::bind(GLuint unit) { glBindSampler(unit, _samplerId); }

void Sampler::unbind(GLuint unit) { glBindSampler(unit, 0); }

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
