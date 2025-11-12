#include <mgl/models/meshes/mglMesh.hpp>
#include <utils/file.hpp>
#include <utils/Logger.hpp>

namespace mgl {

////////////////////////////////////////////////////////////////////////////////

Mesh::Mesh() {
  AssimpFlags = aiProcess_Triangulate;
}

Mesh::~Mesh() { destroyBufferObjects(); }

void Mesh::setAssimpFlags(unsigned int flags) { AssimpFlags = flags; }

void Mesh::joinIdenticalVertices() {
  AssimpFlags |= aiProcess_JoinIdenticalVertices;
}

void Mesh::generateNormals() { AssimpFlags |= aiProcess_GenNormals; }

void Mesh::generateSmoothNormals() {
  AssimpFlags |= aiProcess_GenSmoothNormals;
}

void Mesh::generateTexcoords() { AssimpFlags |= aiProcess_GenUVCoords; }

void Mesh::flipUVs() { AssimpFlags |= aiProcess_FlipUVs; }

bool Mesh::hasNormals() { return _normalsLoaded; }

bool Mesh::hasTexcoords() { return _texcoordsLoaded; }


////////////////////////////////////////////////////////////////////////////////

void Mesh::processMesh(const aiMesh *mesh) {
  MGL_DEBUG("Loading model");
  _normalsLoaded = mesh->HasNormals();
  _texcoordsLoaded = mesh->HasTextureCoords(0);
  _colorsLoaded = mesh->HasVertexColors(0);

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    const aiVector3D &aiPosition = mesh->mVertices[i];
    _positions.push_back(math::vec3(aiPosition.x, aiPosition.y, aiPosition.z));
    if (_normalsLoaded) {
      MGL_DEBUG("Model has normals");
      const aiVector3D &aiNormal = mesh->mNormals[i];
      _normals.push_back(math::vec3(aiNormal.x, aiNormal.y, aiNormal.z));
    }
    if (_texcoordsLoaded) {
      MGL_DEBUG("Model has texture coordinates");
      const aiVector3D &aiTexcoord = mesh->mTextureCoords[0][i];
      _texCoords.push_back(math::vec2(aiTexcoord.x, aiTexcoord.y));
    }
    if (_colorsLoaded) {
      MGL_DEBUG("Model has vertex colors");
      const aiColor4D &aiColor = mesh->mColors[0][i];
      _colors.push_back(
          math::vec4(aiColor.r, aiColor.g, aiColor.b, aiColor.a));
    }
  }
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    const aiFace &face = mesh->mFaces[i];
    _indices.push_back(face.mIndices[0]);
    _indices.push_back(face.mIndices[1]);
    _indices.push_back(face.mIndices[2]);
  }
}

void Mesh::processScene(const aiScene *scene) {
  _meshes.resize(scene->mNumMeshes);
  unsigned int n_vertices = 0;
  unsigned int n_indices = 0;

  for (unsigned int i = 0; i < _meshes.size(); i++) {
    _meshes[i].n_indices = scene->mMeshes[i]->mNumFaces * 3;
    _meshes[i].baseVertex = n_vertices;
    _meshes[i].baseIndex = n_indices;

    n_vertices += scene->mMeshes[i]->mNumVertices;
    n_indices += _meshes[i].n_indices;
  }

  _indices.reserve(n_indices);
  _positions.reserve(n_vertices);
  _normals.reserve(n_vertices);
  _texCoords.reserve(n_vertices);
  _colors.reserve(n_vertices);

  for (unsigned int i = 0; i < _meshes.size(); i++) {
    processMesh(scene->mMeshes[i]);
  }


  MGL_DEBUG("Loaded {} mesh(es) [{} vertices, {} indices, {} triangles]",
            _meshes.size(), n_vertices, n_indices, n_indices / 3);
}

void Mesh::createFromFile(const std::string &filename) {
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      file::resource_path(filename).string(), AssimpFlags);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    MGL_ERROR("Error while loading: {}", importer.GetErrorString());
    exit(EXIT_FAILURE);
  }

  MGL_DEBUG("Processing [{}]", filename);

  processScene(scene);
  createBufferObjects();
}


  void Mesh::createFromData(const MeshData &data) {
      // --- Basic validation ---
      const ui32 positionCount = static_cast<ui32>(data.positions.size());
      if (positionCount == 0) throw std::invalid_argument("createFromData: positions is empty");

      const ui32 indicesCount = static_cast<ui32>(data.indices.size());
      if (indicesCount > 0) {
          for (unsigned int idx : data.indices)
              if (idx >= positionCount)
                  throw std::out_of_range("createFromData: index out of range");
      }

      const ui32 normalsCount = static_cast<ui32>(data.normals.size());
      const ui32 texcoordsCount = static_cast<ui32>(data.texcoords.size());
      const ui32 colorsCount = static_cast<ui32>(data.colors.size());

      // Optional attributes must match vertex count if provided
      if (normalsCount > 0 && normalsCount != positionCount)
          throw std::invalid_argument("createFromData: normals.size() != positions.size()");
      if (texcoordsCount > 0 && texcoordsCount != positionCount)
          throw std::invalid_argument("createFromData: texcoords.size() != positions.size()");
      if (colorsCount > 0 && colorsCount != positionCount)
          throw std::invalid_argument("createFromData: colors.size() != positions.size()");

      // --- Take ownership (moves) ---
      _positions = std::move(data.positions);
      _indices   = std::move(data.indices);

      // Load normals if provided
      if (normalsCount > 0) {
          _normals       = std::move(data.normals);
          _normalsLoaded = true;
      } else {
          _normals.clear();
          _normalsLoaded = false;
      }

      // Load texcoords if provided
      if (texcoordsCount > 0) {
          _texCoords       = std::move(data.texcoords);
          _texcoordsLoaded = true;
      } else {
          _texCoords.clear();
          _texcoordsLoaded = false;
      }

      // Load colors if provided
      if (colorsCount > 0) {
          _colors       = std::move(data.colors);
          _colorsLoaded = true;
      } else {
          _colors.clear();
          _colorsLoaded = false;
      }

      // --- Single-submesh layout ---
      _meshes.resize(1);
      _meshes[0].n_indices   = static_cast<unsigned int>(_indices.size());
      _meshes[0].baseIndex  = 0;
      _meshes[0].baseVertex = 0;

      // --- GPU upload ---
      createBufferObjects();
  }


void Mesh::createBufferObjects() {
  GLuint boId[6];

  glGenVertexArrays(1, &VaoId);
  glBindVertexArray(VaoId);
  {
    glGenBuffers(6, boId);

    // Position is always needed
    glBindBuffer(GL_ARRAY_BUFFER, boId[POSITION]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_positions[0]) * _positions.size(),
                 &_positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION);
    glVertexAttribPointer(POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);



    // Optional attributes
    if (_normalsLoaded) {
      glBindBuffer(GL_ARRAY_BUFFER, boId[NORMAL]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(_normals[0]) * _normals.size(),
                   &_normals[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(NORMAL);
      glVertexAttribPointer(NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (_texcoordsLoaded) {
      glBindBuffer(GL_ARRAY_BUFFER, boId[TEXCOORD]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(_texCoords[0]) * _texCoords.size(),
                   &_texCoords[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(TEXCOORD);
      glVertexAttribPointer(TEXCOORD, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (_colorsLoaded) {
      glBindBuffer(GL_ARRAY_BUFFER, boId[COLOR]);
      glBufferData(GL_ARRAY_BUFFER, sizeof(_colors[0]) * _colors.size(),
                   &_colors[0], GL_STATIC_DRAW);
      glEnableVertexAttribArray(COLOR);
      glVertexAttribPointer(COLOR, 4, GL_FLOAT, GL_FALSE, 0, 0);
    }



    // Indexing is always used as well
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, boId[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(),
                 &_indices[0], GL_STATIC_DRAW);
  }
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(6, boId);
}

void Mesh::destroyBufferObjects() {
  glBindVertexArray(VaoId);
  glDisableVertexAttribArray(POSITION);
  glDisableVertexAttribArray(NORMAL);
  glDisableVertexAttribArray(TEXCOORD);
  glDeleteVertexArrays(1, &VaoId);
  glBindVertexArray(0);
}

void Mesh::performDraw() {
  glBindVertexArray(VaoId);
  for (Submesh &mesh : _meshes) {
    glDrawElementsBaseVertex(
        GL_TRIANGLES, mesh.n_indices, GL_UNSIGNED_INT,
        reinterpret_cast<void *>((sizeof(unsigned int) * mesh.baseIndex)),
        mesh.baseVertex);
  }
  glBindVertexArray(0);
}

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl
