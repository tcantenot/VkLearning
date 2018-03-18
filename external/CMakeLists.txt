project(external C CXX)

set(BUILD_TESTING_BCKP ${BUILD_TESTING})
set(BUILD_TESTING OFF CACHE BOOL "Force disable of tests for external dependencies" FORCE)

set(CUSTOM_COVERAGE_EXCLUDE ${CUSTOM_COVERAGE_EXCLUDE} "external" PARENT_SCOPE) # Replaced in CTestCustom.cmake.in

# Use fmtlib for formatting
add_subdirectory(fmt EXCLUDE_FROM_ALL)
add_library(ext::fmt ALIAS fmt)

# Use spdlog for logging, and tell it to use our version of fmtlib
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
target_compile_definitions(spdlog INTERFACE SPDLOG_FMT_EXTERNAL)
add_library(ext::spdlog ALIAS spdlog)

# glfw
set(GLFW_BUILD_EXAMPLES OFF CACHE STRING "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE STRING "" FORCE)
set(GLFW_INSTALL        OFF CACHE STRING "" FORCE)
add_subdirectory(glfw EXCLUDE_FROM_ALL)
add_library(ext::glfw ALIAS glfw)

# glm
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)
add_library(ext::glm ALIAS glm)


if(0)

#Doctest for unit tests
add_library(doctest INTERFACE)
target_include_directories(doctest INTERFACE doctest/doctest)
add_library(ext::doctest ALIAS doctest)


# glew
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(glew EXCLUDE_FROM_ALL)
target_compile_definitions(libglew_static INTERFACE -DGLEW_STATIC)
add_library(ext::glew_static ALIAS libglew_static)

# gli
add_subdirectory(gli INTERFACE)
add_library(ext::gli ALIAS gli)


# stb_image
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE stb)
add_library(ext::stb ALIAS stb)


# zlib
add_subdirectory(zlib EXCLUDE_FROM_ALL)
add_library(ext::zlib ALIAS zlibstatic)


# imgui
add_library(imgui EXCLUDE_FROM_ALL
	imgui/imconfig.h
	imgui/imgui.cpp
	imgui/imgui.h
	imgui/imgui_demo.cpp
	imgui/imgui_draw.cpp
)
target_include_directories(imgui PUBLIC imgui)
add_library(ext::imgui ALIAS imgui)


# assimp
set(ZLIB_LIBRARIES zlibstatic)
set(ENABLE_BOOST_WORKAROUND ON CACHE STRING "" FORCE)
set(BUILD_STATIC_LIB 		ON CACHE STRING "" FORCE)
set(BUILD_ASSIMP_TOOLS  	ON CACHE STRING "" FORCE)
set(ASSIMP_BUILD_STATIC_LIB ON CACHE STRING "" FORCE)
add_subdirectory(assimp EXCLUDE_FROM_ALL)
add_library(ext::assimp ALIAS assimp)


#Add new external (non-interface) targets to this list
set_target_properties(
	fmt
	libglew_static
	glfw
	zlibstatic
	imgui
	assimp
	PROPERTIES FOLDER "external"
)
endif()

set(BUILD_TESTING ${BUILD_TESTING_BCKP} CACHE BOOL "Build tests (default variable for CTest)" FORCE) #Set it back to its past value
