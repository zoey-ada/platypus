set(project platypus)
project(${project})

include_directories(
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(main_srcs
	platypus.h
	platypus.cpp
	settings.h
	settings.cpp
)
source_group(source FILES ${main_srcs})

set(application_srcs
	application_layer/platform.h
)
source_group(source\\application_layer FILES ${application_srcs})

set(application_window_srcs
	application_layer/window/IWindow.h
	application_layer/window/windowFactory.h
	application_layer/window/windowFactory.cpp
	application_layer/window/windowsWindow.h
	application_layer/window/windowsWindow.cpp
)
source_group(source\\application_layer\\window FILES ${application_window_srcs})

set(renderer_srcs
	renderer/color.h
	renderer/color.cpp
	renderer/IRenderer.h
	renderer/rendererFactory.h
	renderer/rendererFactory.cpp
)

if(WIN32)
	set(renderer_srcs
		${renderer_srcs}
		renderer/directXRenderer.h
		renderer/directXrenderer.cpp
	)
endif()
source_group(source\\renderer FILES ${renderer_srcs})

set(utilities_srcs
	utilities/encoding.h
	utilities/encoding.cpp
	utilities/time.h
	utilities/time.cpp
)
source_group(source\\utilities FILES ${utilities_srcs})

set(all_srcs
	${main_srcs}
	${application_srcs}
	${application_window_srcs}
	${renderer_srcs}
	${utilities_srcs}
)

add_library(${project} ${all_srcs})

set(link_libs
	#platypus-proto
)

if(WIN32)
	set(link_libs
		${link_libs}
		d3d11.lib
	)
endif()

target_link_libraries(${project}
	${link_libs}
)
