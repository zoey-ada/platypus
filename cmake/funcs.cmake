function(_p__warnings_as_errors target)
	if(MSVC)
		# Force to always compile with W4
		target_compile_options(${target} PRIVATE /W4 /WX)
	elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
		# Update if necessary
		target_compile_options(${target} PRIVATE -Werror -Wall -Wextra -Wpedantic)
	endif()
endfunction()

function(_p__ignore_warnings target)
	get_target_property(type ${target} TYPE)
	if(NOT ${type} STREQUAL "INTERFACE_LIBRARY")
		if(MSVC)
			target_compile_options(${target} PRIVATE /w)
		else()
			target_compile_options(${target} PRIVATE -w)
		endif()
	endif()
endfunction()

function(_p__link_static target)
	set_property(TARGET ${target}
		PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
	)
endfunction()

function(_p__link_dynamic target)
	set_property(TARGET ${target}
		PROPERTY
		MSVC_RUNTIME_LIBRARY "MultiThreadedDLL$<$<CONFIG:Debug>:Debug>"
	)
endfunction()
