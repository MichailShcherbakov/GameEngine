#include "StdAfx.h"
#include "Toolkit.h"

#include <fstream>

bool Render::CheckRequestedInstanceLayersValidation(const std::vector<const char*>& requestedLayers)
{
	uint32_t numberLayerProperties = 0;
	std::vector<VkLayerProperties> properties;

	vkEnumerateInstanceLayerProperties(&numberLayerProperties, nullptr);

	if (numberLayerProperties > 0)
	{
		properties.resize(numberLayerProperties);
		vkEnumerateInstanceLayerProperties(&numberLayerProperties, properties.data());

		bool isFound = false;
		for (auto requestedLayer : requestedLayers)
		{
			for (auto property : properties)
				if (strcmp(requestedLayer, property.layerName) == 0)
					isFound = true;

			if (!isFound)
				return false;
		}

		return true;
	}

	return false;
}

bool Render::CheckRequestedInstanceExtensions(const std::vector<const char*>& requestedExtensions)
{
	uint32_t numberExtensions = 0;
	std::vector<VkExtensionProperties> extensions;

	vkEnumerateInstanceExtensionProperties(nullptr, &numberExtensions, nullptr);

	if (numberExtensions > 0)
	{
		extensions.resize(numberExtensions);
		vkEnumerateInstanceExtensionProperties(nullptr, &numberExtensions, extensions.data());

		bool isFound = false;
		for (auto requestedExtension : requestedExtensions)
		{
			for (auto extension : extensions)
				if (strcmp(requestedExtension, extension.extensionName) == 0)
					isFound = true;

			if (!isFound)
				return false;
		}

		return true;
	}

	return false;
}

bool Render::CheckRequestedDeviceLayersValidation(const VkPhysicalDevice& device, const std::vector<const char*>& requestedLayers)
{
	uint32_t numberLayerProperties = 0;
	std::vector<VkLayerProperties> properties;

	vkEnumerateDeviceLayerProperties(device, &numberLayerProperties, nullptr);

	if (numberLayerProperties > 0)
	{
		properties.resize(numberLayerProperties);
		vkEnumerateDeviceLayerProperties(device, &numberLayerProperties, properties.data());

		bool isFound = false;
		for (auto requestedLayer : requestedLayers)
		{
			for (auto property : properties)
				if (strcmp(requestedLayer, property.layerName) == 0)
					isFound = true;

			if (!isFound)
				return false;
		}

		return true;
	}

	return false;
}

bool Render::CheckRequestedDeviceExtensions(const VkPhysicalDevice& device, const std::vector<const char*>& requestedExtensions)
{
	uint32_t numberExtensions = 0;
	std::vector<VkExtensionProperties> extensions;

	vkEnumerateDeviceExtensionProperties(device, nullptr, &numberExtensions, nullptr);

	if (numberExtensions > 0)
	{
		extensions.resize(numberExtensions);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &numberExtensions, extensions.data());

		bool isFound = false;
		for (auto requestedExtension : requestedExtensions)
		{
			for (auto extension : extensions)
				if (strcmp(requestedExtension, extension.extensionName) == 0)
					isFound = true;

			if (!isFound)
				return false;
		}

		return true;
	}

	return false;
}

Render::QueueFamilyInfo Render::GetQueueFamilyInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, bool uniqueStrict)
{
	Render::QueueFamilyInfo queueFamilyInfo;

	uint32_t numberQueueFamilyProperties = 0;
	std::vector<VkQueueFamilyProperties> queueFamilyProperties;

	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numberQueueFamilyProperties, nullptr);

	if (numberQueueFamilyProperties > 0)
	{
		queueFamilyProperties.resize(numberQueueFamilyProperties);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &numberQueueFamilyProperties, queueFamilyProperties.data());

		for (uint32_t index = 0; index < queueFamilyProperties.size(); ++index)
		{
			if (queueFamilyProperties[index].queueCount > 0 && queueFamilyProperties[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
			{
				queueFamilyInfo.graphics = index;
				break;
			}
		}
		VkBool32 presentSupport = false;

		for (uint32_t index = 0; index < queueFamilyProperties.size(); ++index)
		{
			if (index == queueFamilyInfo.graphics && uniqueStrict)
				continue;

			vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &presentSupport);

			if (queueFamilyProperties[index].queueCount > 0 && presentSupport)
			{
				queueFamilyInfo.present = index;
				break;
			}
		}
	}

	return queueFamilyInfo;
}

Render::SurfaceInfo Render::GetSurfaceInfo(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface)
{
	Render::SurfaceInfo surfaceInfo;

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);
	surfaceInfo.capabilities = surfaceCapabilities;

	uint32_t numberSurfaceFormats;
	std::vector<VkSurfaceFormatKHR> surfaceFormats;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &numberSurfaceFormats, nullptr);

	if (numberSurfaceFormats > 0)
	{
		surfaceFormats.resize(numberSurfaceFormats);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &numberSurfaceFormats, surfaceFormats.data());
	}

	surfaceInfo.formats = surfaceFormats;

	uint32_t numnerPresentModes;
	std::vector<VkPresentModeKHR> surfacePresentModes;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numnerPresentModes, nullptr);

	if (numnerPresentModes > 0)
	{
		surfacePresentModes.resize(numnerPresentModes);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &numnerPresentModes, surfacePresentModes.data());
	}

	surfaceInfo.peresentModes = surfacePresentModes;

	return surfaceInfo;
}

VkShaderModule Render::LoadShader(const std::string path, const VkDevice& device)
{
	VkShaderModule shaderModule = VK_NULL_HANDLE;

	char* pData = nullptr;
	size_t size = 0;

	std::ifstream is(path.c_str(), std::ios::binary | std::ios::in | std::ios::ate);

	if (is.is_open())
	{
		size = (size_t)is.tellg(); 
		is.seekg(0, std::ios::beg); 
		pData = new char[size];  
		is.read(pData, size);
		is.close();                
	}

	if (!pData || size == 0)
		throw std::runtime_error("Error while loading shader code from file: " + path);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = size;
	shaderModuleCreateInfo.pCode = (uint32_t*)pData;

	VK_CHECK_ERROR(vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule), "");

	delete[] pData;

	return shaderModule;
}


VKAPI_ATTR VkBool32 VKAPI_CALL Render::DebugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
	std::cout << pCallbackData->pMessage << std::endl;
	return VK_FALSE;
}
