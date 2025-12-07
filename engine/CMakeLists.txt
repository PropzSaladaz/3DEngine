set(ENGINE_SOURCES
  src/glad.c
  src/utils/stb_image.cpp
  src/mgl/mglApp.cpp
  src/mgl/mglAnimation.cpp
  src/mgl/mglError.cpp
  src/mgl/mglInputManager.cpp
  src/mgl/shaders/ShaderBuilder.cpp
  src/mgl/shaders/ShaderProgram.cpp
  src/mgl/mglShaderManager.cpp
  src/mgl/mglSimulation.cpp
  src/mgl/mglTransform.cpp
  src/mgl/camera/mglCamera.cpp
  src/mgl/camera/mglCameraController.cpp
  src/mgl/camera/mglFPSCamController.cpp
  src/mgl/camera/mglOrbitCamController.cpp
  src/mgl/camera/mglPerspectiveCamera.cpp
  src/mgl/models/materials/mglBasicMaterial.cpp
  src/mgl/models/materials/mglMaterial.cpp
  src/mgl/models/materials/mglPhongMaterial.cpp
  src/mgl/models/meshes/mglMesh.cpp
  src/mgl/models/meshes/mglMeshManager.cpp
  src/mgl/models/textures/mglSampler.cpp
  src/mgl/models/textures/mglTexture.cpp
  src/mgl/models/textures/mglTextureSampler.cpp
  src/mgl/scene/mglDirectionalLight.cpp
  src/mgl/scene/mglLight.cpp
  src/mgl/scene/mglLightManager.cpp
  src/mgl/scene/mglPointLight.cpp
  src/mgl/scene/mglPositionalLight.cpp
  src/mgl/scene/mglSceneGraph.cpp
  src/mgl/scene/mglSceneObject.cpp
  src/mgl/scene/mglSpotLight.cpp
)

include(FetchContent)
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.13.0
)
FetchContent_MakeAvailable(spdlog)

find_package(glfw3 3.3 REQUIRED)
find_package(assimp REQUIRED)
find_package(Threads REQUIRED)

include(CheckLibraryExists)
set(HAVE_LIBDL OFF)
check_library_exists(dl dlopen "" HAVE_LIBDL)

add_library(engine_library STATIC ${ENGINE_SOURCES})
add_library(engine::library ALIAS engine_library)

target_include_directories(engine_library
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/libs
)

target_link_libraries(engine_library
  PUBLIC
    glfw
    assimp::assimp
    Threads::Threads
    spdlog::spdlog
)

if(UNIX AND HAVE_LIBDL)
  target_link_libraries(engine_library PUBLIC dl)
endif()

target_compile_definitions(engine_library
  PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
