cmake_minimum_required( VERSION 3.15 )

project (HelloD3D11)
set (CMAKE_CXX_STANDARD  17)
if(WIN32)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/source-charset:utf-8>")
endif()

file(GLOB_RECURSE SOURCE_FILES		"Source/*.h" "Source/*.cpp" "Source/*.c")

source_group(TREE "Source" PREFIX "Source" FILES ${SOURCE_FILES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ../../../Library/include ../../../Library/include/directx)
target_link_directories(${PROJECT_NAME} PUBLIC ../../../Library/lib)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw3.lib)

