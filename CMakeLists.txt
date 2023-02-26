cmake_minimum_required (VERSION 3.8)

project ("dict")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif ()

# Add source to this project's executable.
add_executable (dict_example "dict.c" "main.c")

# Add source to the tester.
add_executable (dict_test "dict.c" "dict_test.c")

# Add source to this project's library
add_library    (dict SHARED "dict.c")

# Define targets
target_include_directories(dict_example PUBLIC "include/")
target_include_directories(dict_test PUBLIC "include/")
target_include_directories(dict PUBLIC "include/")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET dict PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
