add_subdirectory(core)
#add_subdirectory(demos)
#add_subdirectory(tools)

################
##  Executable 
################

# Always list the files explicitly
set(VKLEARNING_SOURCE_FILES main.cpp)

# Vulkan
find_package(Vulkan REQUIRED)

add_executable(VkLearning ${VKLEARNING_SOURCE_FILES})
target_link_libraries(VkLearning
    PUBLIC # Useful for libraries, see https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html for more details about transitive usage requirements.
        #libraries/targets to link when linking this library
        #this will automatically setup the needed flags and dependencies when linking against this target
		vkl::core
    PRIVATE # The following libraries are only linked for this target, and its flags/dependencies will not be used when linking against this target
		general
			Vulkan::Vulkan
			ext::fmt
			ext::spdlog
			ext::glfw
			ext::glm
			#ext::glew_static
			#ext::imgui
			#ext::zlib
			#ext::gli
		#It is possible to link some libraries for debug or optimized builds only
        #debug DEBUGLIBS
        #optimized RELEASELIBS
)

# Require c++14, this is better than setting CMAKE_CXX_STANDARD since it won't pollute other targets
# note : cxx_std_* features were added in CMake 3.8.2
target_compile_features(VkLearning PRIVATE cxx_std_14)

# CMake scripts extensions
target_set_warnings(VkLearning ENABLE ALL AS_ERROR ALL DISABLE Annoying) # Helper that can set default warning flags for you
target_enable_lto(VkLearning optimized) #enable lto if available for non-debug configurations
run_fixup(VkLearning "" "") # Copy dependencies next to the executable (DLLs for example)
