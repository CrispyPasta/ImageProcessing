cmake_minimum_required(VERSION 3.10)

project(ImageProcessing)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

add_executable(ImageProcessing main.cpp Matrix/Matrix.cpp Matrix/Matrix.h Color/Color.cpp Color/Color.h)

target_link_libraries( ImageProcessing ${OpenCV_LIBS} )