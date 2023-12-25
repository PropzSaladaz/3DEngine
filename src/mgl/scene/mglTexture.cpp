#include <cassert>
#include <sstream>

#include "mglTexture.hpp"
#include "stb_image.h"

namespace mgl {

///////////////////////////////////////////////////////////////// TextureCubeMap

void TextureCubeMap::bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }

void TextureCubeMap::unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

const std::string CUBEMAP_SUFFIXES[6] = {"right",  "left",  "top",
                                         "bottom", "front", "back"};

const GLenum CUBEMAP_TEXTURES[6] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z};

void TextureCubeMap::loadCubeMap(const std::string &prefix,
                                 const std::string &suffix) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id);

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  for (int i = 0; i < 6; ++i) {
    std::stringstream ss;
    ss << prefix << CUBEMAP_SUFFIXES[i] << suffix;
    std::string filename = ss.str();

    int width, height, channels;
    std::cout << "Loading cubemap file " << filename << "... ";
    unsigned char *image =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);
    assert(channels == 4);
    if (image == nullptr) {
      std::cout << "error." << std::endl;
      exit(EXIT_FAILURE);
    } else {
      std::cout << "ok. " << std::endl;
    }
    glTexImage2D(CUBEMAP_TEXTURES[i], 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
  }
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
