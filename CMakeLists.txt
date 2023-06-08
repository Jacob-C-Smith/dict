cmake_minimum_required (VERSION 3.26)

project ("dict")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")
if ( NOT "${HAS_DICT}")
    message("[dict] Adding dict project")
    set(HAS_DICT true PARENT_SCOPE)
endif()

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Add source to this project's executable.
add_executable(dict_example "main.c" "dict.c")
target_include_directories(dict_example PUBLIC include)

# Add source to the tester
add_executable (dict_test "dict_test.c" "dict.c")
target_include_directories(dict_test PUBLIC include)

# Add source to the library
add_library(dict SHARED "dict.c")
target_include_directories(dict PUBLIC include include/dict)
