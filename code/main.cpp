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

const std::vector<char const *> DeviceExtensions =
{
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

struct SwapchainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class HelloWorldApp final : public App
{
	public:
		HelloWorldApp(): 
			m_window(nullptr),
			m_vkInstance(VK_NULL_HANDLE),
			m_debugCallback(),
			m_vkSurface(VK_NULL_HANDLE),
			m_vkPhysicalDevice(VK_NULL_HANDLE),
			m_vkDevice(VK_NULL_HANDLE),
			m_vkGraphicsQueue(VK_NULL_HANDLE),
			m_vkPresentQueue(VK_NULL_HANDLE),
			m_vkSwapchain(VK_NULL_HANDLE),
			m_vkSwapchainImages(),
			m_vkSwapchainImageFormat(),
			m_vkSwapchainExtent(),
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

			if(!createSwapchain())
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
			vkDestroySwapchainKHR(m_vkDevice, m_vkSwapchain, nullptr);
			m_vkSwapchain = VK_NULL_HANDLE;

			vkDestroyDevice(m_vkDevice, nullptr);
			m_vkDevice = VK_NULL_HANDLE;

			// The physical device is destroyed when the instance is
			m_vkPhysicalDevice = VK_NULL_HANDLE;

			DestroyDebugReportCallbackEXT(m_vkInstance, m_debugCallback, nullptr);

			vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, nullptr);
			m_vkSurface = VK_NULL_HANDLE;

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

		// TODO: make this static or free function
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const
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

		// TODO: make this static or free function
		bool isDeviceSuitable(VkPhysicalDevice device) const
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

			if(queueFamilyIndices.isComplete())
			{
				if(checkDeviceExtensionSupport(device))
				{
					SwapchainSupportDetails SwapchainSupport = querySwapchainSupport(device);
					bool SwapchainAdequate = (SwapchainSupport.formats.size() > 0) &&
											 (SwapchainSupport.presentModes.size() > 0);

					return SwapchainAdequate;
				}
			}

			return false;
		}

		bool checkDeviceExtensionSupport(VkPhysicalDevice device) const
		{
			uint32_t extensionCount = 0;
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

			std::vector<VkExtensionProperties> availableExtensions(extensionCount);
			vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, &availableExtensions[0]);

			for(char const * requiredExtension : DeviceExtensions)
			{
				bool bFound = false;
				for(VkExtensionProperties const & extension : availableExtensions)
				{
					if(strcmp(requiredExtension, extension.extensionName) == 0)
					{
						bFound = true;
						break;
					}
				}

				if(!bFound)
					return false;
			}
		
			return true;
		}

		SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device) const
		{
			SwapchainSupportDetails SwapchainSupportDetails;

			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, m_vkSurface, &SwapchainSupportDetails.capabilities);

			uint32_t formatCount;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkSurface, &formatCount, nullptr);

			if(formatCount > 0)
			{
				SwapchainSupportDetails.formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_vkSurface, &formatCount, &SwapchainSupportDetails.formats[0]);
			}

			uint32_t modeCount;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkSurface, &modeCount, nullptr);

			if(modeCount > 0)
			{
				SwapchainSupportDetails.presentModes.resize(modeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_vkSurface, &modeCount, &SwapchainSupportDetails.presentModes[0]);
			}


			return SwapchainSupportDetails;
		}

		static VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR> const & availableFormats)
		{
			if((availableFormats.size() == 1) && (availableFormats[0].format == VK_FORMAT_UNDEFINED))
			{
				// The surface has no preferred format, so we can choose the one we want
				return { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
			}

			for(VkSurfaceFormatKHR const & availableFormat : availableFormats)
			{
				if((availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM) &&
				   (availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				)
				{
					return availableFormat;
				}
			}

			return availableFormats[0];
		}

		static VkPresentModeKHR chooseSwapSurfacePresentMode(std::vector<VkPresentModeKHR> const & availablePresentModes)
		{
			// VK_PRESENT_MODE_IMMEDIATE_KHR
			// VK_PRESENT_MODE_FIFO_KHR
			// VK_PRESENT_MODE_FIFO_RELAXED_KHR
			// VK_PRESENT_MODE_MAILBOX_KHR

			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for(VkPresentModeKHR const & availablePresentMode : availablePresentModes)
			{
				if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					return availablePresentMode;
				}
				else if(availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				{
					bestMode = availablePresentMode; 
				}
			}

			return bestMode;
		}

		static VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR const & capabilities, uint32_t width, uint32_t height)
		{
			if(capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
			{
				return capabilities.currentExtent;
			}
			else
			{
				VkExtent2D actualExtent = { width, height };
				actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
				actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
			
				return actualExtent;
			}
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
			createInfo.enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size());
			createInfo.ppEnabledExtensionNames = &DeviceExtensions[0];

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

		bool createSwapchain()
		{
			SwapchainSupportDetails SwapchainSupport = querySwapchainSupport(m_vkPhysicalDevice);

			VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(SwapchainSupport.formats);
			VkPresentModeKHR presentMode = chooseSwapSurfacePresentMode(SwapchainSupport.presentModes);

			int32_t width, height;
			glfwGetWindowSize(m_window, &width, &height);

			VkExtent2D extent = chooseSwapExtent(SwapchainSupport.capabilities, width, height);

			uint32_t imageCount = SwapchainSupport.capabilities.minImageCount + 1; // +1 for triple buffering

			// capabilities.maxImageCount = 0 means that there are no limi besides memory requirements
			if((SwapchainSupport.capabilities.maxImageCount > 0) &&
			   (imageCount > SwapchainSupport.capabilities.maxImageCount)
			)
			{
				imageCount = SwapchainSupport.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR createInfo = { };
			createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			createInfo.surface = m_vkSurface;
			createInfo.minImageCount = imageCount;
			createInfo.imageFormat = surfaceFormat.format;
			createInfo.imageColorSpace = surfaceFormat.colorSpace;
			createInfo.imageExtent = extent;
			createInfo.imageArrayLayers = 1;
			createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			QueueFamilyIndices queueFamilyIndices = findQueueFamilies(m_vkPhysicalDevice);
			uint32_t pQueueFamilyIndices[] = { uint32_t(queueFamilyIndices.graphicsFamily), uint32_t(queueFamilyIndices.presentFamily) };
		
			if(queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily)
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				createInfo.queueFamilyIndexCount = 2;
				createInfo.pQueueFamilyIndices = pQueueFamilyIndices;
			}
			else
			{
				createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				createInfo.queueFamilyIndexCount = 0;
				createInfo.pQueueFamilyIndices = nullptr;
			}

			createInfo.preTransform = SwapchainSupport.capabilities.currentTransform;
			createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			createInfo.presentMode = presentMode;
			createInfo.clipped = VK_TRUE;

			createInfo.oldSwapchain = VK_NULL_HANDLE;

			if(vkCreateSwapchainKHR(m_vkDevice, &createInfo, nullptr, &m_vkSwapchain))
			{
				fmt::print("Failed to create Swapchain\n");
				return false;
			}

			vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &imageCount, nullptr);
			m_vkSwapchainImages.resize(imageCount);
			vkGetSwapchainImagesKHR(m_vkDevice, m_vkSwapchain, &imageCount, &m_vkSwapchainImages[0]);

			m_vkSwapchainImageFormat = surfaceFormat.format;
			m_vkSwapchainExtent = extent;

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
		VkSwapchainKHR m_vkSwapchain;
		std::vector<VkImage> m_vkSwapchainImages;
		VkFormat m_vkSwapchainImageFormat;
		VkExtent2D m_vkSwapchainExtent;
		bool m_bEnableValidationLayers;
};

int main()
{
	HelloWorldApp app;
	return app.run();
}
