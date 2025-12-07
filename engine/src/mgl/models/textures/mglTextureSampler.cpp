#include "mgl/models/textures/mglTextureSampler.hpp"
#include "mgl/models/textures/mglSampler.hpp"

namespace mgl {

TextureSampler::TextureSampler(GLenum _unit, GLint _index,
                         const std::string &_uniform, std::shared_ptr<Texture> _texture,
                         std::shared_ptr<Sampler> _sampler) {
  unit = _unit;
  uniform = _uniform;
  index = _index;
  texture = _texture;
  sampler = _sampler;
}

void TextureSampler::updateShader(ShaderProgram& shader) {
  glActiveTexture(unit);
  texture->bind();
  if (sampler) {
    sampler->bind(index);
  }
  else {
    static std::shared_ptr<Sampler> defaultSampler = []() {
      auto s = std::make_shared<Sampler>();
      s->create();
      return s;
    }();
    defaultSampler->bind(index);
  }
  shader.setUniform(uniform, index);
}

}