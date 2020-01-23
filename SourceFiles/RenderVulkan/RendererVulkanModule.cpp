#include "StdAfx.h"
#include "RendererVulkanModule.h"

#include <Graphics/Surface/ISurface.h>

#include "Graphics/Mesh/Mesh.h"

RendererVulkanModule::RendererVulkanModule()
{
}

RendererVulkanModule::~RendererVulkanModule()
{
	gEngine->pCore->OnEngineEvent -= LISTENER(*this, RendererVulkanModule::OnEngineEvent);

	vkDeviceWaitIdle(m_device.GetLogicalDevice());
	vkDestroyFence(m_device.GetLogicalDevice(), m_fence, nullptr);
	vkDestroySemaphore(m_device.GetLogicalDevice(), m_readyToPresent, nullptr);
	EngineRenderers::Vulkan::Renderer::DeInitCommandBuffers(m_buffers, m_device, m_commandPool);
	EngineRenderers::Vulkan::Renderer::DeInitCommandPool(m_commandPool, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitDescriptorSet(m_descriptorSet, m_descriptorPool, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitGraphicsPipeline(m_graphicsPipeline, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitPipelineLayout(m_pipelineLayout, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitDescriptorPool(m_descriptorPool, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitDescriptorSetLayout(m_layout, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitSwapchain(m_swapchain, m_device);
	EngineRenderers::Vulkan::Renderer::DeinitRenderPass(m_renderPass, m_device);
	delete m_colorUniformBuffer;
	delete m_vertexBuffer;
	EngineRenderers::Vulkan::Renderer::DeinitDevice(m_device);
	EngineRenderers::Vulkan::Renderer::DeinitWin32Surface(m_instance, m_surface);
	EngineRenderers::Vulkan::Renderer::DeinitInstance(m_instance, m_debugUtilsMessenger);
}

void RendererVulkanModule::Initialize()
{
	m_instance = EngineRenderers::Vulkan::Renderer::InitInstance(
		"Union",
		"UnionEngine",
		{ "VK_LAYER_LUNARG_standard_validation" },
		{ VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME },
		m_debugUtilsMessenger
	);

#if WIN32
	m_surface = EngineRenderers::Vulkan::Renderer::InitWin32Surface(m_instance, GetModuleHandle(nullptr), gEngine->pSurface->GetHandle());
#endif

	m_device = EngineRenderers::Vulkan::Renderer::InitDevice(
		m_instance,
		m_surface,
		{ "VK_LAYER_LUNARG_standard_validation" },
		{ VK_KHR_SWAPCHAIN_EXTENSION_NAME },
		false
	);

	m_renderPass = EngineRenderers::Vulkan::Renderer::InitRenderPass(
		m_device,
		m_surface,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_D32_SFLOAT_S8_UINT
	);

	m_swapchain = EngineRenderers::Vulkan::Renderer::InitSwapchain(
		m_device,
		m_surface,
		m_renderPass,
		{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
		1
	);

	std::vector<VkDescriptorSetLayoutBinding> layoutBinding =
	{
		{
			0,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			nullptr
		}
	};

	m_layout = EngineRenderers::Vulkan::Renderer::InitDescriptorSetLayout(m_device, layoutBinding);

	std::vector<VkDescriptorPoolSize> layoutPoolSize =
	{
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
		}
	};

	m_descriptorPool = EngineRenderers::Vulkan::Renderer::InitDescriptorPool(m_device, layoutPoolSize);

	m_pipelineLayout = EngineRenderers::Vulkan::Renderer::InitPipelineLayout(m_device, { m_layout }, {});

	m_graphicsPipeline = EngineRenderers::Vulkan::Renderer::InitGraphicsPipeline(m_device, m_swapchain, m_pipelineLayout, m_renderPass);

	m_commandPool = EngineRenderers::Vulkan::Renderer::InitCommandPool(m_device, m_device.GetQueueFamilyInfo().present);
	m_buffers = EngineRenderers::Vulkan::Renderer::InitCommandBuffers(m_device, m_commandPool, 1);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkRect2D renderArea;
	renderArea.offset.x = 0;
	renderArea.offset.y = 0;
	renderArea.extent = m_swapchain.GetImageExtent();





	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VK_CHECK_ERROR(vkCreateSemaphore(m_device.GetLogicalDevice(), &semaphore_create_info, nullptr, &m_readyToPresent), "");

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;


	VK_CHECK_ERROR(vkCreateFence(m_device.GetLogicalDevice(), &fenceInfo, nullptr, &m_fence), "");

	std::vector<Graphics::Vertex> vertices =
	{
		{ glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), },
		{ glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), },
		{ glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), },
	};

	uint64_t size = (uint64_t)(vertices.size() * sizeof(Graphics::Vertex));
	m_vertexBuffer = new Render::Buffer(
		m_device,
		size,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);

	m_vertexBuffer->Map(size, 0);
	m_vertexBuffer->MemoryCopy(vertices.data(), (size_t)(vertices.size() * sizeof(Graphics::Vertex)));
	m_vertexBuffer->UnMap();



	glm::vec3 mainColor = { 1.0f, 0.0f, 0.0f };
	size = (uint64_t)sizeof(mainColor);

	m_colorUniformBuffer = new Render::UniformBuffer(
		m_device,
		size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);

	m_colorUniformBuffer->Map(size, 0);
	m_colorUniformBuffer->MemoryCopy(&mainColor, size);
	m_colorUniformBuffer->UnMap();

	m_descriptorSet = EngineRenderers::Vulkan::Renderer::InitDescriptorSet(m_device, m_descriptorPool, { m_layout }, { m_colorUniformBuffer });




	VK_CHECK_ERROR(vkBeginCommandBuffer(m_buffers[0], &commandBufferBeginInfo), "");

	VkClearValue clearValue;
	clearValue.color = { 0.1f, 0.1f, 0.1f, 1.0f };

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.framebuffer = m_swapchain.GetFramebuffers()[0];
	renderPassBeginInfo.renderPass = m_renderPass;
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(m_buffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(m_buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);

	VkDeviceSize offsets[1] = { 0 };
	VkBuffer vertexBuffers = m_vertexBuffer->GetHandle();
	vkCmdBindVertexBuffers(m_buffers[0], 0, 1, &vertexBuffers, offsets);

	vkCmdBindDescriptorSets(m_buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayout, 0, 1, &m_descriptorSet, 0, nullptr);

	vkCmdDraw(m_buffers[0], (uint32_t)vertices.size(), 1, 0, 0);

	vkCmdEndRenderPass(m_buffers[0]);

	VK_CHECK_ERROR(vkEndCommandBuffer(m_buffers[0]), "");
}

void RendererVulkanModule::Update()
{
	uint32_t imageIndex = UINT16_MAX;

	VK_CHECK_ERROR(vkAcquireNextImageKHR(m_device.GetLogicalDevice(), m_swapchain.GetHandle(), UINT64_MAX, VK_NULL_HANDLE, m_fence, &imageIndex), "Error");
	VK_CHECK_ERROR(vkWaitForFences(m_device.GetLogicalDevice(), 1, &m_fence, VK_TRUE, UINT64_MAX), "");
	VK_CHECK_ERROR(vkResetFences(m_device.GetLogicalDevice(), 1, &m_fence), "");
	VK_CHECK_ERROR(vkQueueWaitIdle(m_device.GetPresentQueue()), "");

	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	VkSubmitInfo submit_info = {};
	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.waitSemaphoreCount = 0;
	submit_info.pWaitSemaphores = nullptr;
	submit_info.pWaitDstStageMask = waitStages;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &m_buffers[0];
	submit_info.signalSemaphoreCount = 1;
	submit_info.pSignalSemaphores = &m_readyToPresent;

	VK_CHECK_ERROR(vkQueueSubmit(m_device.GetPresentQueue(), 1, &submit_info, VK_NULL_HANDLE), "");

	VkResult result = VkResult::VK_RESULT_MAX_ENUM;

	VkSwapchainKHR s = m_swapchain.GetHandle();

	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &m_readyToPresent;
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &s;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = &result;

	VK_CHECK_ERROR(vkQueuePresentKHR(m_device.GetPresentQueue(), &presentInfo), "");
	VK_CHECK_ERROR(result, "");
}

void RendererVulkanModule::Release()
{
	_Log("Releasing the module...", GetModuleName());

	this->~RendererVulkanModule();
}

void RendererVulkanModule::RegisterEngineModule(const IInitializeParams& params)
{
	gEngine = params.gEngine;

	gEngine->pCore->OnEngineEvent += LISTENER(*this, RendererVulkanModule::OnEngineEvent);

	_LogSuccess("Module has been loaded.", GetModuleName());
}

void RendererVulkanModule::OnEngineEvent(const EEngineEvent& event)
{
	switch (event)
	{
	case EEngineEvent::EVENT_ENGINE_SURFACE_INIT_COMPLETE:
		Initialize();
		break;
	}
}
