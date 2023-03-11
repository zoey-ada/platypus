#[=============================================================================[
  ZLIB_INCLUDE_DIRS   - where to find zlib.h, etc.
  ZLIB_LIBRARIES      - List of libraries when using zlib.
  ZLIB_FOUND          - True if zlib found.

::

  ZLIB_VERSION_STRING - The version of zlib found (x.y.z)
  ZLIB_VERSION_MAJOR  - The major version of zlib
  ZLIB_VERSION_MINOR  - The minor version of zlib
  ZLIB_VERSION_PATCH  - The patch version of zlib
  ZLIB_VERSION_TWEAK  - The tweak version of zlib
#]=============================================================================]

set(ZLIB_FOUND true)

get_target_property(ZLIB_SOURCE_DIR zlib SOURCE_DIR)
get_target_property(ZLIB_BUILD_DIR zlib BINARY_DIR)

set(ZLIB_INCLUDE_DIR
	${ZLIB_SOURCE_DIR}
	${ZLIB_BUILD_DIR}
)

get_target_property(ZLIB_BIN_DIR zlib LIBRARY_OUTPUT_DIRECTORY)
set(ZLIB_LIBRARY_RELEASE ${ZLIB_BIN_DIR}/zlib.lib)
set(ZLIB_LIBRARY_DEBUG ${ZLIB_BIN_DIR}/zlibd.lib)

if(ZLIB_INCLUDE_DIR AND EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h")
	file(STRINGS "${ZLIB_INCLUDE_DIR}/zlib.h" ZLIB_H REGEX "^#define ZLIB_VERSION \"[^\"]*\"$")

	string(REGEX REPLACE "^.*ZLIB_VERSION \"([0-9]+).*$" "\\1" ZLIB_VERSION_MAJOR "${ZLIB_H}")
	string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_MINOR "${ZLIB_H}")
	string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_PATCH "${ZLIB_H}")
	set(ZLIB_VERSION_STRING "${ZLIB_VERSION_MAJOR}.${ZLIB_VERSION_MINOR}.${ZLIB_VERSION_PATCH}")

	# only append a TWEAK version if it exists:
	set(ZLIB_VERSION_TWEAK "")

	if("${ZLIB_H}" MATCHES "ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.[0-9]+\\.([0-9]+)")
		set(ZLIB_VERSION_TWEAK "${CMAKE_MATCH_1}")
		string(APPEND ZLIB_VERSION_STRING ".${ZLIB_VERSION_TWEAK}")
	endif()

	set(ZLIB_MAJOR_VERSION "${ZLIB_VERSION_MAJOR}")
	set(ZLIB_MINOR_VERSION "${ZLIB_VERSION_MINOR}")
	set(ZLIB_PATCH_VERSION "${ZLIB_VERSION_PATCH}")
endif()


if(ZLIB_FOUND)
	set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})

	if(NOT ZLIB_LIBRARIES)
		set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
	endif()

	if(NOT TARGET ZLIB::ZLIB)
		add_library(ZLIB::ZLIB UNKNOWN IMPORTED)
		set_target_properties(ZLIB::ZLIB PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIRS}")

		if(ZLIB_LIBRARY_RELEASE)
			set_property(TARGET ZLIB::ZLIB APPEND PROPERTY
				IMPORTED_CONFIGURATIONS RELEASE)
			set_target_properties(ZLIB::ZLIB PROPERTIES
				IMPORTED_LOCATION_RELEASE "${ZLIB_LIBRARY_RELEASE}")
		endif()

		if(ZLIB_LIBRARY_DEBUG)
			set_property(TARGET ZLIB::ZLIB APPEND PROPERTY
				IMPORTED_CONFIGURATIONS DEBUG)
			set_target_properties(ZLIB::ZLIB PROPERTIES
				IMPORTED_LOCATION_DEBUG "${ZLIB_LIBRARY_DEBUG}")
		endif()
	endif()
endif()
