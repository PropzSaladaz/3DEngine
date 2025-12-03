#include <iostream>

#include <mgl/models/textures/mglSampler.hpp>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

Sampler::Sampler() {
  glGenSamplers(1, &_samplerId);
  glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glSamplerParameteri(_samplerId, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

void Sampler::bind(GLuint unit) { glBindSampler(unit, _samplerId); }

void Sampler::unbind(GLuint unit) { glBindSampler(unit, 0); }

void NearestSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void LinearSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void NearestMimapNearestSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_NEAREST);
}

void NearestMimapLinearSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
                      GL_NEAREST_MIPMAP_LINEAR);
}

void LinearMimapNearestSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_NEAREST);
}

void LinearMimapLinearSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
}

void LinearAnisotropicSampler::create() {
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY, Anisotropy);
}

void MaxAnisotropicSampler::create() {
  GLfloat max = 0.0f;
  glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &max);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glSamplerParameteri(_samplerId, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);
  glSamplerParameterf(_samplerId, GL_TEXTURE_MAX_ANISOTROPY, max);
  std::cout << "Sampler set to maximum anisotropy (" << max << ")."
            << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
