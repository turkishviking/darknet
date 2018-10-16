cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_STANDARD 11)
project(arapaho)

add_definitions(-DGPU)
add_definitions(-DCUDNN)

find_package( OpenCV REQUIRED )

if(EXISTS "/etc/arch-release") #arch like cuda path
    include_directories(/opt/cuda/include)
    link_directories(/opt/cuda/lib64)
else()
    #debian like cuda path
    include_directories(/usr/local/cuda/include)
    link_directories(/usr/local/cuda/lib64)
endif()

link_directories(${CMAKE_SOURCE_DIR})

add_executable(
    ${PROJECT_NAME}
    test.cpp
    arapaho.cpp
    yolopp.cpp
    )

target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_SOURCE_DIR}
    PUBLIC ../src
    PUBLIC ../include
    )



target_link_libraries(
    ${PROJECT_NAME}
    ${OpenCV_LIBS}
    darknet-cpp
    -lcuda -lcudart -lcublas -lcurand
  )
