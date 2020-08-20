set(project platypus)
project(${project})

include_directories(
	${CMAKE_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(main_srcs
	platypus.cpp
)
source_group(source FILES ${main_srcs})

set(all_srcs
	${main_srcs}
)

add_library(${project} ${all_srcs})

target_link_libraries(${project}
#	platypus-proto
)
