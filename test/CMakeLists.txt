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
# <input> relative path is treated with respect to the value of CMAKE_CURRENT_SOURCE_DIR
# <output> relative path is treated with respect to the value of CMAKE_CURRENT_BINARY_DIR
configure_file(test_data/coco.names data/coco.names COPYONLY)
configure_file(test_data/coco.data data/coco.data COPYONLY)
configure_file(test_data/yolov3.cfg data/yolov3.cfg COPYONLY)
configure_file(test_data/yolov3.weights data/yolov3.weights COPYONLY)
configure_file(test_data/dog.jpg data/dog.jpg COPYONLY)
configure_file(test_data/eagle.jpg data/eagle.jpg COPYONLY)
configure_file(test_data/giraffe.jpg data/giraffe.jpg COPYONLY)
configure_file(test_data/horses.jpg data/horses.jpg COPYONLY)