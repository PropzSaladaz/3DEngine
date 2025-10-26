set(EXAMPLE_TARGET example-lighting-showcase)

add_executable(${EXAMPLE_TARGET}
  src/main.cpp
)

target_link_libraries(${EXAMPLE_TARGET}
  PRIVATE
    engine_library
)

set(EXAMPLE_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)
if(EXISTS ${EXAMPLE_RESOURCES_DIR})
  add_custom_command(TARGET ${EXAMPLE_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${EXAMPLE_RESOURCES_DIR}
            $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>/resources
    COMMENT "Copying lighting-showcase resources"
    VERBATIM
  )
endif()
