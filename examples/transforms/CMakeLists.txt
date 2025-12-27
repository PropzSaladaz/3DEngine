set(APP_TARGET transforms)

add_executable(${APP_TARGET}
  src/main.cpp
)

target_link_libraries(${APP_TARGET}
  PRIVATE
    engine_library
)

set(APP_RESOURCES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resources)

if(EXISTS ${APP_RESOURCES_DIR})
  add_custom_command(TARGET ${APP_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${APP_RESOURCES_DIR}
            $<TARGET_FILE_DIR:${APP_TARGET}>/resources
    COMMENT "Copying transforms resources"
    VERBATIM
  )
endif()
