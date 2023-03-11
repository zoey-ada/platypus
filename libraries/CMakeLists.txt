set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/../cmake;)
set(BUILD_SHARED_LIBS off)

# -------------------------------------------------------------------------------
# zlib
# -------------------------------------------------------------------------------
add_subdirectory(zlib)

_p__ignore_warnings(zlib)
_p__link_static(zlib)

_p__ignore_warnings(zlibstatic)
_p__link_static(zlibstatic)

set_target_properties(zlib PROPERTIES FOLDER libraries/zlib)
set_target_properties(zlibstatic PROPERTIES FOLDER libraries/zlib)
set_target_properties(example PROPERTIES FOLDER libraries/zlib)
set_target_properties(minigzip PROPERTIES FOLDER libraries/zlib)

set_target_properties(example PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
)

set_target_properties(minigzip PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
)

#-------------------------------------------------------------------------------
# assimp
#-------------------------------------------------------------------------------
set(ASSIMP_HUNTER_ENABLED off) # Enable Hunter package manager support
set(BUILD_SHARED_LIBS off) # Build package with shared libraries.
set(ASSIMP_BUILD_FRAMEWORK off) # Build package as Mac OS X Framework bundle.
set(ASSIMP_DOUBLE_PRECISION off) # Set to ON to enable double precision processing
set(ASSIMP_OPT_BUILD_PACKAGES off) # Set to ON to generate CPack configuration files and packaging targets
set(ASSIMP_ANDROID_JNIIOSYSTEM off) # Android JNI IOSystem support is active
set(ASSIMP_NO_EXPORT on) # Disable Assimp's export functionality.
# set(ASSIMP_BUILD_ZLIB on) # Build your own zlib		This setting seems needs to be off for some reason for the built zlib to be found!!
set(ASSIMP_BUILD_ASSIMP_TOOLS off) # If the supplementary tools for Assimp are built in addition to the library.
set(ASSIMP_BUILD_SAMPLES off) # If the official samples are built as well (needs Glut).
set(ASSIMP_BUILD_TESTS off) # If the test suite for Assimp is built in addition to the library.
set(ASSIMP_COVERALLS off) # Enable this to measure test coverage.
set(ASSIMP_INSTALL off) # Disable this if you want to use assimp as a submodule.
set(ASSIMP_ERROR_MAX off) # Enable all warnings.
set(ASSIMP_ASAN off) # Enable AddressSanitizer.
set(ASSIMP_UBSAN off) # Enable Undefined Behavior sanitizer.
set(ASSIMP_BUILD_DOCS off) # Build documentation using Doxygen.
set(ASSIMP_INJECT_DEBUG_POSTFIX off) # Inject debug postfix in .a/.so/.dll lib names
set(ASSIMP_IGNORE_GIT_HASH on) # Don't call git to get the hash.
set(ASSIMP_BUILD_ASSIMP_VIEW off) # If the Assimp view tool is built. (requires DirectX)
set(ASSIMP_INSTALL_PDB on) # Install MSVC debug files.
set(ASSIMP_BUILD_DRACO off) # If the Draco libraries are to be built. Primarily for glTF

add_subdirectory(assimp)

_p__ignore_warnings(assimp)
_p__link_static(assimp)

set_target_properties(assimp PROPERTIES FOLDER libraries/assimp)
set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES FOLDER libraries/assimp)

set_target_properties(UpdateAssimpLibsDebugSymbolsAndDLLs PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
)

#-------------------------------------------------------------------------------
# catch
#-------------------------------------------------------------------------------
add_subdirectory(catch)
include(${CMAKE_CURRENT_SOURCE_DIR}/catch/contrib/Catch.cmake)

#-------------------------------------------------------------------------------
# freetype
#-------------------------------------------------------------------------------
set(SKIP_INSTALL_ALL on)

set(FT_DISABLE_ZLIB off) # Disable use of system zlib and use internal zlib library instead.
set(FT_DISABLE_BZIP2 on) # Disable support of bzip2 compressed fonts.
set(FT_DISABLE_PNG on) # Disable support of PNG compressed OpenType embedded bitmaps.
set(FT_DISABLE_HARFBUZZ on) # Disable HarfBuzz (used for improving auto-hinting of OpenType fonts).
set(FT_DISABLE_BROTLI on) # Disable support of compressed WOFF2 fonts.

add_subdirectory(freetype)

_p__ignore_warnings(freetype)
_p__link_static(freetype)

set_target_properties(freetype PROPERTIES FOLDER libraries/freetype)

target_include_directories(freetype
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/zlib
		${CMAKE_CURRENT_BINARY_DIR}/zlib
)

#-------------------------------------------------------------------------------
# libzip
#-------------------------------------------------------------------------------
set(BUILD_SHARED_LIBS off)
set(LIBZIP_DO_INSTALL off)

set(ENABLE_COMMONCRYPTO off) # Enable use of CommonCrypto
set(ENABLE_GNUTLS off) # Enable use of GnuTLS
set(ENABLE_MBEDTLS off) # Enable use of mbed TLS
set(ENABLE_OPENSSL off) # Enable use of OpenSSL
set(ENABLE_WINDOWS_CRYPTO off) # Enable use of Windows cryptography libraries

set(ENABLE_BZIP2 off) # Enable use of BZip2
set(ENABLE_LZMA off) # Enable use of LZMA
set(ENABLE_ZSTD off) # Enable use of Zstandard

set(BUILD_TOOLS off) # Build tools in the src directory (zipcmp, zipmerge, ziptool)
set(BUILD_REGRESS off) # Build regression tests
set(BUILD_EXAMPLES off) # Build examples
set(BUILD_DOC off) # Build documentation

add_subdirectory(libzip)

_p__ignore_warnings(zip)
_p__link_static(zip)

target_include_directories(zip
	PUBLIC
		${CMAKE_CURRENT_BINARY_DIR}/libzip
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/zlib
		${CMAKE_CURRENT_BINARY_DIR}/zlib
)

set_target_properties(zip PROPERTIES FOLDER libraries/libzip)
set_target_properties(dist PROPERTIES FOLDER libraries/libzip)
set_target_properties(distcheck PROPERTIES FOLDER libraries/libzip)

set_target_properties(dist PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
)

set_target_properties(distcheck PROPERTIES
	EXCLUDE_FROM_ALL true
	EXCLUDE_FROM_DEFAULT_BUILD true
)

#-------------------------------------------------------------------------------
# platypus-proto
#-------------------------------------------------------------------------------
add_subdirectory("platypus-proto")

#-------------------------------------------------------------------------------
# utf8-cpp
#-------------------------------------------------------------------------------
set(UTF8_TESTS off) # Enable tests for UTF8-CPP
set(UTF8_INSTALL off) # Enable installation for UTF8-CPP
set(UTF8_SAMPLES off) # Enable building samples for UTF8-CPP

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
