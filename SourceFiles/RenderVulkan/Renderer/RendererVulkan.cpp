#include "StdAfx.h"
#include "RendererVulkan.h"

#include <string>
#include <vector>

#include "boost/filesystem.hpp"
#include "Graphics/Mesh/Mesh.h"

/*EngineRenderers::Vulkan::Renderer::Renderer(HINSTANCE hInstance, HWND hwnd)
{
	m_instance = InitInstance(
		"Union",
		"UnionEngine",
		{ "VK_LAYER_LUNARG_standard_validation" },
		{ VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME },
		m_debugUtilsMessengerEXT
	);

#if WIN32
	m_surface = InitWin32Surface(m_instance, hInstance, hwnd);
#endif

	m_device = InitDevice(
		m_instance,
		m_surface,
		{ "VK_LAYER_LUNARG_standard_validation" },
		{ VK_KHR_SWAPCHAIN_EXTENSION_NAME }, 
		false
	);

	m_renderPass = InitRenderPass(
		m_device,
		m_surface,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_D32_SFLOAT_S8_UINT
	);

	m_swapchain = InitSwapchain(
		m_device, 
		m_surface, 
		m_renderPass,
		{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR }, 
		1
	);

	m_graphicsPipeline = InitGraphicsPipeline(m_device, m_swapchain, m_pipelineLayout, m_renderPass);
}
*/
EngineRenderers::Vulkan::Renderer::~Renderer()
{
	/*DeinitGraphicsPipeline(m_graphicsPipeline, m_device);

	DeinitSwapchain(m_swapchain, m_device);

	DeinitRenderPass(m_renderPass, m_device);

	DeinitDevice(m_device);

#if WIN32
	DeinitWin32Surface(m_instance, m_surface);
#endif

	DeinitInstance(m_instance, m_debugUtilsMessengerEXT);*/
}

VkInstance EngineRenderers::Vulkan::Renderer::InitInstance(
	const char* appName,
	const char* engName,
	const std::vector<const char*>& requestedLayersValidation,
	const std::vector<const char*>& requestedExtension,
	VkDebugUtilsMessengerEXT& debugUtilsMessenger
)
{
	VkApplicationInfo applicationInfo;
	applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	applicationInfo.pApplicationName = appName;
	applicationInfo.pEngineName = engName;
	applicationInfo.pNext = nullptr;
	applicationInfo.apiVersion = VK_API_VERSION_1_0;
	applicationInfo.engineVersion = VK_MAKE_VERSION(0, 0, 1);
	applicationInfo.applicationVersion = VK_MAKE_VERSION(0, 0, 1);

	VkInstanceCreateInfo instanceCreateInfo;
	instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceCreateInfo.flags = 0;
	instanceCreateInfo.pNext = nullptr;
	instanceCreateInfo.pApplicationInfo = &applicationInfo;

	bool isDebuqReportExtensionRequired = false;

	if (Render::CheckRequestedInstanceExtensions(requestedExtension) && !requestedExtension.empty())
	{
		instanceCreateInfo.enabledExtensionCount = (uint32_t)requestedExtension.size();
		instanceCreateInfo.ppEnabledExtensionNames = requestedExtension.data();

		for (auto extension : requestedExtension)
		{
			if (strcmp(extension, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
			{
				isDebuqReportExtensionRequired = true;
				break;
			}
		}

		if (isDebuqReportExtensionRequired && !requestedLayersValidation.empty())
		{
			if (Render::CheckRequestedInstanceLayersValidation(requestedLayersValidation))
			{
				instanceCreateInfo.enabledLayerCount = (uint32_t)requestedLayersValidation.size();
				instanceCreateInfo.ppEnabledLayerNames = requestedLayersValidation.data();
			}
		}
	}

	VkInstance instance;

	if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS)
		FATAL_ERROR("Failed creation the Vulkan instance");

	if (isDebuqReportExtensionRequired)
	{
		VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT;
		debugUtilsMessengerCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugUtilsMessengerCreateInfoEXT.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;;
		debugUtilsMessengerCreateInfoEXT.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugUtilsMessengerCreateInfoEXT.flags = 0;
		debugUtilsMessengerCreateInfoEXT.pNext = nullptr;
		debugUtilsMessengerCreateInfoEXT.pfnUserCallback = Render::DebugUtilsMessengerCallback;

		auto vkCreateDebugUtilsMessengerEXT = 
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

		if (vkCreateDebugUtilsMessengerEXT(instance, &debugUtilsMessengerCreateInfoEXT, nullptr, &debugUtilsMessenger) != VK_SUCCESS)
			FATAL_ERROR("Failed creation the debug utils messenger");

		_Log("The debug utils messenger was successfully created");
	}

	_Log("The Vulkan instance was successfully created");

	return instance;
}

void EngineRenderers::Vulkan::Renderer::DeinitInstance(VkInstance& instance, VkDebugUtilsMessengerEXT& debugUtilsMessenger)
{
	if (debugUtilsMessenger != VK_NULL_HANDLE)
	{
		auto vkDestroyDebugUtilsMessengerEXT =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");

		vkDestroyDebugUtilsMessengerEXT(instance, debugUtilsMessenger, nullptr);
		debugUtilsMessenger = VK_NULL_HANDLE;

		_Log("The debug utils messenger was successfully destroyed");
	}

	if (instance != VK_NULL_HANDLE)
	{
		vkDestroyInstance(instance, nullptr);
		instance = VK_NULL_HANDLE;

		_Log("The Vulkan instance was successfully destroyed");
	}
}
#if WIN32

VkSurfaceKHR EngineRenderers::Vulkan::Renderer::InitWin32Surface(const VkInstance& instance, HINSTANCE hInstance, HWND hwnd)
{
	VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo;
	win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	win32SurfaceCreateInfo.pNext = nullptr;
	win32SurfaceCreateInfo.flags = 0;
	win32SurfaceCreateInfo.hwnd = hwnd;
	win32SurfaceCreateInfo.hinstance = hInstance;
	
	VkSurfaceKHR surface;
	auto vkCreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(instance, "vkCreateWin32SurfaceKHR");

	if (vkCreateWin32SurfaceKHR(instance, &win32SurfaceCreateInfo, nullptr, &surface) != VK_SUCCESS)
		FATAL_ERROR("The surface was not sucessfully creared");

	_Log("The surface was successfully creared");
	return surface;
}

void EngineRenderers::Vulkan::Renderer::DeinitWin32Surface(const VkInstance& instance, VkSurfaceKHR& surface)
{
	if (surface != VK_NULL_HANDLE)
	{
		vkDestroySurfaceKHR(instance, surface, nullptr);
		surface == VK_NULL_HANDLE;

		_Log("The surface was successfully destroyed");
	}
}

#endif // WIN32

Render::Device EngineRenderers::Vulkan::Renderer::InitDevice(
	const VkInstance& instance, 
	const VkSurfaceKHR& surface, 
	const std::vector<const char*>& requiredValidationLayers, 
	const std::vector<const char*>& requiredExtentions, 
	bool uniqueQueueFamilyIndexes)
{
	Render::Device device;

	uint32_t numberPhysicalDevice = 0;
	std::vector<VkPhysicalDevice> physicalDevices;
	vkEnumeratePhysicalDevices(instance, &numberPhysicalDevice, nullptr);

	if (numberPhysicalDevice == 0)
		FATAL_ERROR("Can't detect divice with Vulkan suppurt!");

	physicalDevices.resize(numberPhysicalDevice);
	vkEnumeratePhysicalDevices(instance, &numberPhysicalDevice, physicalDevices.data());

	for (auto physicalDevice : physicalDevices)
	{
		if (!Render::CheckRequestedDeviceExtensions(physicalDevice, requiredExtentions))
			break;

		if (!Render::CheckRequestedDeviceLayersValidation(physicalDevice, requiredValidationLayers))
			break;

		Render::QueueFamilyInfo queueFamilyInfo = Render::GetQueueFamilyInfo(physicalDevice, surface, uniqueQueueFamilyIndexes);

		if (!queueFamilyInfo.IsRenderingCompatible())
			break;

		device.SetPhysicalDevice(physicalDevice);
		device.SetQueueFamilyInfo(queueFamilyInfo);
		break;
	}

	if (device.GetPhysicalDevice() == VK_NULL_HANDLE)
		FATAL_ERROR("Can't detect suitable device!");

	std::vector<uint32_t> queueFamilies = {
		(uint32_t)device.GetQueueFamilyInfo().present,
	};

	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos(queueFamilies.size());

	float priority = 1.0f;

	for (uint32_t i = 0; i < queueFamilies.size(); ++i)
	{
		deviceQueueCreateInfos[i] = {};
		deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		deviceQueueCreateInfos[i].queueCount = 1;
		deviceQueueCreateInfos[i].queueFamilyIndex = queueFamilies[i];
		deviceQueueCreateInfos[i].pQueuePriorities = &priority;
	}
	VkPhysicalDeviceFeatures physicalDeviceFeatures = {};
	vkGetPhysicalDeviceFeatures(device.GetPhysicalDevice(), &physicalDeviceFeatures);

	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.enabledExtensionCount = (int32_t)requiredExtentions.size();
	deviceCreateInfo.ppEnabledExtensionNames = requiredExtentions.data();
	deviceCreateInfo.enabledLayerCount = (int32_t)requiredValidationLayers.size();
	deviceCreateInfo.ppEnabledLayerNames = requiredValidationLayers.data();
	deviceCreateInfo.pEnabledFeatures = &physicalDeviceFeatures;
	deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
	deviceCreateInfo.queueCreateInfoCount = (uint32_t)deviceQueueCreateInfos.size();

	VkDevice logicalDevice;
	if (vkCreateDevice(device.GetPhysicalDevice(), &deviceCreateInfo, nullptr, &logicalDevice) != VK_NULL_HANDLE)
		FATAL_ERROR("Failed to create logical device!");

	device.SetLogicalDevice(logicalDevice);

	VkQueue present;
	vkGetDeviceQueue(device.GetLogicalDevice(), device.GetQueueFamilyInfo().present, 0, &present);
	//vkGetDeviceQueue(device.GetLogicalDevice(), device.GetQueueFamilyInfo().graphics, 0, &graphics);

	//device.SetGraphicsQueue(graphics);
	device.SetPresentQueue(present);

	_Log("The device was successfully created");

	return device;
}

void EngineRenderers::Vulkan::Renderer::DeinitDevice(Render::Device& device)
{
	device.DeInit();

	_Log("The device was successfully destroyed");
}

Render::Swapchain EngineRenderers::Vulkan::Renderer::InitSwapchain(
	const Render::Device& device,
	const VkSurfaceKHR& surface, 
	const VkRenderPass& renderPass,
	const VkSurfaceFormatKHR& surfaceFormat,
	uint32_t imageCount,
	Render::Swapchain* oldSwapchain)
{
	Render::Swapchain swapchain;

	Render::SurfaceInfo surfaceInfo = Render::GetSurfaceInfo(device.GetPhysicalDevice(), surface);
	if (!surfaceInfo.IsSurfaceFormatSupported(surfaceFormat))
		FATAL_ERROR("The surface format don't support");

	if (imageCount > 0)
	{
		if (imageCount < surfaceInfo.capabilities.minImageCount || imageCount > surfaceInfo.capabilities.maxImageCount)
			FATAL_ERROR("The surface don't support " + std::to_string(imageCount) + " images/buffers in swap-chain");
	}
	else
	{
		imageCount = surfaceInfo.capabilities.minImageCount + 1 > surfaceInfo.capabilities.maxImageCount ? surfaceInfo.capabilities.maxImageCount : surfaceInfo.capabilities.minImageCount + 1;
	}

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = imageCount;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.preTransform = surfaceInfo.capabilities.currentTransform;
	swapchainCreateInfo.imageExtent = surfaceInfo.capabilities.currentExtent;
	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.imageFormat = surfaceFormat.format;
	swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapchainCreateInfo.clipped = VK_TRUE;

	if (oldSwapchain)
		swapchainCreateInfo.oldSwapchain = oldSwapchain->GetHandle();
		

	swapchain.SetImageFormat(swapchainCreateInfo.imageFormat);
	swapchain.SetImageExtent(swapchainCreateInfo.imageExtent);

	uint32_t queueFamilyIndeces[] =
	{
		(uint32_t)device.GetQueueFamilyInfo().present,
		(uint32_t)device.GetQueueFamilyInfo().graphics
	};

	swapchainCreateInfo.queueFamilyIndexCount = 0;
	swapchainCreateInfo.pQueueFamilyIndices = nullptr;//&queueFamilyIndeces[0];

	if (device.GetQueueFamilyInfo().graphics != device.GetQueueFamilyInfo().present && false)
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	}
	else
	{
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;

	if (imageCount > 1)
	{
		for (auto availablePresentMode : surfaceInfo.peresentModes)
		{
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				presentMode = availablePresentMode;
				break;
			}
		}
	}

	swapchainCreateInfo.presentMode = presentMode;

	VkSwapchainKHR swapchainKHR;

	if (vkCreateSwapchainKHR(device.GetLogicalDevice(), &swapchainCreateInfo, nullptr, &swapchainKHR) != VK_SUCCESS)
		FATAL_ERROR("Failed to create swapchain");

	if (oldSwapchain)
		delete oldSwapchain;

	uint32_t swapchainImageCount;
	vkGetSwapchainImagesKHR(device.GetLogicalDevice(), swapchainKHR, &swapchainImageCount, nullptr);

	if (swapchainImageCount > 0)
	{
		std::vector<VkImage> swapchainImages;
		std::vector<VkImageView> swapchainVkImageViews;
		swapchainImages.resize(swapchainImageCount);
		vkGetSwapchainImagesKHR(device.GetLogicalDevice(), swapchainKHR, &swapchainImageCount, swapchainImages.data());

		for (auto image : swapchainImages)
		{
			VkImageView imageView;

			VkImageViewCreateInfo imageViewCreateInfo = {};
			imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			imageViewCreateInfo.image = image;
			imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			imageViewCreateInfo.format = surfaceFormat.format;
			imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			imageViewCreateInfo.subresourceRange.levelCount = 1;
			imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			imageViewCreateInfo.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device.GetLogicalDevice(), &imageViewCreateInfo, nullptr, &imageView) != VK_SUCCESS)
				FATAL_ERROR("Failed to create image views");

			swapchainVkImageViews.push_back(imageView);
		}

		swapchain.SetImages(swapchainImages);
		swapchain.SetImageViews(swapchainVkImageViews);
	}

	std::vector<VkFramebuffer> framebuffers;
	for (const auto& imageView : swapchain.GetImageViews())
	{
		VkFramebuffer framebuffer;

		VkFramebufferCreateInfo framebufferCreateInfo = {};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = renderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &imageView;
		framebufferCreateInfo.width = swapchain.GetImageExtent().width;
		framebufferCreateInfo.height = swapchain.GetImageExtent().height;
		framebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(device.GetLogicalDevice(), &framebufferCreateInfo, nullptr, &framebuffer) != VK_SUCCESS)
			FATAL_ERROR("Failed to create framebuffers");

		framebuffers.push_back(framebuffer);
	}

	swapchain.SetFramebuffers(framebuffers);

	swapchain.SetHandle(swapchainKHR);

	_Log("The Swapchain successfully initialized");

	return swapchain;
}

void EngineRenderers::Vulkan::Renderer::DeinitSwapchain(Render::Swapchain& swapchain, const Render::Device& device)
{
	swapchain.Deinit(device.GetLogicalDevice());

	_Log("The Swapchain successfully destroyed");
}

VkCommandPool EngineRenderers::Vulkan::Renderer::InitCommandPool(const Render::Device& device, uint32_t queueFamilyIndex)
{
	VkCommandPool commandPool;

	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

	if (vkCreateCommandPool(device.GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &commandPool) != VK_SUCCESS)
		FATAL_ERROR("Failed to create command pool");

	_Log("The command pool was successfully created");

	return commandPool;
}

void EngineRenderers::Vulkan::Renderer::DeInitCommandPool(VkCommandPool& commandPool, const Render::Device& device)
{
	if (commandPool != VK_NULL_HANDLE)
	{
		vkDestroyCommandPool(device.GetLogicalDevice(), commandPool, nullptr);
		commandPool = VK_NULL_HANDLE;

		_Log("The command pool was successfully destroyed");
	}
}

std::vector<VkCommandBuffer> EngineRenderers::Vulkan::Renderer::InitCommandBuffers(const Render::Device& device, const VkCommandPool& commandPool, uint32_t count)
{
	std::vector<VkCommandBuffer> buffers(count);

	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	commandBufferAllocateInfo.commandBufferCount = count;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;

	if (vkAllocateCommandBuffers(device.GetLogicalDevice(), &commandBufferAllocateInfo, buffers.data()) != VK_SUCCESS)
		FATAL_ERROR("Failed to allocate command buffers");

	_Log("The command buffers was successfully allocated");

	return buffers;
}

void EngineRenderers::Vulkan::Renderer::DeInitCommandBuffers(std::vector<VkCommandBuffer>& buffers, const Render::Device& device, const VkCommandPool& commandPool)
{
	if (!buffers.empty())
	{
		vkFreeCommandBuffers(device.GetLogicalDevice(), commandPool, (uint32_t)buffers.size(), buffers.data());
		buffers.clear();

		_Log("The command buffers was successfully freed");
	}
}

VkPipelineLayout EngineRenderers::Vulkan::Renderer::InitPipelineLayout(const Render::Device& device, const std::vector<VkDescriptorSetLayout>& layouts, const std::vector<VkPushConstantRange>& constants)
{
	VkPipelineLayout pipelineLayout = VK_NULL_HANDLE;

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = (uint32_t)layouts.size();
	pipelineLayoutCreateInfo.pSetLayouts = layouts.data();
	pipelineLayoutCreateInfo.pushConstantRangeCount = (uint32_t)constants.size();
	pipelineLayoutCreateInfo.pPushConstantRanges = constants.data();

	VK_CHECK_ERROR(vkCreatePipelineLayout(device.GetLogicalDevice(), &pipelineLayoutCreateInfo, nullptr, &pipelineLayout), "");

	_Log("The pipeline layout successfully initialized");

	return pipelineLayout;
}

void EngineRenderers::Vulkan::Renderer::DeinitPipelineLayout(VkPipelineLayout& pipelineLayout, const Render::Device& device)
{
	if (pipelineLayout != VK_NULL_HANDLE)
	{
		vkDestroyPipelineLayout(device.GetLogicalDevice(), pipelineLayout, nullptr);
		pipelineLayout = VK_NULL_HANDLE;

		_Log("The pipeline layout successfully deinitialized");
	}
}

VkPipeline EngineRenderers::Vulkan::Renderer::InitGraphicsPipeline(const Render::Device& device, const Render::Swapchain& swapchain, const VkPipelineLayout& pipelineLayout, const VkRenderPass& renderPass)
{
	VkPipeline graphicsPipeline = VK_NULL_HANDLE;

	std::vector<VkVertexInputBindingDescription> bindingDescription =
	{
		{
			0,
			sizeof(Graphics::Vertex),
			VK_VERTEX_INPUT_RATE_VERTEX
		}
	};

	std::vector<VkVertexInputAttributeDescription> attributeDescriptions =
	{
		{
			0,
			0,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			offsetof(Graphics::Vertex, Vertex::position)
		},
		{
			1,
			0,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			offsetof(Graphics::Vertex, Vertex::color)
		}
	};

	VkPipelineVertexInputStateCreateInfo vertexInputState = {};
	vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputState.vertexBindingDescriptionCount = (uint32_t)bindingDescription.size();
	vertexInputState.pVertexBindingDescriptions = bindingDescription.data();
	vertexInputState.vertexAttributeDescriptionCount = (uint32_t)attributeDescriptions.size();
	vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
	inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages =
	{
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_VERTEX_BIT,
			Render::LoadShader(boost::filesystem::absolute("../../../Shaders/base.vert.spv").string().c_str(), device.GetLogicalDevice()),
			"main",
			nullptr
		},
		{
			VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
			nullptr,
			0,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			Render::LoadShader("../../../Shaders/base.frag.spv", device.GetLogicalDevice()),
			"main",
			nullptr
		},
	};

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float)swapchain.GetImageExtent().width;
	viewport.height = (float)swapchain.GetImageExtent().height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D rect2D;
	rect2D.offset = { 0, 0 };
	rect2D.extent = swapchain.GetImageExtent();

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &rect2D;

	VkPipelineRasterizationStateCreateInfo rasterizationStage = {};
	rasterizationStage.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStage.depthClampEnable = VK_FALSE;
	rasterizationStage.rasterizerDiscardEnable = VK_FALSE;
	rasterizationStage.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationStage.lineWidth = 1.0f;
	rasterizationStage.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationStage.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationStage.depthBiasEnable = VK_FALSE;
	rasterizationStage.depthBiasConstantFactor = 0.0f;
	rasterizationStage.depthBiasClamp = 0.0f;
	rasterizationStage.depthBiasSlopeFactor = 0.0f;

	VkPipelineDepthStencilStateCreateInfo depthStencilStage = {};
	depthStencilStage.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depthStencilStage.depthTestEnable = VK_TRUE;
	depthStencilStage.depthWriteEnable = VK_TRUE;
	depthStencilStage.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
	depthStencilStage.depthBoundsTestEnable = VK_FALSE;
	depthStencilStage.back.failOp = VK_STENCIL_OP_KEEP;
	depthStencilStage.back.passOp = VK_STENCIL_OP_KEEP;
	depthStencilStage.back.compareOp = VK_COMPARE_OP_ALWAYS;
	depthStencilStage.stencilTestEnable = VK_FALSE;
	depthStencilStage.front = depthStencilStage.back;

	VkPipelineMultisampleStateCreateInfo multisamplingStage = {};
	multisamplingStage.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisamplingStage.sampleShadingEnable = VK_FALSE;
	multisamplingStage.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisamplingStage.minSampleShading = 1.0f;
	multisamplingStage.pSampleMask = nullptr;
	multisamplingStage.alphaToCoverageEnable = VK_FALSE;
	multisamplingStage.alphaToOneEnable = VK_FALSE;

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_TRUE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	VkPipelineColorBlendStateCreateInfo colorBlendState = {};
	colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendState.logicOpEnable = VK_FALSE;
	colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	colorBlendState.attachmentCount = 1;
	colorBlendState.pAttachments = &colorBlendAttachment;

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = (uint32_t)shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();
	pipelineInfo.pVertexInputState = &vertexInputState;
	pipelineInfo.pInputAssemblyState = &inputAssemblyState;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizationStage;
	pipelineInfo.pDepthStencilState = &depthStencilStage;
	pipelineInfo.pMultisampleState = &multisamplingStage;
	pipelineInfo.pColorBlendState = &colorBlendState;
	pipelineInfo.layout = pipelineLayout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = 0;

	VK_CHECK_ERROR(vkCreateGraphicsPipelines(device.GetLogicalDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline), "Error while creating pipeline");

	for (auto& shader : shaderStages)
		vkDestroyShaderModule(device.GetLogicalDevice(), shader.module, nullptr);

	_Log("The pipeline sucessfully initialized");

	return graphicsPipeline;
}

void EngineRenderers::Vulkan::Renderer::DeinitGraphicsPipeline(VkPipeline& pipeline, const Render::Device& device)
{
	if (pipeline != VK_NULL_HANDLE)
	{
		vkDestroyPipeline(device.GetLogicalDevice(), pipeline, nullptr);
		pipeline = VK_NULL_HANDLE;

		_Log("The pipeline sucessfully deinitialized");
	}
}

VkDescriptorSetLayout EngineRenderers::Vulkan::Renderer::InitDescriptorSetLayout(const Render::Device& device, const std::vector<VkDescriptorSetLayoutBinding>& binding)
{
	VkDescriptorSetLayout layout = VK_NULL_HANDLE;

	VkDescriptorSetLayoutCreateInfo descriptorSetLayoutCreateInfo = {};
	descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSetLayoutCreateInfo.bindingCount = (uint32_t)binding.size();
	descriptorSetLayoutCreateInfo.pBindings = binding.data();

	VK_CHECK_ERROR(vkCreateDescriptorSetLayout(device.GetLogicalDevice(), &descriptorSetLayoutCreateInfo, nullptr, &layout), "");

	_Log("The descriptor set layout successfully initialized");

	return layout;
}

void EngineRenderers::Vulkan::Renderer::DeinitDescriptorSetLayout(VkDescriptorSetLayout& layout, const Render::Device& device)
{
	if (layout != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorSetLayout(device.GetLogicalDevice(), layout, nullptr);
		layout = VK_NULL_HANDLE;

		_Log("The descriptor set layout successfully deinitialized");
	}
}

VkDescriptorPool EngineRenderers::Vulkan::Renderer::InitDescriptorPool(const Render::Device& device, const std::vector<VkDescriptorPoolSize> descriptorPoolSizes)
{
	VkDescriptorPool descriptorPool = VK_NULL_HANDLE;

	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo = {};
	descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
	descriptorPoolCreateInfo.poolSizeCount = (uint32_t)descriptorPoolSizes.size();
	descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
	descriptorPoolCreateInfo.maxSets = 1;

	VK_CHECK_ERROR(vkCreateDescriptorPool(device.GetLogicalDevice(), &descriptorPoolCreateInfo, nullptr, &descriptorPool), "");

	_Log("The descriptor pool successfully initialized");
	
	return descriptorPool;
}

void EngineRenderers::Vulkan::Renderer::DeinitDescriptorPool(VkDescriptorPool& descriptorPool, const Render::Device& device)
{
	if (descriptorPool != VK_NULL_HANDLE)
	{
		vkDestroyDescriptorPool(device.GetLogicalDevice(), descriptorPool, nullptr);
		descriptorPool = VK_NULL_HANDLE;

		_Log("The descriptor pool successfully deinitialized");
	}
}

VkDescriptorSet EngineRenderers::Vulkan::Renderer::InitDescriptorSet(
	const Render::Device& device, 
	const VkDescriptorPool& descriptorPool, 
	const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts, 
	const std::vector<Render::UniformBuffer*>& buffers
)
{
	VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

	VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
	descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descriptorSetAllocateInfo.pSetLayouts = descriptorSetLayouts.data();
	descriptorSetAllocateInfo.descriptorSetCount = 1;
	descriptorSetAllocateInfo.descriptorPool = descriptorPool;

	VK_CHECK_ERROR(vkAllocateDescriptorSets(device.GetLogicalDevice(), &descriptorSetAllocateInfo, &descriptorSet), "");

	std::vector<VkWriteDescriptorSet> writes;

	for (auto& buffer : buffers)
	{
		VkWriteDescriptorSet writeDescriptorSet = {};
		writeDescriptorSet.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptorSet.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER; // variable
		writeDescriptorSet.descriptorCount = 1; // variable
		writeDescriptorSet.dstSet = descriptorSet;
		writeDescriptorSet.dstBinding = 0;
		writeDescriptorSet.dstArrayElement = 0;
		writeDescriptorSet.pBufferInfo = &buffer->GetDescriptorBufferInfo();

		writes.push_back(writeDescriptorSet);
	};

	vkUpdateDescriptorSets(device.GetLogicalDevice(), (uint32_t)writes.size(), writes.data(), 0, nullptr);

	_Log("The descriptor set successfully initialized");

	return descriptorSet;
}

void EngineRenderers::Vulkan::Renderer::DeinitDescriptorSet(VkDescriptorSet& descriptorSet, const VkDescriptorPool& descriptorPool, const Render::Device& device)
{
	if (descriptorSet != VK_NULL_HANDLE)
	{
		VK_CHECK_ERROR(vkFreeDescriptorSets(device.GetLogicalDevice(), descriptorPool, 1, &descriptorSet), "");
		descriptorSet = VK_NULL_HANDLE;

		_Log("The descriptor set successfully deinitialized");
	}
}

VkRenderPass EngineRenderers::Vulkan::Renderer::InitRenderPass(
	const Render::Device& device,
	const VkSurfaceKHR& surface,
	const VkFormat& colorAttachmentFormat,
	const VkFormat& depthStencilFormat)
{
	std::vector<VkAttachmentDescription> attachments;

	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = colorAttachmentFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	attachments.push_back(colorAttachment);

	/*VkAttachmentDescription depthStencilAttachment = {};
	depthStencilAttachment.format = depthStencilFormat;
	depthStencilAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthStencilAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthStencilAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	depthStencilAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthStencilAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthStencilAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthStencilAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	attachments.push_back(depthStencilAttachment);*/

	std::vector<VkAttachmentReference> colorAttachmentReferences = {
		{ 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL }
	};

	/*VkAttachmentReference depthStencilAttachemntReference = {
		1,
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
	};*/

	VkSubpassDescription subpassDescription = {};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpassDescription.colorAttachmentCount = (int32_t)colorAttachmentReferences.size();
	subpassDescription.pColorAttachments = colorAttachmentReferences.data();
	//subpassDescription.pDepthStencilAttachment = &depthStencilAttachemntReference;

	std::vector<VkSubpassDependency> dependencies = {
		{
			VK_SUBPASS_EXTERNAL,
			0,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_DEPENDENCY_BY_REGION_BIT
		}/*,
		{
			0,
			VK_SUBPASS_EXTERNAL,
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
			VK_ACCESS_MEMORY_READ_BIT,
			VK_DEPENDENCY_BY_REGION_BIT
		}*/
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.pAttachments = attachments.data();
	renderPassCreateInfo.attachmentCount = (uint32_t)attachments.size();
	//renderPassCreateInfo.pDependencies = dependencies.data();
	//renderPassCreateInfo.dependencyCount = (uint32_t)dependencies.size();
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.subpassCount = 1;

	VkRenderPass renderPass;
	if (vkCreateRenderPass(device.GetLogicalDevice(), &renderPassCreateInfo, nullptr, &renderPass) != VK_SUCCESS)
		FATAL_ERROR("Failed to create render pass!");

	_Log("The render pass was successfully created");

	return renderPass;
}

void EngineRenderers::Vulkan::Renderer::DeinitRenderPass(VkRenderPass& renderPass, const Render::Device& device)
{
	if (renderPass != VK_NULL_HANDLE)
	{
		vkDestroyRenderPass(device.GetLogicalDevice(), renderPass, nullptr);
		renderPass = VK_NULL_HANDLE;

		_Log("The render pass was successfully destoyed");
	}
}
