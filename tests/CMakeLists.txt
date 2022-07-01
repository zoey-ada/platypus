add_executable(platypus-tests catch_main.cpp)
_pp__link_static(platypus-tests)

target_link_libraries(platypus-tests
	PRIVATE
		Catch2::Catch2
		platypus
)

target_sources(platypus-tests
	PRIVATE
		utilities/encoding.cpp
)

include(CTest)
catch_discover_tests(platypus-tests)
