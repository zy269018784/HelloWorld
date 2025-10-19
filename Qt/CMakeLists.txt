cmake_minimum_required(VERSION 3.20)

project (HelloQt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)



if (USE_QT6)
	#set (CMAKE_PREFIX_PATH G:/Project/BEngine/Library/Windows/GUI/Qt/5.9.2/msvc2017_64/lib/cmake)
	find_package(Qt6 COMPONENTS Widgets Qml Quick QuickControls2 QuickWidgets REQUIRED)
	set (QT_LIBS Qt6::Widgets
				 Qt6::QuickWidgets
				 Qt6::Quick
				 Qt6::Qml
				 Qt6::QuickControls2)
else ()
	#set (CMAKE_PREFIX_PATH G:/Project/BEngine/Library/Windows/GUI/Qt/5.9.2/msvc2017_64/lib/cmake)
	find_package(Qt5 COMPONENTS Widgets Qml Quick QuickControls2 QuickWidgets REQUIRED)
	set (QT_LIBS Qt5::Widgets
				 Qt5::QuickWidgets
				 Qt5::Quick
				 Qt5::Qml
				 Qt5::QuickControls2)
endif ()

set (SRC_PREFIX ${PROJECT_SOURCE_DIR}/Source)

file(GLOB_RECURSE SRC_LIST ${SRC_PREFIX} ${SRC_PREFIX}/*.cpp ${SRC_PREFIX}/*.c ${SRC_PREFIX}/*.h)


if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	source_group (TREE "${SRC_PREFIX}" PREFIX ${PROJECT_NAME} FILES ${SRC_LIST})
endif ()

add_executable(${PROJECT_NAME}  ${SRC_LIST})

target_include_directories(${PROJECT_NAME}	PUBLIC ${SRC_PREFIX})
target_link_libraries(${PROJECT_NAME} ${QT_LIBS})
	
if (CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

elseif (CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")

endif()