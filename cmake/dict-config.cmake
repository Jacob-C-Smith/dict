# DICT CMake configuration file:
# This file is meant to be placed in a cmake subfolder of DICT-devel-2.x.y-VC

cmake_minimum_required(VERSION 3.0)

include(FeatureSummary)
set_package_properties(DICT PROPERTIES
    URL "https://www.libsdl.org/"
    DESCRIPTION "low level access to audio, keyboard, mouse, joystick, and graphics hardware"
)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(DICT_FOUND TRUE)

# For compatibility with autotools DICT-config.cmake, provide DICT_* variables.

set_and_check(DICT_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(DICT_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(DICT_INCLUDE_DIR  "${DICT_PREFIX}/include")
set(DICT_INCLUDE_DIRS           "${DICT_INCLUDE_DIR}")
set_and_check(DICT_BINDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")
set_and_check(DICT_LIBDIR       "${CMAKE_CURRENT_LIST_DIR}/../build/Debug/")

set(DICT_LIBRARIES DICT::DICT)

# All targets are created, even when some might not be requested though COMPONENTS.
# This is done for compatibility with CMake generated DICT-target.cmake files.

set(_DICT_library     "${DICT_LIBDIR}/DICT.lib")
set(_DICT_dll_library "${DICT_BINDIR}/DICT.dll")
if(EXISTS "${_DICT_library}" AND EXISTS "${_DICT_dll_library}")
    if(NOT TARGET DICT::DICT)
        add_library(DICT::DICT SHARED IMPORTED)
        set_target_properties(DICT::DICT
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${DICT_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_DICT_library}"
                IMPORTED_LOCATION "${_DICT_dll_library}"
                COMPATIBLE_INTERFACE_BOOL "DICT_SHARED"
                INTERFACE_DICT_SHARED "ON"
        )
    endif()
    set(DICT_DICT_FOUND TRUE)
else()
    set(DICT_DICT_FOUND FALSE)
endif()
unset(_DICT_library)
unset(_DICT_dll_library)

check_required_components(DICT)
