cmake_minimum_required(VERSION 3.20)

project (LearnBoost)

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

if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	target_include_directories(${PROJECT_NAME}		PUBLIC ${SRC_PREFIX})
	target_include_directories(${PROJECT_NAME}			PUBLIC ${PROJECT_SOURCE_DIR}/../../Library/Windows/Boost/1.85/msvc2022_64/include)
	target_link_directories(${PROJECT_NAME}			PUBLIC ${PROJECT_SOURCE_DIR}/../../Library/Windows/Boost/1.85/msvc2022_64/lib)
	target_link_libraries(${PROJECT_NAME} libboost_system-vc143-mt-x64-1_85)
	target_link_libraries(${PROJECT_NAME} libboost_json-vc143-mt-x64-1_85)
	target_link_libraries(${PROJECT_NAME} libboost_exception-vc143-mt-x64-1_85)
elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	target_include_directories(${PROJECT_NAME}  PUBLIC ${SRC_PREFIX})
	target_link_libraries(${PROJECT_NAME} boost_system)
	target_link_libraries(${PROJECT_NAME} boost_json)
    target_link_libraries(${PROJECT_NAME} boost_filesystem)
    target_link_libraries(${PROJECT_NAME} boost_thread)
	target_link_libraries(${PROJECT_NAME} boost_exception)
endif()