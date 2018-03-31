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


#include <cstring>
#include <cstdint>
#include <vector>
#include <set>

#include <App/App.hpp>

namespace {

const std::vector<char const *> ValidationLayers =
{
	"VK_LAYER_LUNARG_standard_validation"
};

}

static bool CheckValidationLayerSupport()
{
	uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, &availableLayers[0]);

	for(char const * layerName : ValidationLayers)
	{
		bool bLayerFound = false;
		for(VkLayerProperties const & layerProperty : availableLayers)
		{
			if(strcmp(layerName, layerProperty.layerName) == 0)
			{
				bLayerFound = true;
				break;
			}
		}

		if(!bLayerFound)
		{
			fmt::print("Layer \"{}\" not found", layerName);
			return false;
		}
	}

	return true;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    char const * layerPrefix,
    char const * msg,
    void * userData
)
{
	(void) flags;
	(void) objType;
	(void) obj;
	(void) location;
	(void) code;
	(void) layerPrefix;
	(void) userData;

	fmt::print("Validation layer: {}\n", msg);

	// The callback returns a boolean that indicates if the Vulkan call that triggered
	// the validation layer message should be aborted. If the callback returns true, 
	// then the call is aborted with the VK_ERROR_VALIDATION_FAILED_EXT error. 
	// This is normally only used to test the validation layers themselves, 
	// so you should always return VK_FALSE.
	return VK_FALSE;
}

static VkResult CreateDebugReportCallbackEXT(
	VkInstance instance,
	VkDebugReportCallbackCreateInfoEXT const * pCreateInfo,
	VkAllocationCallbacks const * pAllocator,
	VkDebugReportCallbackEXT * pCallback
)
{
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    return func ? func(instance, pCreateInfo, pAllocator, pCallback) : VK_ERROR_EXTENSION_NOT_PRESENT;
}

static void DestroyDebugReportCallbackEXT(
	VkInstance instance,
	VkDebugReportCallbackEXT callback,
	VkAllocationCallbacks const * pAllocator
)
{
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if(func != nullptr) 
	{
        func(instance, callback, pAllocator);
    }
}

struct QueueFamilyIndices
{
	int32_t graphicsFamily = -1;
	int32_t presentFamily  = -1;

	bool isComplete() const
	{
		return (graphicsFamily >= 0) && (presentFamily >= 0);
	}
};

class HelloWorldApp final : public App
{
	public:
		HelloWorldApp(): 
			m_window(nullptr),
			m_vkInstance(VK_NULL_HANDLE),
			m_debugCallback(),
			m_vkSurface(0),
			m_vkPhysicalDevice(VK_NULL_HANDLE),
			m_vkDevice(VK_NULL_HANDLE),
			m_vkGraphicsQueue(VK_NULL_HANDLE),
			m_vkPresentQueue(VK_NULL_HANDLE),
			m_bEnableValidationLayers(true)
		{
		
		}

	private:
		bool init() override
		{
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			m_window = glfwCreateWindow(800, 600, "Vulkan window", nullptr, nullptr);

			if(!m_window)
			{
				return false;
			}

			if(!createInstance())
			{
				return false;
			}

			if(!setupDebugCallback())
			{
				return false;
			}

			if(!createSurface())
			{
				return false;
			}

			if(!selectPhysicalDevice())
			{
				return false;
			}

			if(!createLogicalDevice())
			{
				return false;
			}

			return true;
		}

		void mainLoop() override
		{
		    while(!glfwWindowShouldClose(m_window))
			{
				glfwPollEvents();
			}
		}

		void cleanup() override
		{
			vkDestroyDevice(m_vkDevice, nullptr);
			m_vkDevice = VK_NULL_HANDLE;

			// The physical device is destroyed when the instance is
			m_vkPhysicalDevice = VK_NULL_HANDLE;

			DestroyDebugReportCallbackEXT(m_vkInstance, m_debugCallback, nullptr);

			vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
			m_vkSurface = 0;

			vkDestroyInstance(m_vkInstance, nullptr);
			m_vkInstance = VK_NULL_HANDLE;

			glfwDestroyWindow(m_window);
			m_window = nullptr;

			glfwTerminate();
		}

	private:

		bool createInstance()
		{
			// Extensions
			{
				uint32_t extensionCount = 0;
				vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
				std::vector<VkExtensionProperties> extensions(extensionCount);
				vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, &extensions[0]);
				fmt::print("{} extension{} supported\n", extensionCount, (extensionCount > 1 ? "s" : ""));
				for(VkExtensionProperties const & extension : extensions)
				{
					fmt::print("\t{}\n", extension.extensionName);
				}
			}

			// (Optional) application info
			VkApplicationInfo appInfo = { };
			appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			appInfo.pApplicationName = "Hello world";
			appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.pEngineName = "Vulkan learning engine";
			appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			appInfo.apiVersion = VK_API_VERSION_1_1;

			// Instance creation info
			VkInstanceCreateInfo createInfo = { };
			createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			createInfo.pApplicationInfo = &appInfo;

			auto extensions = getRequiredExtensions(m_bEnableValidationLayers);

			createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
			createInfo.ppEnabledExtensionNames = &extensions[0];

			// Validation layers
			if(m_bEnableValidationLayers && !CheckValidationLayerSupport())
			{
				return false;
			}

			if(m_bEnableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
				createInfo.ppEnabledLayerNames = &ValidationLayers[0];
			}
			else
			{
				createInfo.enabledLayerCount = 0;
			}

			VkResult result = vkCreateInstance(&createInfo, nullptr, &m_vkInstance);

			if(result != VK_SUCCESS)
			{
				fmt::print("Failed to create Vulkan instance\n");
				return false;
			}

			return true;
		}

		std::vector<char const *> getRequiredExtensions(bool bEnableValidationLayers) const
		{
			uint32_t glfwExtensionCount = 0;
			const char ** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
			
			fmt::print("GLFW required extensions:\n");
			for(uint32_t i = 0; i < glfwExtensionCount; ++i)
			{
				fmt::print("\t{}\n", glfwExtensions[i]);
			}

			std::vector<char const *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

			if(bEnableValidationLayers)
			{
				extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			}

			return extensions;
		}

		bool setupDebugCallback()
		{
			VkDebugReportCallbackCreateInfoEXT createInfo = { };
			createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
			createInfo.flags = 0;
			//createInfo.flags |= VK_DEBUG_REPORT_INFORMATION_BIT_EXT;
			createInfo.flags |= VK_DEBUG_REPORT_WARNING_BIT_EXT;
			createInfo.flags |= VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
			createInfo.flags |= VK_DEBUG_REPORT_ERROR_BIT_EXT;
			//createInfo.flags |= VK_DEBUG_REPORT_DEBUG_BIT_EXT;
			createInfo.pfnCallback = VulkanDebugCallback;

			if(CreateDebugReportCallbackEXT(m_vkInstance, &createInfo, nullptr, &m_debugCallback) != VK_SUCCESS)
			{
				fmt::print("Failed to set up debug callback\n");
				return false;
			}

			return true;
		}

		bool createSurface()
		{
			if(glfwCreateWindowSurface(m_vkInstance, m_window, nullptr, &m_vkSurface) != VK_SUCCESS)
			{
				fmt::print("Failed to create window surface\n");
				m_vkSurface = 0;
				return false;
			}

			return true;
		}

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
		{
			QueueFamilyIndices queueFamilyIndices;

			uint32_t queueFamilyCount = 0;
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

			std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
			vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, &queueFamilies[0]);

			for(uint32_t i = 0; i < queueFamilyCount; ++i)
			{
				VkQueueFamilyProperties const & queueFamily = queueFamilies[i];

				if(queueFamily.queueCount == 0)
					continue;

				if(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					queueFamilyIndices.graphicsFamily = i;
				}

				VkBool32 presentSupport = false;
				vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_vkSurface, &presentSupport);

				if(presentSupport)
				{
					queueFamilyIndices.presentFamily = i;
				}

				if(queueFamilyIndices.isComplete())
					break;
			}

			return queueFamilyIndices;
		}

		bool selectPhysicalDevice()
		{
			uint32_t deviceCount = 0;
			vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, nullptr);

			if(deviceCount == 0)
			{
				fmt::print("Could not find device with Vulkan support\n");
				return false;
			}
			
			std::vector<VkPhysicalDevice> devices(deviceCount);
			vkEnumeratePhysicalDevices(m_vkInstance, &deviceCount, &devices[0]);

			fmt::print("{} Vulkan compatible device{} found\n", deviceCount, deviceCount > 1 ? "s" : "");
			for(VkPhysicalDevice const & device : devices)
			{
				VkPhysicalDeviceProperties deviceProperties;
				vkGetPhysicalDeviceProperties(device, &deviceProperties);

				char const * deviceTypeStr = "Unknown device";
				switch(deviceProperties.deviceType)
				{
					case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
						deviceTypeStr = "Discrete GPU";
						break;
					case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
						deviceTypeStr = "Integrated GPU";
						break;
					case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
						deviceTypeStr = "Virtual GPU";
						break;
					case VK_PHYSICAL_DEVICE_TYPE_CPU:
						deviceTypeStr = "CPU";
						break;
					case VK_PHYSICAL_DEVICE_TYPE_OTHER:
						deviceTypeStr = "\"Other\" device";
						break;
					default: break;
				}

				fmt::print("\t{}: {}\n", deviceTypeStr, deviceProperties.deviceName);
				fmt::print("\t\tDriver version: {}\n", deviceProperties.driverVersion);
				fmt::print("\t\tAPI version: {}\n", deviceProperties.apiVersion);
				fmt::print("\t\tVendor ID: {}\n", deviceProperties.vendorID);
				fmt::print("\t\tDevice ID: {}\n", deviceProperties.deviceID);
			}

			const auto isDeviceSuitable = [&](VkPhysicalDevice device)
			{
				#if 0
				VkPhysicalDeviceProperties deviceProperties;
				vkGetPhysicalDeviceProperties(device, &deviceProperties);

				VkPhysicalDeviceFeatures deviceFeatures;
				vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

				return	(deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) &&
						(deviceFeatures.geometryShader);
				#endif

				QueueFamilyIndices queueFamilyIndices = findQueueFamilies(device);
				return queueFamilyIndices.isComplete();
			};

			for(VkPhysicalDevice const & device : devices)
			{
				if(isDeviceSuitable(device))
				{
					m_vkPhysicalDevice = device;
					break;
				}
			}

			if(m_vkPhysicalDevice == VK_NULL_HANDLE)
			{
				fmt::print("Could not find a suitable GPU\n");
				return false;
			}

			return true;
		}

		bool createLogicalDevice()
		{
			// TODO: query queue family indices only once
			QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_vkPhysicalDevice);

			std::set<int32_t> uniqueQueueFamilies = { queueFamilyIndices.graphicsFamily, queueFamilyIndices.presentFamily };
			std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
			queueCreateInfos.reserve(uniqueQueueFamilies.size());

			float queuePriority = 1.0f;

			for(int32_t queueFamiliy : uniqueQueueFamilies)
			{
				VkDeviceQueueCreateInfo queueCreateInfo = { };
				queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex = queueFamiliy;
				queueCreateInfo.queueCount = 1;
				queueCreateInfo.pQueuePriorities = &queuePriority;
				queueCreateInfos.push_back(queueCreateInfo);
			}

			VkPhysicalDeviceFeatures deviceFeatures = { };

			VkDeviceCreateInfo createInfo = { };
			createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			createInfo.pQueueCreateInfos = &queueCreateInfos[0];
			createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
			createInfo.pEnabledFeatures = &deviceFeatures;
			createInfo.enabledExtensionCount = 0;

			if(m_bEnableValidationLayers)
			{
				createInfo.enabledLayerCount = static_cast<uint32_t>(ValidationLayers.size());
				createInfo.ppEnabledLayerNames = &ValidationLayers[0];
			}
			else
			{
				createInfo.enabledLayerCount = 0;
			}

			if(vkCreateDevice(m_vkPhysicalDevice, &createInfo, nullptr, &m_vkDevice) != VK_SUCCESS)
			{
				fmt::print("Failed to create logical device\n");
				return false;
			}

			// Get graphics and present queues from the device (can be the same)
			vkGetDeviceQueue(m_vkDevice, queueFamilyIndices.graphicsFamily, 0, &m_vkGraphicsQueue);
			vkGetDeviceQueue(m_vkDevice, queueFamilyIndices.presentFamily,  0, &m_vkPresentQueue);

			return true;
		}

	private:
		GLFWwindow * m_window;
		VkInstance m_vkInstance;
		VkDebugReportCallbackEXT m_debugCallback;
		VkSurfaceKHR m_vkSurface;
		VkPhysicalDevice m_vkPhysicalDevice;
		VkDevice m_vkDevice;
		VkQueue m_vkGraphicsQueue;
		VkQueue m_vkPresentQueue;
		bool m_bEnableValidationLayers;
};

int main()
{
	HelloWorldApp app;
	return app.run();
}
