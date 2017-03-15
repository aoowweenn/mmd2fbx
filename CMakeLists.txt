cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(pmx2fbx)

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/contrib)

add_library(convertUTF ConvertUTF.c)
add_library(pmx Pmx.cpp)

add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME} PRIVATE pmx)

