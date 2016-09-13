cmake_minimum_required(VERSION 2.8.11)
project(libworld2d-qt)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS}")

set(INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIBWORLD2D ${INCLUDE} PARENT_SCOPE)
include_directories(${INCLUDE} ${LIBLA})

set(SOURCE source/world.cpp)

add_library(world2d STATIC ${SOURCE})
