# Need CMake 3.0
cmake_minimum_required (VERSION 3.28.0)

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

    # Add the sync directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/sync ${CMAKE_SOURCE_DIR}/build/sync)

    # Let any parent project know that this project has sync
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_SYNC true PARENT_SCOPE)
        set(SYNC_INCLUDE_DIR ${SYNC_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the crypto module
if ( NOT "${HAS_CRYPTO}")

    # Add the crypto directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/crypto ${CMAKE_SOURCE_DIR}/build/crypto)

    # Let any parent project know that this project has crypto
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_CRYPTO true PARENT_SCOPE)
        set(CRYPTO_INCLUDE_DIR ${CRYPTO_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()

# Find the log module
if ( NOT "${HAS_LOG}")
    
    # Add the log directory
    add_subdirectory(${CMAKE_SOURCE_DIR}/extern/log ${CMAKE_SOURCE_DIR}/build/log)

    # Let any parent project know that this project has log
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_LOG true PARENT_SCOPE)
        set(LOG_INCLUDE_DIR ${LOG_INCLUDE_DIR} PARENT_SCOPE)
    endif()
endif()


# Add the dict project
if ( NOT "${HAS_DICT}")

    # Log
    message("[dict] Adding dict project")
    
    # Set the include directory
    set(DICT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)

    # Let any parent project know that it has the dict project
    if(NOT PROJECT_IS_TOP_LEVEL)
        set(HAS_DICT true PARENT_SCOPE)
        set(DICT_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include PARENT_SCOPE)
    endif()
endif()

# Add source to this project's executable.
add_executable(dict_example "main.c" "dict.c")
add_dependencies(dict_example dict)
target_include_directories(dict_example PUBLIC ${DICT_INCLUDE_DIR})
target_link_libraries(dict_example dict)

# Add source to the tester
add_executable (dict_test "dict_test.c" "dict.c")
add_dependencies(dict_test sync dict log)
target_include_directories(dict_test PUBLIC ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${LOG_INCLUDE_DIR})
target_link_libraries(dict_test sync dict log)

# Add source to the library
add_library(dict SHARED "dict.c")
add_dependencies(dict sync crypto)
target_include_directories(dict PUBLIC ${DICT_INCLUDE_DIR} ${SYNC_INCLUDE_DIR} ${CRYPTO_INCLUDE_DIR})
target_link_libraries(dict sync crypto)
