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
	serviceProvider.hpp
)
source_group(source FILES ${main_srcs})

set(platform_srcs
	platform/iPlatform.hpp
	platform/windowsPlatform.hpp
	platform/windowsPlatform.cpp
	platform/platformFactory.hpp
	platform/platformFactory.cpp
	platform/platform.hpp
	platform/platform.cpp
	platform/utils.hpp
	platform/windowsUtils.cpp
)
source_group(source\\platform FILES ${platform_srcs})

set(platform_filesys_srcs
	platform/file_system/iFileSystem.hpp
	platform/file_system/standardFileSystem.hpp
	platform/file_system/standardFileSystem.cpp
)
source_group(source\\platform\\file_system FILES ${platform_filesys_srcs})

set(platform_window_srcs
	platform/window/iWindow.hpp
	platform/window/windowsWindow.hpp
	platform/window/windowsWindow.cpp
)
source_group(source\\platform\\window FILES ${platform_window_srcs})

set(entities_srcs
	entities/entity.hpp
	entities/entity.cpp
	entities/entityFactory.hpp
	entities/entityFactory.cpp
)
source_group(source\\entities FILES ${entities_srcs})

set(entities_components_srcs
	entities/components/entityComponent.hpp
	entities/components/movementComponent2d.hpp
	entities/components/movementComponent2d.cpp
	entities/components/renderComponent2d.hpp
	entities/components/renderComponent2d.cpp
	entities/components/renderComponent3d.hpp
	entities/components/renderComponent3d.cpp
	entities/components/transformComponent2d.hpp
	entities/components/transformComponent2d.cpp
	entities/components/transformComponent3d.hpp
	entities/components/transformComponent3d.cpp
)
source_group(source\\entities\\components FILES ${entities_components_srcs})

set(events_srcs
	events/eventManager.hpp
	events/eventManager.cpp
	events/iEventManager.hpp
)
source_group(source\\events FILES ${events_srcs})

set(events_events_src
	events/events/newRenderComponentEvent.hpp
)
source_group(source\\events\\events FILES ${events_events_src})

set(exceptions_srcs
	exceptions/creationException.hpp
	exceptions/platypusException.hpp
)
source_group(source\\exceptions FILES ${exceptions_srcs})

set(input_src
	input/command.hpp
	input/input.hpp
	input/inputManager.hpp
	input/inputManager.cpp
	input/inputType.hpp
)
source_group(source\\input FILES ${input_src})

set(input_devices_srcs
	input/devices/iInputDevice.hpp
)
source_group(source\\input\\devices FILES ${input_devices_srcs})

set(input_devices_raw_input_srcs
	input/devices/raw_input/iRawInputDevice.hpp
	input/devices/raw_input/rawInputKeyboardDevice.hpp
	input/devices/raw_input/rawInputKeyboardDevice.cpp
	input/devices/raw_input/windowsHidMappings.hpp
)
source_group(source\\input\\devices\\raw_input FILES ${input_devices_raw_input_srcs})

set(libraries_srcs
	libraries/libzip_include.hpp
)
source_group(source\\libraries FILES ${libraries_srcs})

set(renderer_srcs
	renderer/color.hpp
	renderer/color.cpp
	renderer/graphics.hpp
	renderer/graphics.cpp
	renderer/iPixelShader.hpp
	renderer/iRenderer.hpp
	renderer/iVertexShader.hpp
	renderer/material.hpp
	renderer/rendererFactory.hpp
	renderer/rendererFactory.cpp
	renderer/iRendererState.hpp
	renderer/scene.hpp
	renderer/scene.cpp
	renderer/screenElementScene.hpp
	renderer/screenElementScene.cpp
	renderer/textRenderer.hpp
	renderer/textRenderer.cpp
)
source_group(source\\renderer FILES ${renderer_srcs})

if(WIN32)
	set(directx_srcs
		renderer/directx/directXAlphaPass.hpp
		renderer/directx/directXAlphaPass.cpp
		renderer/directx/directXObjectCreator.hpp
		renderer/directx/directXObjectCreator.cpp
		renderer/directx/directXPixelShader.hpp
		renderer/directx/directXPixelShader.cpp
		renderer/directx/directXRenderer.hpp
		renderer/directx/directXRenderer.cpp
		renderer/directx/directXShaderLoader.hpp
		renderer/directx/directXShaderLoader.cpp
		renderer/directx/directXVertexShader.hpp
		renderer/directx/directXVertexShader.cpp
	)
	source_group(source\\renderer\\directx FILES ${directx_srcs})
endif()

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
	renderer/scene_nodes/alphaSceneNode.hpp
	renderer/scene_nodes/cameraNode.hpp
	renderer/scene_nodes/cameraNode.cpp
	renderer/scene_nodes/iSceneNode.hpp
	renderer/scene_nodes/meshNode.hpp
	renderer/scene_nodes/meshNode.cpp
	renderer/scene_nodes/renderPass.hpp
	renderer/scene_nodes/rectangleNode.hpp
	renderer/scene_nodes/rectangleNode.cpp
	renderer/scene_nodes/rootNode.hpp
	renderer/scene_nodes/rootNode.cpp
	renderer/scene_nodes/sceneNode.hpp
	renderer/scene_nodes/sceneNode.cpp
	renderer/scene_nodes/textNode.hpp
	renderer/scene_nodes/textNode.cpp
	renderer/scene_nodes/sceneNodeProperties.hpp
)
source_group(source\\scene_nodes FILES ${scene_node_srcs})

set(utilities_srcs
	utilities/encoding.hpp
	utilities/encoding.cpp
	utilities/safeDelete.hpp
	utilities/wildcardMatch.hpp
	utilities/wildcardMatch.cpp
)
source_group(source\\utilities FILES ${utilities_srcs})

set(utilities_time_srcs
	utilities/time/iClock.hpp
	utilities/time/systemClock.hpp
	utilities/time/systemClock.cpp
	utilities/time/utils.hpp
	utilities/time/utils.cpp
)
source_group(source\\utilities\\time FILES ${utilities_time_srcs})

set(utilities_common_srcs
	utilities/common/ptExtent.hpp
)
source_group(source\\utilities\\common FILES ${utilities_common_srcs})

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

set(utilities_math_srcs
	utilities/math/mathTypes.hpp
	utilities/math/directXMathTypes.hpp
	utilities/math/directXMathTypes.cpp
	utilities/math/utils.hpp
	utilities/math/utils.cpp
)
source_group(source\\utilities\\math FILES ${utilities_math_srcs})

set(views_srcs
	views/humanView.hpp
	views/humanView.cpp
	views/iScreenElement.hpp
	views/iView.hpp
)
source_group(source\\views FILES ${views_srcs})

set(all_srcs
	${main_srcs}
	${platform_srcs}
	${platform_filesys_srcs}
	${platform_window_srcs}
	${entities_srcs}
	${entities_components_srcs}
	${events_srcs}
	${events_events_srcs}
	${exceptions_srcs}
	${input_src}
	${input_devices_srcs}
	${input_devices_raw_input_srcs}
	${libraries_srcs}
	${renderer_srcs}
	${scene_node_srcs}
	${directx_srcs}
	${resource_cache_srcs}
	${resource_cache_loaders_srcs}
	${resource_cache_resources_srcs}
	${resource_cache_stores_srcs}
	${utilities_srcs}
	${utilities_time_srcs}
	${utilities_common_srcs}
	${utilities_logging_srcs}
	${utilities_math_srcs}
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

add_subdirectory(tests)
