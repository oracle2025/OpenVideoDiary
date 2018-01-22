project(OpenVideoDiary)
cmake_minimum_required(VERSION 3.9)

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5MultimediaWidgets)
find_package(Qt5Sql)

#set(CMAKE_AUTOMOC ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Core_INCLUDE_DIRS}
	${Qt5MultimediaWidgets_INCLUDE_DIRS}
	${Qt5Sql_INCLUDE_DIRS}
	)

if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
	set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-deprecated")
endif()


add_subdirectory(src)
