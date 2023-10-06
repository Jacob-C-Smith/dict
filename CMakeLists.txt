# Need CMake 3.20
cmake_minimum_required (VERSION 3.20)

# This is the name of the repository
project ("dict")

# Set the correct paths
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/extern/)

# Comment out for Debug mode
set(IS_DEBUG_BUILD CMAKE_BUILD_TYPE STREQUAL "Debug")

# Set debug mode
if (${IS_DEBUG_BUILD})
    add_compile_definitions(NDEBUG)
endif()

# Find the sync module
if ( NOT "${HAS_SYNC}")

    # Log
    message("[dict] Adding sync project")

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    set(HAS_SYNC true PARENT_SCOPE)
endif()

# Find the crypto module
if ( NOT "${HAS_CRYPTO}")

    # Log
    message("[dict] Adding crypto project")

    # Add the crypto directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/crypto ${CMAKE_SOURCE_DIR}/build/crypto)

    # Let any parent project know that this project has crypto
    set(HAS_CRYPTO true PARENT_SCOPE)
endif()

# Add the dict project
if ( NOT "${HAS_DICT}")

    # Log
    message("[dict] Adding dict project")

    # Let any parent project know that it has the dict project
    set(HAS_DICT true PARENT_SCOPE)
endif()

# Add source to this project's executable.
add_executable(dict_example "main.c" "dict.c")
add_dependencies(dict_example sync crypto)
target_include_directories(dict_example PUBLIC include include/dict ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/crypto/include/)
target_link_libraries(dict_example sync crypto)

# Add source to the tester
add_executable (dict_test "dict_test.c" "dict.c")
add_dependencies(dict_test sync crypto)
target_include_directories(dict_test PUBLIC include include/dict ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/crypto/include/)
target_link_libraries(dict_test sync crypto)

# Add source to the library
add_library(dict SHARED "dict.c")
add_dependencies(dict sync crypto)
target_include_directories(dict PUBLIC include include/dict ${CMAKE_SOURCE_DIR}/extern/sync/include/ ${CMAKE_SOURCE_DIR}/extern/crypto/include/)
target_link_libraries(dict sync crypto)
