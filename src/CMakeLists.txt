file(GLOB_RECURSE Sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE Headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h)

add_executable(OpenVideoDiary ${Sources} ${Headers})

target_link_libraries(OpenVideoDiary
	Qt5::Core
	Qt5::Widgets
	Qt5::MultimediaWidgets
	Qt5::Sql
	)
