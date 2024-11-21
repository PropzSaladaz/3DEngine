////////////////////////////////////////////////////////////////////////////////
//
// Mesh Loader Class
//
// Copyright (c)2022-23 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_MESH_HPP
#define MGL_MESH_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

#include <mgl/scene/mglDrawable.hpp>

namespace mgl {

    class Mesh;

/////////////////////////////////////////////////////////////////////////// Mesh

class Mesh : public IDrawable {
public:
    // Specifies the id of the shaders ainput attribute 
    // to which we mesh data.
    // INDEX is sent to shader attribute 0
    // POSITION is sent to shader attribute 1, and so on
    static const GLuint INDEX = 0;
    static const GLuint POSITION = 1;
    static const GLuint NORMAL = 2;
    static const GLuint TEXCOORD = 3;

    Mesh();
    ~Mesh();

    void setAssimpFlags(unsigned int flags);
    void joinIdenticalVertices();
    void generateNormals();
    void generateSmoothNormals();
    void generateTexcoords();
    void flipUVs();

    void create(const std::string &filename);

    bool hasNormals();
    bool hasTexcoords();

protected:
    void performDraw() override;

private:
    GLuint VaoId;
    unsigned int AssimpFlags;
    bool NormalsLoaded, TexcoordsLoaded;

    struct MeshData {
        unsigned int nIndices = 0;
        unsigned int baseIndex = 0;
        unsigned int baseVertex = 0;
    };

    std::vector<MeshData> Meshes;

    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec2> Texcoords;

    std::vector<unsigned int> Indices;

    void processScene(const aiScene *scene);
    void processMesh(const aiMesh *mesh);
    void createBufferObjects();
    void destroyBufferObjects();
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_MESH_HPP  */
