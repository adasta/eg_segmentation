
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(eg_segmentation)
	
#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
link_directories(${CMAKE_BINARY_DIR}/lib)

include_directories( include)

set(CMAKE_ECLIPSE_MAKE_ARGUMENTS -j12)

######################### External Libraries ###########################
	
find_package(OpenCV REQUIRED COMPONENTS highgui core imgproc)
include_directories(${OpenCV_INCLUDE_DIRS})

FIND_PACKAGE( Boost 1.44 COMPONENTS 
					program_options filesystem thread
					serialization python 
					REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )
include_directories( include)

######################### Project Variables ############################
	
set(EG_SEGMENTATION_DEP_LIBRARIES  ${OpenCV_LIBS}
${Boost_LIBRARIES}
)

######################## Executables / Libraries ######################

#add a directory for throw away programs for testing out code
if( IS_DIRECTORY "${CMAKE_SOURCE_DIR}/scratch")
message(STATUS "Adding scratch directory")
add_subdirectory(scratch)
endif()


#Project specific stuff

add_library( eg_segmentation src/eg_segmentation.cpp)
target_link_libraries( eg_segmentation ${EG_SEGMENTATION_DEP_LIBRARIES}) 

add_executable(segment tools/segment.cpp)
target_link_libraries(segment ${EG_SEGMENTATION_DEP_LIBRARIES} eg_segmentation)


#install( FILE gseg_opencv ${CMAKE_INSTALL_DIR}/lib)
