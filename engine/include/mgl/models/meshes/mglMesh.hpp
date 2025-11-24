#pragma once

#include "types.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "math/math.hpp"

#include <assimp/Importer.hpp>
#include <stdexcept>
#include <string>
#include <vector>

#include <mgl/scene/mglDrawable.hpp>

namespace mgl {


    // @brief Structure to hold mesh data with generic types.
    // Usually used for mesh initialization
    struct MeshData {
        std::vector<math::vec3> positions;
        std::vector<ui32> indices;
        std::vector<math::vec3> normals;
        std::vector<math::vec2> texcoords;

        // TODO maybe we could make the entire vec the generic type.
        // users may want to use vec3 instead of vec4
        std::vector<math::vec4> colors;
    };

    class Mesh : public IDrawable {
    public:
        // Specifies the id of the shaders ainput attribute 
        // to which we mesh data.
        // INDEX is sent to shader attribute 0
        // POSITION is sent to shader attribute 1, and so on
        static const u8 INDEX = 0;
        static const u8 POSITION = 1;
        static const u8 NORMAL = 2;
        static const u8 TEXCOORD = 3;
        static const u8 COLOR = 4;

        Mesh();
        ~Mesh();

        void setAssimpFlags(unsigned int flags);
        void joinIdenticalVertices();
        void generateNormals();
        void generateSmoothNormals();
        void generateTexcoords();
        void flipUVs();

        /**
         * @brief Loads a mesh from a file using Assimp library.
         */
        void createFromFile(const std::string &filename);

        /**
         * @brief Creates a mesh from raw vertex data.
         */
        // void createFromData(std::vector<math::vec3> positions,
        //                     std::vector<unsigned int> indices,
        //                     std::vector<math::vec3> normals,
        //                     std::vector<math::vec2> texcoords);

        /**
         * @brief Creates a mesh from raw vertex data using MeshData structure.
         * All attributes are optional except positions and indices
         */
        void createFromData(const MeshData &data);

        bool hasNormals();
        bool hasTexcoords();

    protected:
        void performDraw() override;

    private:
        ui32 VaoId = 0;
        GLuint BufferIds[6] = {0, 0, 0, 0, 0, 0};
        unsigned int AssimpFlags = 0;
        bool _normalsLoaded = false;
        bool _texcoordsLoaded = false;
        bool _colorsLoaded = false;

        struct Submesh {
            unsigned int n_indices = 0;
            unsigned int baseIndex = 0;
            unsigned int baseVertex = 0;
        };

        std::vector<Submesh> _meshes;

        std::vector<unsigned int> _indices;
        std::vector<math::vec3> _positions;
        std::vector<math::vec3> _normals;
        std::vector<math::vec2> _texCoords;
        std::vector<math::vec4> _colors;

        /**
         * @brief Processes the entire aiScene from Assimp and loads all meshes.
         * Used internally by createFromFile().
         */
        void processScene(const aiScene *scene);

        /**
         * @brief Processes an individual aiMesh from Assimp and loads its data.
         * Used internally by processScene().
         */
        void processMesh(const aiMesh *mesh);

        /**
         * @brief Creates OpenGL buffer objects and uploads mesh data to GPU.
         * Currently it builds all vertex data using a single VAO, and separate VBOs.
         * It uses a separate approach (data is not set contiguous in a single buffer).
         * 
         * TODO - this should be updated to be optimized later on to be set interleaved (continuous 
         * in a single buffer with proper offsets).
         */
        void createBufferObjects();
        void destroyBufferObjects();
    };

}  // namespace mgl
