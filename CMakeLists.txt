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
	application_layer/utils.hpp
	application_layer/windowsUtils.cpp
)
source_group(source\\application_layer FILES ${application_srcs})

set(application_filesys_srcs
	application_layer/file_system/iFileSystem.hpp
	application_layer/file_system/standardFileSystem.hpp
	application_layer/file_system/standardFileSystem.cpp
)
source_group(source\\application_layer\\file_system FILES ${application_filesys_srcs})

set(application_window_srcs
	application_layer/window/iWindow.hpp
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

set(exceptions_srcs
	exceptions/creationException.hpp
	exceptions/platypusException.hpp
)
source_group(source\\exceptions FILES ${exceptions_srcs})

set(libraries_srcs
	libraries/libzip_include.hpp
)
source_group(source\\libraries FILES ${libraries_srcs})
set(renderer_srcs
	renderer/color.hpp
	renderer/color.cpp
	renderer/iRenderer.hpp
	renderer/rendererFactory.hpp
	renderer/rendererFactory.cpp
	renderer/scene.hpp
	renderer/scene.cpp
	renderer/screenElementScene.hpp
)
source_group(source\\renderer FILES ${renderer_srcs})

if(WIN32)
	set(directx_srcs
		renderer/directx/directXObjectCreator.hpp
		renderer/directx/directXObjectCreator.cpp
		renderer/directx/directXRenderer.hpp
		renderer/directx/directXRenderer.cpp
		renderer/directx/directXShaderLoader.hpp
		renderer/directx/directXShaderLoader.cpp
	)
endif()
source_group(source\\renderer\\directx FILES ${directx_srcs})

set(resource_cache_srcs
	resource_cache/resourceCache.hpp
	resource_cache/resourceCache.cpp
)
source_group(source\\resource_cache FILES ${resource_cache_srcs})

set(resource_cache_loaders_srcs
	resource_cache/loaders/directXMeshLoader.hpp
	resource_cache/loaders/directXMeshLoader.cpp
	resource_cache/loaders/directXPixelShaderLoader.hpp
	resource_cache/loaders/directXPixelShaderLoader.cpp
	resource_cache/loaders/directXTextureLoader.hpp
	resource_cache/loaders/directXTextureLoader.cpp
	resource_cache/loaders/directXVertexShaderLoader.hpp
	resource_cache/loaders/directXVertexShaderLoader.cpp
	resource_cache/loaders/iResourceLoader.hpp
)
source_group(source\\resource_cache\\loaders FILES ${resource_cache_loaders_srcs})

set(resource_cache_resources_srcs
	resource_cache/resources/meshResource.hpp
	resource_cache/resources/meshResource.cpp
	resource_cache/resources/pixelShaderResource.hpp
	resource_cache/resources/pixelShaderResource.cpp
	resource_cache/resources/resource.hpp
	resource_cache/resources/resource.cpp
	resource_cache/resources/resourceType.hpp
	resource_cache/resources/resourceType.cpp
	resource_cache/resources/textureResource.hpp
	resource_cache/resources/textureResource.cpp
	resource_cache/resources/vertexShaderResource.hpp
	resource_cache/resources/vertexShaderResource.cpp
)
source_group(source\\resource_cache\\resources FILES ${resource_cache_resources_srcs})

set(resource_cache_stores_srcs
	resource_cache/stores/iResourceStore.hpp
	resource_cache/stores/zipResourceStore.hpp
	resource_cache/stores/zipResourceStore.cpp
)
source_group(source\\resource_cache\\stores FILES ${resource_cache_stores_srcs})

set(scene_node_srcs
	renderer/scene_nodes/iSceneNode.hpp
	renderer/scene_nodes/renderPass.hpp
	renderer/scene_nodes/rootNode.hpp
	renderer/scene_nodes/rootNode.cpp
	renderer/scene_nodes/sceneNode.hpp
	renderer/scene_nodes/sceneNode.cpp
	renderer/scene_nodes/sceneNodeProperties.hpp
)
source_group(source\\scene_nodes FILES ${scene_node_srcs})

set(utilities_srcs
	utilities/encoding.hpp
	utilities/encoding.cpp
	utilities/safeDelete.hpp
	utilities/time.hpp
	utilities/time.cpp
	utilities/wildcardMatch.hpp
	utilities/wildcardMatch.cpp
)
source_group(source\\utilities FILES ${utilities_srcs})

set(utilities_logging_srcs
	utilities/logging/consoleFormat.hpp
	utilities/logging/consoleFormat.cpp
	utilities/logging/consoleLogger.hpp
	utilities/logging/consoleLogger.cpp
	utilities/logging/fileLogger.hpp
	utilities/logging/fileLogger.cpp
	utilities/logging/iLogger.hpp
	utilities/logging/logger.hpp
	utilities/logging/logger.cpp
	utilities/logging/verbosityDecorator.hpp
	utilities/logging/verbosityDecorator.cpp
)
source_group(source\\utilities\\logging FILES ${utilities_logging_srcs})

set(views_srcs
	views/humanView.hpp
	views/humanView.cpp
	views/iScreenElement.hpp
	views/iView.hpp
)
source_group(source\\views FILES ${views_srcs})

set(all_srcs
	${main_srcs}
	${application_srcs}
	${application_filesys_srcs}
	${application_window_srcs}
	${entities_srcs}
	${exceptions_srcs}
	${libraries_srcs}
	${renderer_srcs}
	${scene_node_srcs}
	${directx_srcs}
	${resource_cache_srcs}
	${resource_cache_loaders_srcs}
	${resource_cache_resources_srcs}
	${resource_cache_stores_srcs}
	${utilities_srcs}
	${utilities_logging_srcs}
	${views_srcs}
)

add_library(platypus STATIC ${all_srcs})
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
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/libraries/zlib
		${CMAKE_CURRENT_BINARY_DIR}/libraries/zlib
)

target_link_libraries(platypus
	PUBLIC
		platypus-proto
	PRIVATE
		assimp
		freetype
		utf8cpp
		zip
		zlibstatic
)

if(WIN32)
	target_link_libraries(platypus
		PRIVATE
			d3d11.lib
			d3dcompiler.lib
			windowscodecs.lib
	)
endif()
