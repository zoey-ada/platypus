# zlib
add_subdirectory(zlib)
# _p__ignore_warnings(zlib)
_p__ignore_warnings(zlibstatic)
_p__link_static(zlibstatic)
# set_target_properties(zlib
# 	PROPERTIES
# 	FOLDER libraries
# )
set_target_properties(zlibstatic
	PROPERTIES
	FOLDER libraries
)

set(ZLIB_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/zlib)
# add_subdirectory(${ZLIB_DIRECTORY})

# Explicitly set the ZLIB_BUILD_DIRECTORY for libpng (?)
# get_directory_property(ZLIB_BUILD_DIRECTORY DIRECTORY ${ZLIB_DIRECTORY} DEFINITION CMAKE_CURRENT_BINARY_DIR)

# option(PNG_LINK_ZLIB_STATIC "Use a static zlib library for libpng builds" ON)

# get_directory_property(zlib DIRECTORY ${ZLIB_DIRECTORY} DEFINITION zlib)
get_directory_property(zlibstatic DIRECTORY ${ZLIB_DIRECTORY} DEFINITION zlibstatic)

# if (PNG_LINK_ZLIB_STATIC)
	add_library(ZLIB::ZLIB ALIAS zlibstatic)
	set(ZLIB_LIBRARY zlibstatic)
# else()
# 	add_library(ZLIB::ZLIB ALIAS zlib)
# 	set(ZLIB_LIBRARY zlib)
# endif()

set(ZLIB_INCLUDE_DIR
	${ZLIB_DIRECTORY}
	${CMAKE_CURRENT_BINARY_DIR}/zlib/
)

# freetype
add_subdirectory(freetype)
_p__ignore_warnings(freetype)
_p__link_static(freetype)
set_target_properties(freetype
	PROPERTIES
	FOLDER libraries
)

target_include_directories(freetype
	PRIVATE
		${ZLIB_INCLUDE_DIR}
)

# libzip
add_subdirectory(libzip)
_p__ignore_warnings(zip)
_p__link_static(zip)
set_target_properties(zip
	PROPERTIES
	FOLDER libraries
)

target_include_directories(zip
	PRIVATE
		${ZLIB_INCLUDE_DIR}
)

# platypus-proto
add_subdirectory("platypus-proto")

# utf8-cpp
add_subdirectory("utf8-cpp")
_p__ignore_warnings(utf8cpp)
_p__link_static(utf8cpp)
target_compile_definitions(utf8cpp
	INTERFACE
		UTF_CPP_CPLUSPLUS=202002L
)
set_target_properties(utf8cpp
	PROPERTIES
	FOLDER libraries
)
