add_executable(platypus-tests catch_main.cpp)
_pp__link_static(platypus-tests)

target_link_libraries(platypus-tests
	PRIVATE
		Catch2::Catch2
		platypus
)

target_sources(platypus-tests
	PRIVATE
		resource_cache/loaders/mockResourceLoader.hpp
		resource_cache/resources/mockResource.hpp
		resource_cache/stores/mockResourceStore.hpp
		resource_cache/resourceCache.cpp

		utilities/logging/mockLoggingSystem.hpp
		utilities/encoding.cpp
)

target_include_directories(platypus-tests
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/..
)

include(CTest)
catch_discover_tests(platypus-tests)

if(WITH_COVERAGE)
	if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(platypus-tests
			PRIVATE
				-fprofile-instr-generate -fcoverage-mapping
		)
	elseif(CMAKE_CSS_COMPILER_ID STREQUAL "MSVC")
		target_compile_options(platypus-tests
			PRIVATE
			/fsanitize-coverage=inline-8bit-counters
		)
	endif()
endif()
