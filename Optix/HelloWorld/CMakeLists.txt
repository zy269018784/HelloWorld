cmake_minimum_required(VERSION 3.20)
set (TARGET_NAME HelloWorld)
project (${TARGET_NAME})

# c++
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
#set (CMAKE_CUDA_STANDARD 17)
# 方法1: nvcc -arch=sm_61
set(CMAKE_CUDA_ARCHITECTURES "61") 

# cuda
#enable_language (CUDA)

message (STATUS "CMAKE_CUDA_STANDARD ${CMAKE_CUDA_STANDARD}")

#find_package(CUDA REQUIRED)
set (CMAKE_PREFIX_PATH /usr/local/NVIDIA-OptiX-SDK-7.6.0-linux64-x86_64/SDK/cmake)
#set (OptiX_DIR 		   "/usr/local/NVIDIA-OptiX-SDK-7.6.0-linux64-x86_64/cmake")
set (OptiX_INSTALL_DIR "/usr/local/NVIDIA-OptiX-SDK-7.6.0-linux64-x86_64/SDK")
find_package(OptiX REQUIRED)

set (SRC_PREFIX ${PROJECT_SOURCE_DIR}/Source)
file(GLOB_RECURSE SRC_LIST ${SRC_PREFIX} ${SRC_PREFIX}/*.cpp)

#set_source_files_properties (${SRC_PREFIX}/HelloWorld.cpp PROPERTIES LANGUAGE CUDA)

add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE
    OptiX::OptiX
    CUDA::cudart
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${OptiX_INCLUDE_DIR}
)

# 方法2: nvcc -arch=sm_61
#set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_ARCHITECTURES "61") 	

# cuda分离编译
#set_property(TARGET ${PROJECT_NAME} PROPERTY CUDA_SEPARABLE_COMPILATION ON)
