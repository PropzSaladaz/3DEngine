cmake_minimum_required(VERSION 3.16)

# ---- Project -----------------------------------------------------------------
project(engine LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# ---- Sources -----------------------------------------------------------------
# Split: library = engine core; executable = just main.cpp
set(ENGINE_SOURCES
  # glad + core
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

set(EXEC_NAME engine)
set(RESOURCES_DIR ${CMAKE_SOURCE_DIR}/resources)

# Optional: output dirs (bin/lib per-config)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# ---- Include paths -----------------------------------------------------------
set(PROJECT_INCLUDES
  include
  include/mgl
  include/mgl/camera
  include/mgl/scene
  include/mgl/models
  include/utils
)

# ---- Dependencies ------------------------------------------------------------
# GL loader = glad.c compiled above. GLFW gives context/window, Assimp loads models.

# GLFW (package name is glfw3, target is usually 'glfw')
find_package(glfw3 3.3 REQUIRED)

# Assimp provides imported target 'assimp::assimp' on Linux/Windows when installed via package manager
find_package(assimp REQUIRED)

# Threads (for GLFW transitive needs on some platforms)
find_package(Threads REQUIRED)

# Linux: some distros require dl explicitly; add if available
include(CheckLibraryExists)
set(HAVE_LIBDL OFF)
check_library_exists(dl dlopen "" HAVE_LIBDL)

# ---- Library: engine_library -------------------------------------------------
add_library(engine_library STATIC ${ENGINE_SOURCES})
target_include_directories(engine_library
  PUBLIC ${PROJECT_INCLUDES} 
  ${CMAKE_CURRENT_SOURCE_DIR}/libs
)
target_compile_definitions(engine_library
  PRIVATE $<$<CONFIG:Debug>:DEBUG>
          $<$<CONFIG:Release>:NDEBUG>
)
target_link_libraries(engine_library
  PUBLIC
    glfw                          # imported target from find_package(glfw3)
    assimp::assimp                # imported target from find_package(assimp)
    Threads::Threads
)
if(UNIX AND HAVE_LIBDL)
  target_link_libraries(engine_library PUBLIC dl)
endif()

# ---- Executable: engine (only main.cpp) --------------------------------------
add_executable(${EXEC_NAME} src/main.cpp)
target_include_directories(${EXEC_NAME} PRIVATE ${PROJECT_INCLUDES})
target_link_libraries(${EXEC_NAME} PRIVATE engine_library)

# ---- Post-build: copy resources next to the executable -----------------------
add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${RESOURCES_DIR}
          $<TARGET_FILE_DIR:${EXEC_NAME}>/resources
  COMMENT "Copying resources/ to output directory"
  VERBATIM
)

# ---- Windows-only: copy required DLLs (if you vendor them) -------------------
if(WIN32)
  # Example: adjust paths if you actually ship local DLLs in repo
  set(DLLS
    ${CMAKE_SOURCE_DIR}/libs/assimp/assimp-vc143-mt.dll
  )
  foreach(dll ${DLLS})
    if(EXISTS "${dll}")
      add_custom_command(TARGET ${EXEC_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${dll}" $<TARGET_FILE_DIR:${EXEC_NAME}>
        COMMENT "Copying ${dll} to output directory"
      )
    endif()
  endforeach()
endif()
