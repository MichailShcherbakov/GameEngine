#ifndef _TOOL_KIT_H_
#define _TOOL_KIT_H_

#include "vulkan/vulkan.h"
#include "Device/Device.h"
#include "Swapchain/Swapchain.h"
#include <vector>
#include <iostream>

#define VK_CHECK_ERROR(result, message) \
if (result != VK_SUCCESS) \
	throw std::runtime_error(message);

#define VK_CHECK_NULL_HANDLE(result, message) \
if (result == VK_NULL_HANDLE) \
	throw std::runtime_error(message);

namespace EngineRenderers
{
	namespace Vulkan
	{
		bool CheckRequestedInstanceLayersValidation(const std::vector<const char*>& requestedLayers);
		bool CheckRequestedInstanceExtensions(const std::vector<const char*>& requestedExtensions);

		bool CheckRequestedDeviceLayersValidation(const VkPhysicalDevice& device, const std::vector<const char*>& requestedLayers);
		bool CheckRequestedDeviceExtensions(const VkPhysicalDevice& device, const std::vector<const char*>& requestedExtensions);

		EngineRenderers::Vulkan::QueueFamilyInfo GetQueueFamilyInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, bool uniqueStrict = false);
		EngineRenderers::Vulkan::SurfaceInfo GetSurfaceInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);

		VkShaderModule LoadShader(const std::string path, const VkDevice& device);

		VKAPI_ATTR VkBool32 VKAPI_CALL DebugUtilsMessengerCallback
		(
			VkDebugUtilsMessageSeverityFlagBitsEXT			messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT					messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData
		);
	}
}

#endif