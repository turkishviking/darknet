# Have a look to that excellent answer on SO:
# https://stackoverflow.com/questions/17511496/how-to-create-a-shared-library-with-cmake
cmake_minimum_required(VERSION 3.5)
# set(CMAKE_CXX_STANDARD 11)
project(darknet-cpp VERSION 0.0.1)

option(BUILD_SHARED_LIBS "Build a shared library" TRUE)
# option(BUILD_TESTING "Create tests using CMake" FALSE)
option(USE_GPU "Use GPU support" FALSE)
option(USE_CUDNN "Use cuDNN support" FALSE)
# option(INSTALL_SOURCE_HEADERS "Install header files from the src directory" FALSE)

##
# Let the user know what options are selected
##
if(BUILD_SHARED_LIBS)
	message(STATUS "Configure as a SHARED library")
else()
	message(STATUS "Configure as a STATIC library")
endif()
if(USE_GPU)
	message(STATUS "GPU support ON")
else()
	message(STATUS "GPU support OFF")
endif()
if(USE_CUDNN)
	message(STATUS "cuDNN support ON")
else()
	message(STATUS "cuDNN support OFF")
endif()
# if(BUILD_TESTING)
# 	message(STATUS "Configure tests")
# endif()
# message(" - Install headers ........ ${INSTALL_SOURCE_HEADERS}")

##
# GPU support (depends on CUDA)
##
# That part might be rewritten using a Cuda target
if(USE_GPU)
	find_package(CUDA REQUIRED)

	# ##
	# # Uncoment to output paths to cuda libraries (returned by find_package)
	# ##
	# message("")
	# message("CUDA_CUDA_LIBRARY: ${CUDA_CUDA_LIBRARY}")
	# message("CUDA_LIBRARIES: ${CUDA_LIBRARIES}")
	# message("CUDA_CUBLAS_LIBRARIES: ${CUDA_CUBLAS_LIBRARIES}")
	# message("CUDA_curand_LIBRARY: ${CUDA_curand_LIBRARY}")
	# message("CUDA_TOOLKIT_ROOT_DIR: ${CUDA_TOOLKIT_ROOT_DIR}")
	# message("")

	SET(CUDA_PROPAGATE_HOST_FLAGS OFF)
	CUDA_INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)

	## Manually set the path to Cuda library if find_package could find it
	## May not be needed anymore if we don't link to it !
	# if(NOT CUDA_CUDA_LIBRARY)
	# 	SET(CUDA_CUDA_LIBRARY "/usr/local/cuda/targets/x86_64-linux/lib/stubs/libcuda.so")
	# 	message(STATUS "Cuda library path is not set, setting to ${CUDA_CUDA_LIBRARY}")
	# endif()

	## List of nvcc compile flags
	list(APPEND CUDA_NVCC_FLAGS
		-gencode arch=compute_30,code=sm_30
		-gencode arch=compute_35,code=sm_35
		-gencode arch=compute_50,code=[sm_50,compute_50]
		-gencode arch=compute_52,code=[sm_52,compute_52]
		-Xcompiler
		-fPIC
		-DGPU
	)
	if(USE_CUDNN)
		list(APPEND CUDA_NVCC_FLAGS -DCUDNN)
	endif()

	## Cuda compilation
	## Uncoment to get verbose output
	# set(CUDA_VERBOSE_BUILD ON)
	CUDA_COMPILE(CUDA_SRC
		src/convolutional_kernels.cu
		src/deconvolutional_kernels.cu
		src/activation_kernels.cu
		src/im2col_kernels.cu
		src/col2im_kernels.cu
		src/blas_kernels.cu
		src/crop_layer_kernels.cu
		src/dropout_layer_kernels.cu
		src/maxpool_layer_kernels.cu
		src/avgpool_layer_kernels.cu
		# src/softmax_layer_kernels.cu # @todo is this important ? Raises compilation errors
	)
endif()

##
# CUDNN support, (depends on GPU support)
##
if(USE_CUDNN)
	if(NOT USE_GPU)
		message(FATAL_ERROR "cuDNN support requires GPU support")
	endif()

	find_library(CUDNN_LIBRARIES
		NAMES cudnn libcudnn
		HINTS ${CUDA_TOOLKIT_ROOT_DIR}
		${CUDNN_TOOLKIT_ROOT_DIR}
		PATH_SUFFIXES lib lib64 lib/x64 lib/x86 targets/aarch64-linux
	)

	if(NOT CUDNN_LIBRARIES)
		# Set this if cuda and cudnn are in seperate locations
		set(CUDNN_TOOLKIT_ROOT_DIR "" CACHE PATH "cuDNN root folder")
		message(FATAL_ERROR "Unable to find cuDNN libraries, please ensure \
			CUDA_TOOLKIT_ROOT_DIR has cuDNN or the CUDNN_TOOLKIT_ROOT_DIR variable is \
			properly set or set CUDNN_LIBRARIES")
	endif()

	if(CUDNN_TOOLKIT_ROOT_DIR)
		include_directories(SYSTEM ${CUDNN_TOOLKIT_ROOT_DIR}/include)
	endif()
endif()

##
# OpenCV dependency
##
# available modules:
# opencv_aruco        opencv_bgsegm             opencv_bioinspired
# opencv_calib3d      opencv_ccalib             opencv_core
# opencv_datasets     opencv_dpm                opencv_face
# opencv_features2d   opencv_flann              opencv_freetype
# opencv_fuzzy        opencv_hdf                opencv_hfs
# opencv_highgui      opencv_img_hash           opencv_imgcodecs
# opencv_imgproc      opencv_line_descriptor    opencv_ml
# opencv_objdetect    opencv_optflow            opencv_phase_unwrapping
# opencv_photo        opencv_plot               opencv_reg
# opencv_rgbd         opencv_saliency           opencv_shape
# opencv_stereo       opencv_stitching          opencv_structured_light
# opencv_superres     opencv_surface_matching   opencv_tracking
# opencv_video        opencv_videoio            opencv_videostab
# opencv_ximgproc     opencv_xobjdetect         opencv_xphoto
find_package(OpenCV
	REQUIRED
		core
		highgui
	OPTIONAL_COMPONENTS
		imgproc
		videoio
		imgcodecs
)
##
# Build the lib
##
# @WARNING markand@freenode/#cmake reported that genex can cause problems in exported targets
#          in case of doubt replace genex with classic if()endif()
add_library(${PROJECT_NAME}
	src/activation_layer.c
	src/activations.c
	src/avgpool_layer.c
	src/batchnorm_layer.c
	src/blas.c
	src/box.c
	src/col2im.c
	src/connected_layer.c
	src/convolutional_layer.c
	src/cost_layer.c
	src/crnn_layer.c
	src/crop_layer.c
	src/cuda.c
	src/data.c
	src/deconvolutional_layer.c
	src/demo.c
	src/detection_layer.c
	src/dropout_layer.c
	src/gemm.c
	src/gru_layer.c
	src/im2col.c
	src/image.c
	src/l2norm_layer.c
	src/layer.c
	src/list.c
	src/local_layer.c
	src/logistic_layer.c
	src/lstm_layer.c
	src/matrix.c
	src/maxpool_layer.c
	src/network.c
	src/normalization_layer.c
	src/option_list.c
	src/parser.c
	src/region_layer.c
	src/reorg_layer.c
	src/rnn_layer.c
	src/route_layer.c
	src/shortcut_layer.c
	src/softmax_layer.c
	src/tree.c
	src/upsample_layer.c
	src/utils.c
	src/yolo_layer.c
	# cpp wrapper from https://github.com/prabindh/darknet/tree/master/arapaho
	src/arapaho.cpp
	# cpp wrapper from https://github.com/turkishviking/darknet/tree/master/arapaho
	src/darknetpp.cpp
	# add cuda compile result (empty if USE_GPU is OFF
	${CUDA_SRC}
)

## Set compile options
target_compile_options(${PROJECT_NAME} PRIVATE
	-Wall
	-Wfatal-errors
	-Wno-unknown-pragmas
	-Wno-unused-variable
	-Wno-unused-value
	-Wno-unused-result
	# There is a compile error happening only when optimisation is not on.
	# Needed on Cygwin + OpenCV-3.4.1 + Intel Core i5 3570
	# see https://github.com/leggedrobotics/darknet_ros/issues/1#issuecomment-298329957
	-O4 # max optimiszation
	# -g # Add debug symbol
	# -Og # Optimize debugging experience
	# Add -Ofast for compilation ?
)

## Set compile definitions
## no need to add -D in front of the definitions
target_compile_definitions(${PROJECT_NAME}
	PRIVATE
		OPENCV
		CV_MAJOR_VERSION=${OpenCV_VERSION_MAJOR}
)
if (USE_GPU)
	target_compile_definitions(${PROJECT_NAME} PRIVATE GPU)
endif()
if (USE_CUDNN)
	target_compile_definitions(${PROJECT_NAME} PRIVATE CUDNN)
endif()

## Set include directories
target_include_directories(${PROJECT_NAME}
	PRIVATE src
	PUBLIC
		include
		${OpenCV_INCLUDE_DIRS}
)
if (USE_GPU)
    target_include_directories(${PROJECT_NAME}
        PRIVATE ${CUDA_TOOLKIT_ROOT_DIR}/include
    )
endif()
target_include_directories(${PROJECT_NAME} SYSTEM
	PUBLIC ${OpenCV_INCLUDE_DIRS}
)

## link libraries
target_link_libraries(${PROJECT_NAME}
	PUBLIC
		${OpenCV_LIBS}
	PRIVATE
		m
		${CMAKE_THREAD_LIBS_INIT}
)

# only when USE_GPU is ON
if (USE_GPU)
    target_link_libraries(${PROJECT_NAME}
	    PRIVATE
		    ${CUDA_LIBRARIES}
            ${CUDA_CUBLAS_LIBRARIES}
            ${CUDA_curand_LIBRARY}
    )
    if(USE_CUDNN)
    target_link_libraries(${PROJECT_NAME}
	    PRIVATE
		    # only when USE_CUDNN is ON
            ${CUDNN_LIBRARIES}
    )
    endif()
endif()

enable_testing()
add_subdirectory(test)

set_target_properties(${PROJECT_NAME}
	PROPERTIES
	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# install(TARGETS ${PROJECT_NAME}
# 	ARCHIVE DESTINATION lib
# 	LIBRARY DESTINATION lib
# 	RUNTIME DESTINATION bin
# 	COMPONENT runtime
# )

# if(INSTALL_SOURCE_HEADERS)
# 	install( FILES ${headers}
# 		DESTINATION "include/darknet"
# 	)
# endif()

##
# Below is an attempt to look at what was the original Makfile doing
##

# compile C line
# g++ -Iinclude/ -Isrc/ -DOPENCV `pkg-config --cflags opencv`  -DGPU -I/usr/local/cuda/include/ -DCUDNN  -Wno-write-strings -std=c++0x -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC -Ofast -DOPENCV -DGPU -DCUDNN -fPIC -c ./examples/segmenter.c -o obj-cpp-shared/segmenter.o
# compile C flags
	## ??
		# -std=c++0x
		# -fPIC
		# -Ofast

	## definition for Darknet
		# -DOPENCV
		# -DGPU
		# -DCUDNN

	## include dirs
		# -Isrc/
		# -Iinclude/
		# -I/usr/local/cuda/include/
		# `pkg-config --cflags opencv`

	## warnings control
		# -Wall
		# -Wno-write-strings
		# -Wno-unused-result
		# -Wno-unknown-pragmas
		# -Wfatal-errors

# cuda compile line
	# nvcc -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_50,code=[sm_50,compute_50] -gencode arch=compute_52,code=[sm_52,compute_52] -Iinclude/ -Isrc/ -DOPENCV `pkg-config --cflags opencv`  -DGPU -I/usr/local/cuda/include/ -DCUDNN  --compiler-options "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC -Ofast -DOPENCV -DGPU -DCUDNN -fPIC" -c ./src/convolutional_kernels.cu -o obj-cpp-shared/convolutional_kernels.o

# cuda compile flags
	## gencode, cuda related
		# -gencode arch=compute_30,code=sm_30
		# -gencode arch=compute_35,code=sm_35
		# -gencode arch=compute_50,code=[sm_50,compute_50]
		# -gencode arch=compute_52,code=[sm_52,compute_52]

	## Include dirs
		# -Isrc/
		# -Iinclude/
		# `pkg-config --cflags opencv`
		# -I/usr/local/cuda/include/

	## Darknet flags
		# -DOPENCV
		# -DGPU
		# -DCUDNN
	# --compiler-options
	# "-Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC -Ofast -DOPENCV -DGPU -DCUDNN -fPIC"

## link all
# g++ -Iinclude/ -Isrc/ -DOPENCV `pkg-config --cflags opencv`  -DGPU -I/usr/local/cuda/include/ -DCUDNN  -Wall -Wno-unused-result -Wno-unknown-pragmas -Wfatal-errors -fPIC -Ofast -DOPENCV -DGPU -DCUDNN ./obj-cpp-shared/segmenter.o ./obj-cpp-shared/upsample_layer.o ./obj-cpp-shared/l2norm_layer.o ./obj-cpp-shared/logistic_layer.o ./obj-cpp-shared/lstm_layer.o ./obj-cpp-shared/gemm.o ./obj-cpp-shared/utils.o ./obj-cpp-shared/cuda.o ./obj-cpp-shared/deconvolutional_layer.o ./obj-cpp-shared/convolutional_layer.o ./obj-cpp-shared/list.o ./obj-cpp-shared/image.o ./obj-cpp-shared/activations.o ./obj-cpp-shared/im2col.o ./obj-cpp-shared/col2im.o ./obj-cpp-shared/blas.o ./obj-cpp-shared/crop_layer.o ./obj-cpp-shared/dropout_layer.o ./obj-cpp-shared/maxpool_layer.o ./obj-cpp-shared/softmax_layer.o ./obj-cpp-shared/data.o ./obj-cpp-shared/matrix.o ./obj-cpp-shared/network.o ./obj-cpp-shared/connected_layer.o ./obj-cpp-shared/cost_layer.o ./obj-cpp-shared/parser.o ./obj-cpp-shared/option_list.o ./obj-cpp-shared/detection_layer.o ./obj-cpp-shared/captcha.o ./obj-cpp-shared/route_layer.o ./obj-cpp-shared/box.o ./obj-cpp-shared/nightmare.o ./obj-cpp-shared/normalization_layer.o ./obj-cpp-shared/avgpool_layer.o ./obj-cpp-shared/coco.o ./obj-cpp-shared/yolo.o ./obj-cpp-shared/detector.o ./obj-cpp-shared/layer.o ./obj-cpp-shared/regressor.o ./obj-cpp-shared/classifier.o ./obj-cpp-shared/local_layer.o ./obj-cpp-shared/shortcut_layer.o ./obj-cpp-shared/activation_layer.o ./obj-cpp-shared/rnn_layer.o ./obj-cpp-shared/gru_layer.o ./obj-cpp-shared/rnn.o ./obj-cpp-shared/crnn_layer.o ./obj-cpp-shared/demo.o ./obj-cpp-shared/tag.o ./obj-cpp-shared/cifar.o ./obj-cpp-shared/go.o ./obj-cpp-shared/batchnorm_layer.o ./obj-cpp-shared/art.o ./obj-cpp-shared/region_layer.o ./obj-cpp-shared/reorg_layer.o ./obj-cpp-shared/lsd.o ./obj-cpp-shared/super.o ./obj-cpp-shared/tree.o ./obj-cpp-shared/yolo_layer.o ./obj-cpp-shared/convolutional_kernels.o ./obj-cpp-shared/deconvolutional_kernels.o ./obj-cpp-shared/activation_kernels.o ./obj-cpp-shared/im2col_kernels.o ./obj-cpp-shared/col2im_kernels.o ./obj-cpp-shared/blas_kernels.o ./obj-cpp-shared/crop_layer_kernels.o ./obj-cpp-shared/dropout_layer_kernels.o ./obj-cpp-shared/maxpool_layer_kernels.o ./obj-cpp-shared/avgpool_layer_kernels.o -o libdarknet-cpp-shared.so -lm -pthread  `pkg-config --libs opencv`  -L./3rdparty -L/usr/local/cuda/lib64 -lcuda -lcudart -lcublas -lcurand -lcudnn -lstdc++  -shared
#
# linking flags
	## darknet flags
	# -DOPENCV
	# -DGPU
	# -DCUDNN

	## include dirs
		# -Isrc/
		# -Iinclude/
		# -I/usr/local/cuda/include/
		# `pkg-config --libs opencv`
		# `pkg-config --cflags opencv`

	##warning control
		# -Wall
		# -Wno-unused-result
		# -Wno-unknown-pragmas
		# -Wfatal-errors
		# -fPIC
		# -Ofast

	## ??
		# -pthread
		# -lm
		# -L./3rdparty
		# -L/usr/local/cuda/lib64
		# -lcuda
		# -lcudart
		# -lcublas
		# -lcurand
		# -lcudnn
		# -lstdc++
		# -shared
