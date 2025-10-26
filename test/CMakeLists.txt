find_package(GTest QUIET)

if(NOT GTest_FOUND)
    message(WARNING "GoogleTest not found. Unit tests will be skipped. Install googletest or set BUILD_TESTING=OFF.")
    return()
endif()

file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)

set(TESTS_BIN_DIR ${CMAKE_BINARY_DIR}/bin/tests)

# for each lib in test/
foreach(SUBDIR ${SUBDIRS})
    set(SUBDIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}")
    if(IS_DIRECTORY "${SUBDIR_PATH}")
        # get all .cpp files
        file(GLOB TEST_SOURCES "${SUBDIR_PATH}/*.cpp")
        if(TEST_SOURCES)
            # add executable
            set(TEST_EXEC Test_${SUBDIR})
            add_executable(${TEST_EXEC} ${TEST_SOURCES})
            target_link_libraries(${TEST_EXEC}
                PRIVATE
                    engine_library
                    GTest::gtest_main
            )
            include(GoogleTest)

            # set output directory
            set_target_properties(${TEST_EXEC} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${TESTS_BIN_DIR}"
            )

            gtest_discover_tests(${TEST_EXEC})
        endif()
    endif()
endforeach()
