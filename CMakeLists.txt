cmake_minimum_required(VERSION 3.11)
project(IK)

# optimization for dev environments
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project version
set(MAJOR 0)
set(MINOR 1)
set(REMAP_VERSION ${MAJOR}.${MINOR})

# CXX and C versions
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-pthread")

# cmake modules path spec
list(APPEND CMAKE_MODULE_PATH "$ENV{CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# cmake binary path spec
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# optimization
option(OPTIMIZE_CODEBASE "Clang-tidy checks during build" OFF)
if(OPTIMIZE_CODEBASE)

    if(UNIX AND NOT APPLE)
        set(CMAKE_CXX_CLANG_TIDY
            clang-tidy -p pwd
            --checks=-*,readability-*,clang-analyzer-*,performance-*)
    endif()

    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# build shared library (on)
option(BUILD_SHARED_LIBS "Build Shared Library" ON)

# policies
cmake_policy(SET CMP0072 NEW)

# dependencies
find_package(glog REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT REQUIRED)
find_package(GLU REQUIRED)
find_package(OpenGL REQUIRED)
find_package(gflags REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# main project include paths
set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_DIR ${PROJECT_DIR}/src)
set(LIBS_DIR ${PROJECT_DIR}/libs)
set(EXT_DIR ${PROJECT_DIR}/external)

# find header directories
file(GLOB_RECURSE HEADERS
    ${LIBS_DIR}/*.h
    )

set (INCLUDE_DIRS "")

foreach (HEADER ${HEADERS})
    get_filename_component(DIR ${HEADER} PATH)
    list (APPEND INCLUDE_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# src paths
file(GLOB_RECURSE LIBS_SRC ${LIBS_DIR}/*.cpp)

# glad library
add_library(glad ${LIBS_DIR}/glad/src/glad.c)

# link glad libs
target_link_libraries(glad ${CMAKE_DL_LIBS})

# glad header
target_include_directories(glad PRIVATE ${LIBS_DIR}/glad/include)

# project target, includes, and linkages
add_executable(IK
    ${EXT_SRC}
    ${LIBS_SRC}
    ${SRC_DIR}/main.cpp
    )
target_include_directories(IK PRIVATE
    ${GLEW_INCLUDE}
    ${GLUT_INCLUDE}
    ${GLU_INCLUDE}
    ${INCLUDE_DIRS}
    ${GLFW3_INCLUDE}
    ${OPENGL_INCLUDE}
    ${EXTERNAL}/glad/include
    )

target_link_libraries(IK
    glad
    glfw
    glog
    gflags
    OpenGL::GL
    Eigen3::Eigen
    ${GLEW_LIBRARY}
    ${GLUT_LIBRARY}
    ${GLU_LIBRARY}
    )

option(RUN "execute target" ON) # (default ON)
if(RUN)
    add_custom_command(TARGET IK
        POST_BUILD
        COMMAND ./build/bin/IK --logtostderr=1
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
