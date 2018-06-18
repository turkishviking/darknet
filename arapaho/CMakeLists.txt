cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project(arapaho)


#cuda stuff for yolo
include_directories(/usr/local/cuda-8.0/include)# path to cuda include
link_directories(/usr/local/cuda/lib64)# path to cuda src
add_definitions(-DGPU)
add_definitions(-DCUDNN)
#yolo
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../src)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/../include)
link_directories(${CMAKE_SOURCE_DIR}/yolo)

FILE(GLOB_RECURSE libFiles "${CMAKE_SOURCE_DIR}/*.h")
add_custom_target(headers SOURCES ${libFiles})

FILE(GLOB_RECURSE srcFiles "${CMAKE_SOURCE_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${srcFiles})

target_link_libraries(  ${PROJECT_NAME}
                        darknet-cpp-shared
                        -lcuda -lcudart -lcublas -lcurand
                      )
