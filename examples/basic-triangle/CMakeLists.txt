set(EXAMPLE_TARGET example-basic-triangle)

add_executable(${EXAMPLE_TARGET}
  src/main.cpp
)

target_link_libraries(${EXAMPLE_TARGET}
  PRIVATE
    engine_library
)
