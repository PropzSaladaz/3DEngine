////////////////////////////////////////////////////////////////////////////////
//
// Textures 
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_TEXTURE_HPP
#define MGL_TEXTURE_HPP

#include <iostream>
#include <string>
#include <memory>
#include <mgl/models/textures/mglSampler.hpp>
#include <mgl/shaders/ShaderProgram.hpp>

namespace mgl {

class Texture;
class Texture2D;
struct TextureInfo;

//////////////////////////////////////////////////////////////////////// TEXTURE

class Texture {
protected:
  ui32 id;

public:
  Texture();
  ~Texture();
  virtual void bind() = 0;
  virtual void unbind() = 0;
protected:
    void genAndBindTextureOpenGL(ui32  texType, ui32 channels,
        ui32 width, ui32 height, void* image, ui32 type);
};

//////////////////////////////////////////////////////////////////// TextureInfo

struct TextureInfo {
  GLenum unit;                // GL_TEXTUREi
  ui32 index;               // sampler index in shader
  std::string uniform;        // uniform name in shader
  std::shared_ptr<Texture> texture = nullptr; // Texture (engine object)
  std::shared_ptr<Sampler> sampler = nullptr; // Sampler (engine object)

  TextureInfo(GLenum textureunit, ui32 index, const std::string &uniform,
              std::shared_ptr<Texture> texture, std::shared_ptr<Sampler> sampler);
  void updateShader(ShaderProgram& shader);
};

/////////////////////////////////////////////////////////////////////// TEXTURES

class Texture2D : public Texture {
public:
  void bind() override;
  void unbind() override;
  void load(const std::string &filename);
  void genPerlinNoise(ui32 size, ui32 octaves, 
      f64 atenuation, f64 frequency);
  void genSinePerlinNoise(ui32 size, ui32 octaves,
      f64 xPeriod, f64 yPeriod, f32 turbulence);
  void genSawPerlinNoise(ui32 size, ui32 octaves,
      f32 period, f32 turbulence);
};


class Texture3D : public Texture {
public:
    void bind() override;
    void unbind() override;
    void load(const std::string& filename);
    void genPerlinNoise(ui32 size, ui32 octaves,
        f64 atenuation, f64 frequency);
};

////////////////////////////////////////////////////////////////// CubeMap
class TextureCubeMap : public Texture {
public:
    void bind() override;
    void unbind() override;
    void loadCubeMap(const std::string& folder, const std::string& fileType);
};
} // namespace mgl

#endif /* MGL_TEXTURE_HPP */
