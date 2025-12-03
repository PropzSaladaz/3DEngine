#include <cassert>
#include <sstream>
#include <cmath>

#include <mgl/models/textures/mglTexture.hpp>
#include <utils/stb_image.h>
#include <utils/Logger.hpp>
#include <utils/file.hpp>
#include <utils/noise.hpp>

namespace mgl {

//////////////////////////////////////////////////////////////////// TextureInfo

TextureInfo::TextureInfo(GLenum _unit, GLuint _index,
                         const std::string &_uniform, std::shared_ptr<Texture> _texture,
                         std::shared_ptr<Sampler> _sampler) {
  unit = _unit;
  uniform = _uniform;
  index = _index;
  texture = _texture;
  sampler = _sampler;
}

void TextureInfo::updateShader(ShaderProgram& shader) {
  glActiveTexture(unit);
  texture->bind();
  if (sampler)
    sampler->bind(index);
  shader.setUniform(uniform, index);
}

//////////////////////////////////////////////////////////////////////// Texture

Texture::Texture() : id(0) {}

Texture::~Texture() {
    if (id != 0) {
        glDeleteTextures(1, &id);
    }
}

Texture::Texture(Texture&& other) noexcept {
    id = other.id;
    other.id = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (id != 0) {
            glDeleteTextures(1, &id);
        }
        id = other.id;
        other.id = 0;
    }
    return *this;
}

void Texture::genAndBindTextureOpenGL(GLuint  texType, GLuint channels, 
    GLuint width, GLuint height, void* image, GLuint type) {
    glGenTextures(1, &id);
    glBindTexture(texType, id);

    // glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    // glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    // glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    // glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    // glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(texType, GL_TEXTURE_BASE_LEVEL, 0);

    // glTexParameteri(texType, GL_TEXTURE_MIN_FILTER,
    //                GL_LINEAR_MIPMAP_LINEAR);
    // glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(texType, 0, channels, width, height, 0, channels,
        type, image);

    // syntax: glTexImage2D(target, level, internalformat, width, height, border,
    // format, type, data)

    glGenerateMipmap(texType);
    glBindTexture(texType, 0);
}

////////////////////////////////////////////////////////////////////// Texture2D

void Texture2D::bind() { glBindTexture(GL_TEXTURE_2D, id); }

void Texture2D::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture2D::load(const std::string &filename) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

    #ifdef DEBUG
    MGL_DEBUG("Loading image: " + filename + "...");
    #endif

    unsigned char *image = stbi_load(file::resource_path(filename).c_str(),
            &width, &height, &channels, 0);
    if (image == nullptr) {
        MGL_ERROR("Could not load image");
        exit(EXIT_FAILURE);
    } 

    #ifdef DEBUG
    else {
        MGL_DEBUG("Loaded successfully");
    }
    #endif

    channels = 
          channels == 4 ? GL_RGBA
        : channels == 3 ? GL_RGB
        : channels == 2 ? GL_RG
        : GL_R;

    genAndBindTextureOpenGL(GL_TEXTURE_2D, channels, width, height, image, GL_UNSIGNED_BYTE);


    stbi_image_free(image);
}

void Texture2D::genPerlinNoise(GLuint size, GLuint octaves,
    GLdouble atenuation, GLdouble frequency) {

    float* image = new float[size * size * 3];
    double min = 0;
    double max = 0;
    for (size_t x = 0; x < size; x++) {
        for (size_t y = 0; y < size; y++) {
            double noise = util::PerlinNoise::getWithPersistance(x, y, 0, octaves, atenuation, frequency);
            image[3 * x * size + 3 * y]     = noise;
            image[3 * x * size + 3 * y + 1] = noise;
            image[3 * x * size + 3 * y + 2] = noise;
        }
    }
    genAndBindTextureOpenGL(GL_TEXTURE_2D, GL_RGB, size, size, image, GL_FLOAT);
    delete[] image;
}

void Texture2D::genSinePerlinNoise(GLuint size, GLuint octaves,
    GLdouble xPeriod, GLdouble yPeriod, GLfloat turbulence) {

    float* image = new float[size * size * 3];
    double min = 0;
    double max = 0;
    double _xPeriod = xPeriod * size;
    double _yPeriod = yPeriod * size;

    for (size_t x = 0; x < size; x++) {
        for (size_t y = 0; y < size; y++) {
            double perlinNoise = util::PerlinNoise::getWithPersistance(x, y, 0, octaves);
            double xyValue = x * _xPeriod + y * _yPeriod + turbulence * perlinNoise;
            double noise = sin(xyValue * 3.14159)*0.5 + 0.5;
            image[3 * x * size + 3 * y] = noise;
            image[3 * x * size + 3 * y + 1] = noise;
            image[3 * x * size + 3 * y + 2] = noise;
        }
    }
    genAndBindTextureOpenGL(GL_TEXTURE_2D, GL_RGB, size, size, image, GL_FLOAT);
    delete[] image;
}

void Texture2D::genSawPerlinNoise(GLuint size, GLuint octaves,
    GLfloat period, GLfloat turbulence) {
        float* image = new float[size * size * 3];
        double min = 0;
        double max = 0;

        for (size_t x = 0; x < size; x++) {
            for (size_t y = 0; y < size; y++) {
                double perlinNoise = util::PerlinNoise::getWithPersistance(x, y, 0, octaves);
                double sawInput = (x / period) + turbulence * perlinNoise;
                double output = (modf(sawInput, &sawInput));
                image[3 * x * size + 3 * y] = output;
                image[3 * x * size + 3 * y + 1] = output;
                image[3 * x * size + 3 * y + 2] = output;
            }
        }
        genAndBindTextureOpenGL(GL_TEXTURE_2D, GL_RGB, size, size, image, GL_FLOAT);
        delete[] image;
}

////////////////////////////////////////////////////////////////////// Texture3D


void Texture3D::bind() { glBindTexture(GL_TEXTURE_3D, id); }

void Texture3D::unbind() { glBindTexture(GL_TEXTURE_3D, 0); }

void Texture3D::load(const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);
    int width, height, channels;

    #ifdef DEBUG
    MGL_DEBUG("Loading image: " + filename + "...");
    #endif

    unsigned char* image = stbi_load(file::resource_path(filename).c_str(),
        &width, &height, &channels, 0);
    if (image == nullptr) {
        MGL_ERROR("Could not load image");
        exit(EXIT_FAILURE);
    }

    #ifdef DEBUG
    else {
        MGL_DEBUG("Loaded successfully");
    }
    #endif

    channels =
        channels == 4 ? GL_RGBA
        : channels == 3 ? GL_RGB
        : channels == 2 ? GL_RG
        : GL_R;

    genAndBindTextureOpenGL(GL_TEXTURE_3D, channels, width, height, image, GL_UNSIGNED_BYTE);

    stbi_image_free(image);
}

void Texture3D::genPerlinNoise(GLuint size, GLuint octaves,
    GLdouble atenuation, GLdouble frequency) {

    float* image = new float[size * size * size * 3];
    double min = 0;
    double max = 0;
    for (size_t x = 0; x < size; x++) {
        for (size_t y = 0; y < size; y++) {
            for (size_t z = 0; z < size; z++) {
                double noise = util::PerlinNoise::getWithPersistance(x, y, z, octaves, atenuation, frequency);
                image[3 * x * size * size + 3 * y * size + z] = noise;
                image[3 * x * size * size + 3 * y * size + z + 1] = noise;
                image[3 * x * size * size + 3 * y * size + z + 2] = noise;
            }
        }
    }

    genAndBindTextureOpenGL(GL_TEXTURE_3D, GL_RGB, size, size, image, GL_FLOAT);
    delete[] image;
}

///////////////////////////////////////////////////////////////// TextureCubeMap

void TextureCubeMap::bind() { glBindTexture(GL_TEXTURE_CUBE_MAP, id); }

void TextureCubeMap::unbind() { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }

const std::string CUBEMAP_SUFFIXES[6] = { "px", "nx",  // right, left
                                          "py", "ny",  // top, bottom
                                          "pz", "nz" };// back, front

const GLenum CUBEMAP_TEXTURES[6] = {
    GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

void TextureCubeMap::loadCubeMap(const std::string& folder, const std::string &fileType) {
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < 6; ++i) {
        std::stringstream ss;
        ss << folder << CUBEMAP_SUFFIXES[i] << "." << fileType;
        std::string filename = ss.str();

        int width, height, channels;

#ifdef DEBUG // DEBUG
        MGL_DEBUG("Loading cubemap file " + filename + "...");
#endif

        unsigned char* image = stbi_load(file::resource_path(filename).c_str(),
                &width, &height, &channels, 0);

        channels =
            channels == 4 ? GL_RGBA
            : channels == 3 ? GL_RGB
            : channels == 2 ? GL_RG
            : GL_R;

        if (image == nullptr) {
            MGL_ERROR("Could not load image: " + filename);
            exit(EXIT_FAILURE);
        }

#ifdef DEBUG
        else {
            MGL_DEBUG("Successfully loaded image");
        }
#endif

        glTexImage2D(CUBEMAP_TEXTURES[i], 0, channels, width, height, 0, channels,
            GL_UNSIGNED_BYTE, image);
        stbi_image_free(image);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

////////////////////////////////////////////////////////////////////////////////
} // namespace mgl
