cmake_minimum_required(VERSION 2.8.9)

project(mmgirgenerator)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -std=c++14")
SET(PROJECT_SOURCE_DIR "${mmgirgenerator_SOURCE_DIR}/src")

INCLUDE(FindPkgConfig)

pkg_check_modules(GLIBMM24 REQUIRED glibmm-2.4)
include_directories(${GLIBMM24_INCLUDE_DIRS})
link_directories(${GLIBMM24_LIBRARY_DIRS})

pkg_check_modules(LIBXMLPP3 REQUIRED libxml++-3.0)
include_directories(${LIBXMLPP3_INCLUDE_DIRS})
link_directories(${LIBXMLPP3_LIBRARY_DIRS})

pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
include_directories(${GOBJECT_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(verifier)
