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
