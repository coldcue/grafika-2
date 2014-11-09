cmake_minimum_required(VERSION 2.8.4)
project(grafika-2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
set(SOURCE_FILES grafika-2/main.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
add_compile_options("-Wno-deprecated-declarations")

add_executable(grafika-2 ${SOURCE_FILES})
target_link_libraries(grafika-2 ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
