cmake_minimum_required(VERSION 3.6)
project(raytracing)


SET (CMAKE_CXX_COMPILER  "/usr/local/bin/clang++omp")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -I/usr/local/opt/llvm/include  -O3")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/local/opt/llvm/lib -Wl,-rpath,/usr/local/opt/llvm/lib -lc++")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SOURCE_FILES main.cpp Ray.h Objects.h Objects.cpp World.cpp World.h Light.h Material.h Camera.h)
add_executable(raytracing ${SOURCE_FILES})

target_link_libraries(raytracing ${OpenCV_LIBS} )