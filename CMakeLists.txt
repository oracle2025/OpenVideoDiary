project(OpenVideoDiary)
cmake_minimum_required(VERSION 3.9)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0058 OLD)

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

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "OpenVideoDiary")
set(CPACK_PACKAGE_VENDOR "bunfet.me")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Video Diary Application")
set(CPACK_PACKAGE_VERSION "0.0.1")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "1")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "OpenVideoDiary")
SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
SET(CPACK_NSIS_MODIFY_PATH OFF)
SET(CPACK_PACKAGE_EXECUTABLES "OpenVideoDiary;OpenVideoDiary")

INCLUDE(CPack)

