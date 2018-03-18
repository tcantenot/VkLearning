# Variables set by the sub-CMakeLists.txt in src and below
set(VKL_CORE_INCLUDE_FILES "")
set(VKL_CORE_SOURCE_FILES "")

# Add public and private source folders
add_subdirectory(public)
add_subdirectory(private)

# Library
add_library(vkl_core INTERFACE)
target_sources(vkl_core INTERFACE ${VKL_CORE_INCLUDE_FILES} ${VKL_CORE_SOURCE_FILES})
target_include_directories(vkl_core
	INTERFACE # For now as there is only a header...
	#PUBLIC # The folder must be used in the include path for any target using this library
		"public"
)
#target_link_libraries(vkl_core
	#PRIVATE
	#	ext::fmt
#)
# Give a 'scoped' name to libraries targets, as it can't be mistaken with system libraries
add_library(vkl::core ALIAS vkl_core)


# Tests
#add_subdirectory(tests)