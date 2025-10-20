cmake_minimum_required(VERSION 3.20)

project (HelloSFML)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set (SRC_PREFIX ${PROJECT_SOURCE_DIR}/Source)

file(GLOB_RECURSE SRC_LIST ${SRC_PREFIX} ${SRC_PREFIX}/*.cpp ${SRC_PREFIX}/*.c ${SRC_PREFIX}/*.h)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	source_group (TREE "${SRC_PREFIX}" PREFIX ${PROJECT_NAME} FILES ${SRC_LIST})
endif ()

add_executable (${PROJECT_NAME}  ${SRC_LIST})


if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	target_link_directories (${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/../../Library/Windows/Audio/SFML/3.0.2/vc17_64/lib)
	target_include_directories (${PROJECT_NAME}	PUBLIC ${SRC_PREFIX} ${PROJECT_SOURCE_DIR}/../../Library/Windows/Audio/SFML/3.0.2/vc17_64/include)
	target_link_libraries (${PROJECT_NAME} sfml-main sfml-system sfml-audio sfml-window sfml-graphics)
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")

endif()