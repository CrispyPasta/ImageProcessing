cmake_minimum_required(VERSION 3.10)

project(ImageProcessing)

find_package(OpenCV REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 14)

add_executable(ImageProcessing main.cpp Matrix/Matrix.h Matrix/Matrix.cpp Color/Color.cpp Color/Color.h Mixing/Mixing.cpp Mixing/Mixing.h Edges/Edges.cpp Edges/Edges.h Utility/Utility.h Histogram/Histogram.cpp Histogram/Histogram.h Histogram/Histogram.cpp Histogram/Histogram.h)

target_link_libraries( ImageProcessing ${OpenCV_LIBS} )
target_link_libraries(ImageProcessing Threads::Threads)
