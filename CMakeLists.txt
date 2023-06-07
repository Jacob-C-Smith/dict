cmake_minimum_required (VERSION 3.26.0)

project ("dict")

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Add source to this project's executable.
add_executable(dict_example "main.c")
target_include_directories(dict_example PUBLIC include)
target_link_libraries(dict_example PUBLIC dict)

# Add source to the tester
add_executable (dict_test "dict_test.c")
target_include_directories(dict_test PUBLIC include)
target_link_libraries(dict_test PUBLIC dict)

# Add source to the library
add_library(dict STATIC "dict.c")
target_include_directories(dict PUBLIC include include/dict)
