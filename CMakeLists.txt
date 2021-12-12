cmake_minimum_required(VERSION 3.18)

project(
	Platypus
	VERSION 0.1
	DESCRIPTION "Platypus game engine."
	LANGUAGES CXX
)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	include(cmake/config.cmake)

	# # Testing only available if this is the main app
	# # Note this needs to be done in the main CMakeLists
	# # since it calls enable_testing, which must be in the
	# # main CMakeLists.
	# include(CTest)
	# add_subdirectory(tests)

	# # Docs only available if this is the main app
	# find_package(Doxygen)
	# if(Doxygen_FOUND)
	# 	add_subdirectory(docs)
	# else()
	# 	message(STATUS "Doxygen not found, not building docs")
	# endif()

endif()

include(cmake/funcs.cmake)
add_subdirectory(libraries)

# FetchContent -> downloads content durning config step

if((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME OR MODERN_CMAKE_BUILD_TESTING)
	AND BUILD_TESTING)
	# add_subdirectory(tests)
endif()

set(main_srcs
	baseGameLogic.hpp
	baseGameLogic.cpp
	platypus.hpp
	platypus.cpp
)
source_group(source FILES ${main_srcs})

set(application_srcs
	application_layer/platform.hpp
	application_layer/platform.cpp
)
source_group(source\\application_layer FILES ${application_srcs})

set(application_filesys_srcs
	application_layer/file_system/IFileSystem.hpp
	application_layer/file_system/standardFileSystem.hpp
	application_layer/file_system/standardFileSystem.cpp
)
source_group(source\\application_layer\\file_system FILES ${application_filesys_srcs})

set(application_window_srcs
	application_layer/window/IWindow.hpp
	application_layer/window/windowFactory.hpp
	application_layer/window/windowFactory.cpp
	application_layer/window/windowsWindow.hpp
	application_layer/window/windowsWindow.cpp
)
source_group(source\\application_layer\\window FILES ${application_window_srcs})

set(entities_srcs
	entities/entity.hpp
	entities/entity.cpp
)
source_group(source\\entities FILES ${entities_srcs})

set(renderer_srcs
	renderer/color.hpp
	renderer/color.cpp
	renderer/IRenderer.hpp
	renderer/rendererFactory.hpp
	renderer/rendererFactory.cpp
)

if(WIN32)
	set(renderer_srcs
		${renderer_srcs}
		renderer/directXRenderer.hpp
		renderer/directXrenderer.cpp
	)
endif()
source_group(source\\renderer FILES ${renderer_srcs})

set(utilities_srcs
	utilities/encoding.hpp
	utilities/encoding.cpp
	utilities/time.hpp
	utilities/time.cpp
)
source_group(source\\utilities FILES ${utilities_srcs})

set(utilities_logging_srcs
	utilities/logging/consoleFormat.hpp
	utilities/logging/consoleFormat.cpp
	utilities/logging/consoleLogger.hpp
	utilities/logging/consoleLogger.cpp
	utilities/logging/fileLogger.hpp
	utilities/logging/fileLogger.cpp
	utilities/logging/ILogger.hpp
	utilities/logging/logger.hpp
	utilities/logging/logger.cpp
	utilities/logging/verbosityDecorator.hpp
	utilities/logging/verbosityDecorator.cpp
)
source_group(source\\utilities\\logging FILES ${utilities_logging_srcs})

set(views_srcs
	views/humanView.hpp
	views/humanView.cpp
	views/IScreenElement.hpp
	views/IView.hpp
)
source_group(source\\views FILES ${views_srcs})

set(all_srcs
	${main_srcs}
	${application_srcs}
	${application_filesys_srcs}
	${application_window_srcs}
	${entities_srcs}
	${renderer_srcs}
	${utilities_srcs}
	${utilities_logging_srcs}
	${views_srcs}
)

add_library(platypus ${all_srcs})
_p__warnings_as_errors(platypus)
_p__link_static(platypus)

target_compile_definitions(platypus
	PUBLIC
		ENABLE_WINDOWS_CONSOLE=0
		_CRT_SECURE_NO_WARNINGS
)

target_include_directories(platypus
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(platypus
	PUBLIC
		platypus-proto
	PRIVATE
		freetype
		utf8cpp
)

if(WIN32)
	target_link_libraries(platypus
		PRIVATE
			d3d11.lib
	)
endif()
