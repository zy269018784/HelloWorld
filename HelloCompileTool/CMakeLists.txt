cmake_minimum_required(VERSION 3.20)

project (HelloCompileTool)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 设置 UTF-8 编码（跨平台兼容）
if (MSVC)
    add_compile_options("/utf-8")
else()
    add_compile_options("-finput-charset=utf-8")
endif()

message (STATUS "CMAKE_GENERATOR ${CMAKE_GENERATOR}")
set (SRC_PREFIX ${PROJECT_SOURCE_DIR}/Source)

file(GLOB_RECURSE SRC_LIST ${SRC_PREFIX} ${SRC_PREFIX}/*.cpp ${SRC_PREFIX}/*.c ${SRC_PREFIX}/*.h)

#if (MSVC)
#	source_group (TREE "${SRC_PREFIX}" PREFIX ${PROJECT_NAME} FILES ${SRC_LIST})
#endif ()



if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	source_group (TREE "${SRC_PREFIX}" PREFIX ${PROJECT_NAME} FILES ${SRC_LIST})
endif ()

add_executable(${PROJECT_NAME}  ${SRC_LIST})

target_include_directories(${PROJECT_NAME}	PUBLIC ${SRC_PREFIX})
	
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")

endif()