# Based on cpp-boilerplate by Lectem: https://github.com/Lectem/cpp-boilerplate
#
# Project specific options :
#   - VKLEARNING_USE_DOXYGEN
#   - VKLEARNING_BUILD_TESTS (requires BUILD_TESTING set to ON)
# Other options might be available through the cmake scripts including (not exhaustive):
#   - ENABLE_WARNINGS_SETTINGS
#   - ENABLE_LTO
#
cmake_minimum_required(VERSION 3.8.2)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "Do not build in-source. Please remove CMakeCache.txt and the CMakeFiles/ directory. Then build out-of-source.")
endif()

# Put the project early since modules might need to detect the compiler
project("VkLearning" C CXX)


############################
##  Modules and scripts
############################

# Standard CMake modules

#include(CTest) # Must be called before adding tests but after calling project(). This automatically calls enable_testing() and configures ctest targets when using Make/Ninja
include(CMakeDependentOption)# This is a really useful scripts that creates options that depends on other options. It can even be used with generator expressions !

# Custom modules and scripts

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(LTO)
include(Warnings)
include(RunFixupBundle)
include(Coverage)


###############
##  Options
###############

# Doxygen
option(VKLEARNING_USE_DOXYGEN "Add a doxygen target to generate the documentation" ON)

# Tests
option(BUILD_TESTING "" ON)
    
# Use your own option for tests, in case people use your library through add_subdirectory
cmake_dependent_option(VKLEARNING_BUILD_TESTS
    "Enable VkLearning project tests targets" ON # By default we want tests if CTest is enabled
    "BUILD_TESTING" OFF # Stay coherent with CTest variables
)

# Let the user add options to the test runner if needed
set(TEST_RUNNER_PARAMS "--force-colors=true" CACHE STRING "Options to add to our test runners commands")


# Global project setup

# It is always easier to navigate in an IDE when projects are organized in folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Check for LTO support (needs to be after project(...) )
find_lto(CXX)

# External dependencies
add_subdirectory(external EXCLUDE_FROM_ALL)

# We want at least c++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#############
## Tests
#############

if(VKLEARNING_BUILD_TESTS)

	enable_testing()

    # Exclude test folders from coverage, keep in sync with .codecov.yml
    list(APPEND CUSTOM_COVERAGE_EXCLUDE "/tests/")
	
    # You can setup some custom variables and add them to the CTestCustom.cmake.in template to have custom ctest settings
    # For example, you can exclude some directories from the coverage reports such as third-parties and tests
    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/cmake/CTestCustom.cmake.in
        ${CMAKE_CURRENT_BINARY_DIR}/CTestCustom.cmake
        @ONLY
    )
endif()


#############
## Code
#############


# Add source code directory
add_subdirectory(code)

# Setup our project as the startup project for Visual so that people don't need to do it manually
set_directory_properties(PROPERTIES VS_STARTUP_PROJECT VkLearning)

#############
## Doxygen
#############

if(VKLEARNING_USE_DOXYGEN AND CMAKE_VERSION VERSION_GREATER_EQUAL 3.9)
    find_package(Doxygen
        OPTIONAL_COMPONENTS dot mscgen dia
    )
    if(DOXYGEN_FOUND)
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
        doxygen_add_docs(
            doc
            README.md source
            COMMENT "Generate man pages"
        )
    endif()
endif()

