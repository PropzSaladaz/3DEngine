#pragma once

#include <string>
#include <mgl/models/textures/mglSampler.hpp>
#include <mgl/shaders/ShaderProgram.hpp>

namespace mgl {

class Texture;
class Texture2D;

//////////////////////////////////////////////////////////////////////// TEXTURE

class Texture {
protected:
  ui32 id;

public:
  Texture();
  ~Texture();
  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;
  Texture(Texture&& other) noexcept;
  Texture& operator=(Texture&& other) noexcept;
  virtual void bind() = 0;
  virtual void unbind() = 0;
protected:
    void genAndBindTextureOpenGL(ui32  texType, ui32 channels,
        ui32 width, ui32 height, void* image, ui32 type);
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
  void genCheckerboard(ui32 width, ui32 height, ui32 cells);
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
