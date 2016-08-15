cmake_minimum_required(VERSION 2.6)
project(2d-world-framework)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

set(SOURCE "")

# add_library(${PROJECT_NAME} STATIC ${SOURCE})
# target_link_libraries(${PROJECT_NAME} Qt5Core)
