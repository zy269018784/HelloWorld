cmake_minimum_required(VERSION 3.20)

project (HelloFFmpeg)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SRC_PREFIX ${PROJECT_SOURCE_DIR}/Source)

file(GLOB_RECURSE SRC_LIST ${SRC_PREFIX} ${SRC_PREFIX}/*.cpp ${SRC_PREFIX}/*.c ${SRC_PREFIX}/*.h)

#if (MSVC)
#	source_group (TREE "${SRC_PREFIX}" PREFIX ${PROJECT_NAME} FILES ${SRC_LIST})
#endif ()



if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	source_group (TREE "${SRC_PREFIX}" PREFIX ${PROJECT_NAME} FILES ${SRC_LIST})
endif ()

add_executable(${PROJECT_NAME}  ${SRC_LIST})

target_include_directories (${PROJECT_NAME}	PUBLIC ${SRC_PREFIX})

target_include_directories (${PROJECT_NAME}	PUBLIC	${PROJECT_SOURCE_DIR}/../../Library/Windows/Codec/ffmpeg/7.0/vc143/include)
target_link_directories    (${PROJECT_NAME} PUBLIC	${PROJECT_SOURCE_DIR}/../../Library/Windows/Codec/ffmpeg/7.0/vc143/lib)
	
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	list (APPEND LIBRARY avcodec avdevice avfilter avformat avutil postproc swresample swscale)
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")

endif()
target_link_libraries (${PROJECT_NAME}	PUBLIC ${LIBRARY})