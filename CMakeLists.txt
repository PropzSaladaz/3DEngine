# CMakeList.txt : CMake project for lab_1, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)


set(PROJECT_NAME "engine")
set(EXEC_NAME "engine")

# Set the build type to Debug by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

project (${PROJECT_NAME})

# SOURCE FILES
set(SOURCES
  # core
  src/main.cpp
  src/glad.c
  src/mgl/mglApp.cpp
  src/mgl/mglError.cpp
  src/mgl/mglShaders.cpp
  src/mgl/mglTransform.cpp
  src/mgl/mglSimulation.cpp
  src/mgl/mglAnimation.cpp
  src/mgl/mglInputManager.cpp
  src/mgl/mglShaderManager.cpp
  # utils
  src/utils/logger.cpp
  src/utils/stb_image.cpp
  # models
  src/mgl/models/materials/mglMaterial.cpp
  src/mgl/models/materials/mglBasicMaterial.cpp
  src/mgl/models/materials/mglPhongMaterial.cpp
  src/mgl/models/meshes/mglMesh.cpp
  src/mgl/models/meshes/mglMeshManager.cpp
  src/mgl/models/textures/mglTexture.cpp
  src/mgl/models/textures/mglSampler.cpp
  # scene
  src/mgl/scene/mglLight.cpp
  src/mgl/scene/mglLightManager.cpp
  src/mgl/scene/mglPointLight.cpp
  src/mgl/scene/mglDirectionalLight.cpp
  src/mgl/scene/mglPositionalLight.cpp
  src/mgl/scene/mglSpotLight.cpp
  src/mgl/scene/mglSceneGraph.cpp
  src/mgl/scene/mglSceneObject.cpp
  # camera
  src/mgl/camera/mglCamera.cpp
  src/mgl/camera/mglCameraController.cpp
  src/mgl/camera/mglFPSCamController.cpp
  src/mgl/camera/mglOrbitCamController.cpp
  src/mgl/camera/mglPerspectiveCamera.cpp
)

# INCLUDE
set(INCLUDE_DIRS
	include
	include/mgl
	include/mgl/camera
	include/mgl/scene
	include/mgl/models
	include/utils
)

# EXTERNAL LIBRARIES DIRECTORIES
set(INCLUDE_LIBS_DIR
	libs
)

set(INCLUDE_LIBS_NAME
  glfw3
  assimp-vc143-mt
)

# DLLS
set(DLLS
	libs/assimp/assimp-vc143-mt.dll
)

# RESOURCES
set(RESOURCES resources)

include_directories(${INCLUDE_DIRS})
link_directories(${INCLUDE_LIBS_DIR})

# used for the tests
add_library(engine_library ${SOURCES})
target_link_libraries(engine_library ${INCLUDE_LIBS_NAME})

add_executable(       ${EXEC_NAME} ${SOURCES})
include_directories(  ${EXEC_NAME} ${INCLUDE_LIBS_DIR})
target_link_libraries(${EXEC_NAME} ${INCLUDE_LIBS_NAME})

# Different build configs
if(CMAKE_BUILD_TYPE STREQUAL "Debug") # DEBUG
  message("Debug build configuration")
  set(BUILD_DEST "out/build/x64-debug")
  target_compile_definitions(${EXEC_NAME} PRIVATE DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release") # RELEASE
  message("Release build configuration")
  set(BUILD_DEST "./release")
  target_compile_definitions(${EXEC_NAME} PRIVATE RELEASE)
  # Copy resources folder to release
  file(COPY ${CMAKE_SOURCE_DIR}/${RESOURCES} 
  DESTINATION ${CMAKE_SOURCE_DIR}/${BUILD_DEST}/)
endif()

# copy all .dlls to build target where .exe is
foreach(file ${DLLS})
	configure_file(
		${CMAKE_SOURCE_DIR}/${file}
		${CMAKE_SOURCE_DIR}/${BUILD_DEST}/
		COPYONLY
	)
endforeach()
