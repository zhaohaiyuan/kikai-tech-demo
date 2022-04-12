cmake_minimum_required (VERSION 2.8.8)

project(DemoProj)

# if(NOT "${CMAKE_GENERATOR}" MATCHES "Visual Studio 14 Win64")
	# message(FATAL_ERROR "DemoProj only support Visual Studio 2015 Win64.")
# endif()

set(PROJ_DIR ${CMAKE_SOURCE_DIR}/DemoProj)
set(DEPENDENCY_DIR ${PROJ_DIR}/dependencies)
set(OPENCV_DIR ${DEPENDENCY_DIR}/opencv)
set(OPENVINO_DIR ${DEPENDENCY_DIR}/openvino/v2021.2.185)
set(SOURCE_DIR ${PROJ_DIR}/src)
set(MODEL_DIR ${PROJ_DIR}/models)

file(GLOB MODEL_SRC_FILES
	${MODEL_DIR}/*.cpp
	${MODEL_DIR}/*.h
	)
	
file(GLOB SOURCE_FILES
	${SOURCE_DIR}/*.cpp
	)
	
file(GLOB HEADER_FILES
	${SOURCE_DIR}/*.h
	)

source_group("Header Files" FILES "${HEADER_FILES}")
source_group("Source Files" FILES "${SOURCE_FILES}")
source_group("Models" FILES "${MODEL_SRC_FILES}")

include_directories(${PROJ_DIR}/src)
include_directories(${PROJ_DIR}/cfg)
include_directories(${PROJ_DIR}/models)
include_directories(${OPENCV_DIR}/include)
include_directories(${OPENVINO_DIR}/include)
include_directories(${DEPENDENCY_DIR}/json/include)

link_directories(${OPENCV_DIR}/x64/vc14/lib)
link_directories(${OPENVINO_DIR}/lib)

set(PROJ_SOURCE_FILES
	${SOURCE_FILES}
	${HEADER_FILES}
	${MODEL_SRC_FILES}
	${PROJ_DIR}/cfg/config.h
	)

include(${DEPENDENCY_DIR}/deps.cmake)

set(PROJ_LINK_LIBS
	${CV_LINK_LIBS}
	${OV_LINK_LIBS}
	#${MODEL_SRC_FILES}
    )
	
add_executable(DemoProj ${PROJ_SOURCE_FILES})

target_link_libraries(DemoProj ${PROJ_LINK_LIBS})

set_target_properties(DemoProj
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/"
)

set(DESTINATION_DIRS 
	${CMAKE_BINARY_DIR} 
	$<TARGET_FILE_DIR:DemoProj>
	)

foreach(dir ${DESTINATION_DIRS})
	add_custom_command(TARGET DemoProj POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   "${PROJ_DIR}/data/app_config.json"
                   "${dir}"
				   )
	add_custom_command(TARGET DemoProj POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                   "${PROJ_DIR}/data/test_video_001.mp4"
                   "${dir}"
				   )
	add_custom_command(TARGET DemoProj POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "${OPENCV_DIR}/x64/vc14/bin/$<CONFIGURATION>"
                   "${dir}"
				   )
	add_custom_command(TARGET DemoProj POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "${OPENVINO_DIR}/bin/$<CONFIGURATION>"
                   "${dir}"
				   )
endforeach()