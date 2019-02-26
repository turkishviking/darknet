# Have a look to that excellent answer on SO:
# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)

# set(CMAKE_CXX_STANDARD 11)
project(darknet-cpp VERSION 0.0.1 )

option(BUILD_SHARED_LIBS "Build a shared library" TRUE)
# option(BUILD_TESTING "Create tests using CMake" FALSE)
option(USE_GPU "Use GPU support" TRUE)
option(USE_CUDNN "Use cuDNN support" TRUE)
# option(INSTALL_SOURCE_HEADERS "Install header files from the src directory" FALSE)
add_subdirectory(darknet)
#add_subdirectory(test)

find_package(CUDA REQUIRED)

find_package(OpenCV)
if(OpenCV_FOUND)
  add_definitions(-DOPENCV)
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    darknet/include
    )

link_directories(
    ${PROJECT_NAME}
    /opt/cuda/lib64
    )

add_executable(
    ${PROJECT_NAME}
    test/src/darknet-cpp-test.cpp
    src/darknetpp.cpp
    )

target_link_libraries(
    ${PROJECT_NAME}
    darklib
    ${OpenCV_LIBS}
    ${CUDA_LIBRARIES}
    ${CUDA_CUBLAS_LIBRARIES}
    ${CUDA_curand_LIBRARY}
    )
