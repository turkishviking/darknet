cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 11)
project(darknet-cpp-test)

include_directories(../include)

find_package(OpenCV)
if(OpenCV_FOUND)
  add_definitions(-DOPENCV)
  include_directories(${OpenCV_INCLUDE_DIRS})
endif()

add_executable(${PROJECT_NAME}
	src/darknet-cpp-test.cpp
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE darklib
)
set_target_properties(${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_test(
	NAME
		test_darknet-cpp
	COMMAND
		darknet-cpp-test "${CMAKE_CURRENT_SOURCE_DIR}/test_data/"
)

# Make sure yolov3-tiny.weights is present in the test folder or download it
SET(WEIGTHS_FILE "yolov3-tiny.weights")
SET(WEIGTHS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/test_data/${WEIGTHS_FILE}")
SET(WEIGTHS_URL "https://pjreddie.com/media/files/${WEIGTHS_FILE}")
SET(WEIGTHS_MD5 "3bcd6b390912c18924b46b26a9e7ff53")
message(STATUS "Checking/downloading ${WEIGTHS_FILE}...")
file(DOWNLOAD
	${WEIGTHS_URL}
	${WEIGTHS_PATH}
	EXPECTED_MD5 ${WEIGTHS_MD5}
	TLS_VERIFY ON
	STATUS status
)
message(STATUS "Checking/downloading ${WEIGTHS_FILE} - done")

# Make sure yolov3-tiny.weights is present in the test folder or download it
SET(CFG_FILE "yolov3-tiny.cfg")
SET(CFG_PATH "${CMAKE_CURRENT_SOURCE_DIR}/test_data/${CFG_FILE}")
SET(CFG_URL "https://raw.githubusercontent.com/pjreddie/darknet/master/cfg/${CFG_FILE}")
SET(CFG_MD5 "5c837d89621cc057832c3ae83f6b96ac")
message(STATUS "Checking/downloading ${CFG_FILE}...")
file(DOWNLOAD
	${CFG_URL}
	${CFG_PATH}
	EXPECTED_MD5 ${CFG_MD5}
	TLS_VERIFY ON
	STATUS status
)
message(STATUS "Checking/downloading ${CFG_FILE} - done")

#copy files needed for the tests
# <input> relative path is treated with respect to the value of CMAKE_CURRENT_SOURCE_DIR
# <output> relative path is treated with respect to the value of CMAKE_CURRENT_BINARY_DIR
configure_file(test_data/coco.names data/coco.names COPYONLY)
