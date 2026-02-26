cmake_minimum_required(VERSION 3.31.0)
project(llmetallm LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (KERNEL_DIR ${CMAKE_SOURCE_DIR}/kernels)

# Path to metal-cpp headers
include_directories(${CMAKE_SOURCE_DIR}/metal)

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/default.metallib
    COMMAND xcrun -sdk macosx metal -c ${KERNEL_DIR}/matrix.metal -o ${CMAKE_BINARY_DIR}/matrix.air
    COMMAND xcrun -sdk macosx metallib ${CMAKE_BINARY_DIR}/matrix.air -o ${CMAKE_BINARY_DIR}/default.metallib
    DEPENDS ${CMAKE_SOURCE_DIR}/kernels/matrix.metal
    COMMENT "Compiling Metal kernels..."
)

add_executable(llmetallm main.cpp metal_impl.cpp)

add_custom_target(metal_lib ALL DEPENDS ${CMAKE_BINARY_DIR}/default.metallib)
add_dependencies(llmetallm metal_lib)


find_library(METAL Metal)
find_library(FOUNDATION Foundation)
find_library(QUARTZCORE QuartzCore)
find_library(APPKIT AppKit )

target_link_libraries(llmetallm
  "-framework Metal"
  "-framework Foundation"
  "-framework QuartzCore"
  "-framework AppKit"
)
