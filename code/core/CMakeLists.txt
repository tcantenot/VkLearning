# Variables set by the sub-CMakeLists.txt in src and below
set(VKL_CORE_INCLUDE_FILES "")
set(VKL_CORE_SOURCE_FILES "")

# Add include and source folders
add_subdirectory(include)
add_subdirectory(src)

# Macro used to group source by folders in the IDE
macro(GroupSources rootdir subdir)
	file(GLOB children RELATIVE ${rootdir}/${subdir} ${rootdir}/${subdir}/*)
	foreach(child ${children})
		if(IS_DIRECTORY ${rootdir}/${subdir}/${child})
			GroupSources(${rootdir} ${subdir}/${child})
		else()
			string(REPLACE "/" "\\" groupname ${subdir})
			source_group(${groupname} FILES ${rootdir}/${subdir}/${child})
		endif()
	endforeach()
endmacro()

GroupSources(${CMAKE_CURRENT_SOURCE_DIR} include)
GroupSources(${CMAKE_CURRENT_SOURCE_DIR} src)

# Library
add_library(VkCore ${VKL_CORE_INCLUDE_FILES} ${VKL_CORE_SOURCE_FILES})

target_include_directories(VkCore
	PUBLIC include
	PRIVATE src
)

target_link_libraries(VkCore
	PUBLIC ext::fmt ext::spdlog
)

target_enable_lto(VkCore optimized)

target_set_warnings(VkCore ENABLE ALL AS_ERROR ALL DISABLE Annoying)

# Give a 'scoped' name to libraries targets, as it can't be mistaken with system libraries
add_library(vkl::core ALIAS VkCore)

# Tests
#add_subdirectory(tests)