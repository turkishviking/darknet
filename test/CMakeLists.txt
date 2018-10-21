cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
project(darknet-cpp-test)

add_executable(${PROJECT_NAME}
	src/darknet-cpp-test.cpp
)
target_link_libraries(${PROJECT_NAME}
	PRIVATE darknet-cpp
)
add_test(test_darknet-cpp darknet-cpp-test)

#copy files needed for the tests
configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/data/coco.names
	${CMAKE_CURRENT_BINARY_DIR}/data/coco.names
)
