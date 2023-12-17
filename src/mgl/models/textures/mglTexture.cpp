#include <cassert>
#include <sstream>

#include <mgl/models/textures/mglTexture.hpp>
#include <utils/stb_image.h>
#include <utils/logger.hpp>
#include <utils/file.hpp>

namespace mgl {

//////////////////////////////////////////////////////////////////// TextureInfo

TextureInfo::TextureInfo(GLenum _unit, GLuint _index,
                         const std::string &_uniform, Texture *_texture,
                         Sampler *_sampler) {
  unit = _unit;
  uniform = _uniform;
  index = _index;
  texture = _texture;
  sampler = _sampler;
}

void TextureInfo::updateShader(ShaderProgram *shader) {
  glActiveTexture(unit);
  texture->bind();
  if (sampler)
    sampler->bind(index);
  shader->setUniformInt(uniform, index);
}

//////////////////////////////////////////////////////////////////////// Texture

Texture::Texture() : id(-1) {}

Texture::~Texture() {}

////////////////////////////////////////////////////////////////////// Texture2D

void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, id); }

void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture2D::load(const std::string &filename) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

#ifdef DEBUG
  util::Logger::LogDebug("Loading image: " + filename + "...");
#endif

    unsigned char *image = stbi_load(file::getFullPathFromRelative(filename).c_str(),
            &width, &height, &channels, 0);
    if (image == nullptr) {
        util::Logger::LogError("Could not load image");
        exit(EXIT_FAILURE);
    } 

#ifdef DEBUG
    else {
        util::Logger::LogDebug("Loaded successfully");
    }
#endif


    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
    //                GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    channels = 
          channels == 4 ? GL_RGBA
        : channels == 3 ? GL_RGB
        : channels == 2 ? GL_RG
        : GL_R;

    glTexImage2D(GL_TEXTURE_2D, 0, channels, width, height, 0, channels,
            GL_UNSIGNED_BYTE, image);

    // syntax: glTexImage2D(target, level, internalformat, width, height, border,
    // format, type, data)

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
