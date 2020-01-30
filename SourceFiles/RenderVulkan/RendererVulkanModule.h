#ifndef _RENDERER_VULKAN_MODULE_H_
#define _RENDERER_VULKAN_MODULE_H_

#include <Engine/Engine.h>
#include "Renderer/RendererVulkan.h"

class RendererVulkanModule : public IEngineModule
{
public:
	RendererVulkanModule();
	~RendererVulkanModule() override;

public:
	// IEngineModule
	virtual void Initialize()  override;
	virtual void Update()  override;
	virtual void Release() override;
	virtual const char* GetModuleName() { return "Engine.RenderVulkan"; }
	virtual void RegisterEngineModule(const IInitializeParams& params)  override;
	virtual void OnEngineEvent(const EEngineEvent& event) override;
	// ~IEngineModule

private:
	EngineRenderers::Vulkan::Renderer m_renderer;

private:
	VkInstance m_instance = VK_NULL_HANDLE;
	VkSurfaceKHR m_surface = VK_NULL_HANDLE;
	VkRenderPass m_renderPass = VK_NULL_HANDLE;
	VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
	VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
	VkPipelineLayout m_pipelineLayout = VK_NULL_HANDLE;
	VkPipeline m_graphicsPipeline = VK_NULL_HANDLE;
	VkCommandPool m_commandPool = VK_NULL_HANDLE;
	VkSemaphore m_readyToPresent = VK_NULL_HANDLE;
	VkFence m_fence = VK_NULL_HANDLE;
	VkDescriptorSet m_descriptorSet = VK_NULL_HANDLE;
	EngineRenderers::Vulkan::Buffer* m_vertexBuffer;
	EngineRenderers::Vulkan::UniformBuffer* m_colorUniformBuffer;
	VkDebugUtilsMessengerEXT m_debugUtilsMessenger = VK_NULL_HANDLE;
	EngineRenderers::Vulkan::Device m_device;
	EngineRenderers::Vulkan::Swapchain m_swapchain;
	std::vector<VkCommandBuffer> m_buffers;
};

REGISTER_ENGINE_MODULE(RendererVulkanModule)

#endif // _RENDERER_VULKAN_MODULE_H_