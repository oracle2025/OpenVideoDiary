file(GLOB_RECURSE Sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE Headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

add_executable(OpenVideoDiary ${Sources} ${Headers})

target_link_libraries(OpenVideoDiary
	Qt5::Core
	Qt5::Widgets
	Qt5::MultimediaWidgets
	Qt5::Sql
	)
if (WIN32)
	install(TARGETS OpenVideoDiary
		RUNTIME
		DESTINATION .
		COMPONENT applications)
else()
	install(TARGETS OpenVideoDiary
		RUNTIME
		DESTINATION bin
		COMPONENT applications
		RENAME openvideodiary)
endif()

if (WIN32)
	include(WinDeployQt)
	WinDeployQt(TARGET OpenVideoDiary COMPILER_RUNTIME INCLUDE_MODULES ${QTLIBS} EXCLUDE_MODULES webkit webkit2)
	install(DIRECTORY ${PROJECT_BINARY_DIR}/windeployqt/
		DESTINATION .)
endif()

