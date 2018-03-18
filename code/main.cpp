/** @file main.cpp
 * Just a simple hello world using libfmt
 */
// The previous block is needed in every file for which you want to generate documentation

#include <Config/CompilerWarnings.hpp>

DISABLE_WARNING_MSVC(4459) //  Hides global declaration
DISABLE_WARNING_MSVC(4244) // '=': conversion from 'const wchar_t' to 'char', possible loss of data
#include <fmt/format.h>
ENABLE_WARNING_MSVC(4244) // '=': conversion from 'const wchar_t' to 'char', possible loss of data
ENABLE_WARNING_MSVC(4459) //  Hides global declaration

DISABLE_WARNING_CLANG(unknown-pragmas)

DISABLE_WARNING_MSVC(4201) // Nameless struct/union
DISABLE_WARNING_MSVC(4309) // 'static_cast': truncation of constant value
DISABLE_WARNING_MSVC(4458) // Declaration of 'x' hides class member
DISABLE_WARNING_MSVC(4100) // Unreferenced formal parameter

DISABLE_WARNING_GCC(pedantic)
DISABLE_WARNING_GCC(unused-parameter)
DISABLE_WARNING_GCC(ignored-qualifiers)
DISABLE_WARNING_GCC(type-limits)
DISABLE_WARNING_GCC(empty-body)
DISABLE_WARNING_GCC(unused-variable)

DISABLE_WARNING_CLANG(conversion)
DISABLE_WARNING_CLANG(sign-conversion)
DISABLE_WARNING_CLANG(ignored-qualifiers)
DISABLE_WARNING_CLANG(shadow-field-in-constructor)
DISABLE_WARNING_CLANG(shadow)
DISABLE_WARNING_CLANG(extra-semi)
DISABLE_WARNING_CLANG(unused-variable)
DISABLE_WARNING_CLANG(unused-parameter)
DISABLE_WARNING_CLANG(deprecated)
DISABLE_WARNING_CLANG(old-style-cast)
DISABLE_WARNING_CLANG(switch-enum)
DISABLE_WARNING_CLANG(weak-vtables)

//#include <gli/gli.hpp>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm\vec4.hpp>
#include <glm\mat4x4.hpp>

ENABLE_WARNING_CLANG(weak-vtables)
ENABLE_WARNING_CLANG(switch-enum)
ENABLE_WARNING_CLANG(old-style-cast)
ENABLE_WARNING_CLANG(deprecated)
ENABLE_WARNING_CLANG(unused-parameter)
ENABLE_WARNING_CLANG(unused-variable)
ENABLE_WARNING_CLANG(extra-semi)
ENABLE_WARNING_CLANG(shadow)
ENABLE_WARNING_CLANG(shadow-field-in-constructor)
ENABLE_WARNING_CLANG(ignored-qualifiers)
ENABLE_WARNING_CLANG(sign-conversion)
ENABLE_WARNING_CLANG(conversion)

ENABLE_WARNING_GCC(unused-variable)
ENABLE_WARNING_GCC(empty-body)
ENABLE_WARNING_GCC(type-limits)
ENABLE_WARNING_GCC(ignored-qualifiers)
ENABLE_WARNING_GCC(unused-parameter)
ENABLE_WARNING_GCC(pedantic)

ENABLE_WARNING_MSVC(4100) // Unreferenced formal parameter
ENABLE_WARNING_MSVC(4458) // Declaration of 'x' hides class member
ENABLE_WARNING_MSVC(4309) // 'static_cast': truncation of constant value
ENABLE_WARNING_MSVC(4201) // Nameless struct/union

ENABLE_WARNING_CLANG(unknown-pragmas)


#include <iostream>

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

	fmt::print("{} extension{} supported", extensionCount, (extensionCount > 1 ? "s" : ""));

    glm::mat4 matrix;
    glm::vec4 vec;
    auto test = matrix * vec;

    while(!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
