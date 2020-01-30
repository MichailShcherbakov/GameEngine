#ifndef _RENDERER_VULKAN_H_
#define _RENDERER_VULKAN_H_

#include <vector>

#include "Toolkit/Toolkit.h"
#include "Device/Device.h"
#include "Buffer/Buffer.h"

#include "vulkan/vulkan.h"
#include "glm.hpp"

#if WIN32
	#include <Windows.h>
	#include <vulkan/vulkan_win32.h>
#endif

namespace EngineRenderers
{
	namespace Vulkan
	{
		class Renderer
		{
		public:
#if WIN32
			//Renderer(HINSTANCE hInstance, HWND hwnd);
#endif // WIN32
			~Renderer();

		public:
			static VkInstance InitInstance(
				const char* appName,
				const char* engName,
				const std::vector<const char*>& requestedLayersValidation,
				const std::vector<const char*>& requestedExtension,
				VkDebugUtilsMessengerEXT& debugUtilsMessenger
			);

			static void DeinitInstance(
				VkInstance& instance,
				VkDebugUtilsMessengerEXT& debugUtilsMessenger
			);

#if WIN32
			static VkSurfaceKHR InitWin32Surface(
				const VkInstance& instance,
				HINSTANCE hInstance,
				HWND hwnd
			);

			static void DeinitWin32Surface(
				const VkInstance& instance,
				VkSurfaceKHR& surface
			);
#endif // WIN32

			static EngineRenderers::Vulkan::Device InitDevice(
				const VkInstance& instance,
				const VkSurfaceKHR& surface,
				const std::vector<const char*>& requestedLayersValidation,
				const std::vector<const char*>& requestedExtension,
				bool uniqueQueueFamilyIndexes = true
			);

			static void DeinitDevice(
				EngineRenderers::Vulkan::Device& device
			);

			static VkRenderPass InitRenderPass(
				const EngineRenderers::Vulkan::Device& device,
				const VkSurfaceKHR& surface,
				const VkFormat& colorAttachmentFormat,
				const VkFormat& depthStencilFormat
			);

			static void DeinitRenderPass(
				VkRenderPass& renderPass,
				const EngineRenderers::Vulkan::Device& device
			);

			static EngineRenderers::Vulkan::Swapchain InitSwapchain(
				const EngineRenderers::Vulkan::Device& device,
				const VkSurfaceKHR& surface,
				const VkRenderPass& renderPass,
				const VkSurfaceFormatKHR& surfaceFormat,
				uint32_t imageCount,
				EngineRenderers::Vulkan::Swapchain* oldSwapchain = nullptr
			);

			static void DeinitSwapchain(
				EngineRenderers::Vulkan::Swapchain& swapchain,
				const EngineRenderers::Vulkan::Device& device
			);

			static VkCommandPool InitCommandPool(
				const EngineRenderers::Vulkan::Device& device,
				uint32_t queueFamilyIndex
			);

			static void DeInitCommandPool(
				VkCommandPool& commandPool,
				const EngineRenderers::Vulkan::Device& device
			);

			static std::vector<VkCommandBuffer> InitCommandBuffers(
				const EngineRenderers::Vulkan::Device& device,
				const VkCommandPool& commandPool,
				uint32_t count
			);

			static void DeInitCommandBuffers(
				std::vector<VkCommandBuffer>& buffers,
				const EngineRenderers::Vulkan::Device& device,
				const VkCommandPool& commandPool
			);

			static VkPipelineLayout InitPipelineLayout(
				const EngineRenderers::Vulkan::Device& device,
				const std::vector<VkDescriptorSetLayout>& layouts,
				const std::vector<VkPushConstantRange>& constants
			);

			static void DeinitPipelineLayout(
				VkPipelineLayout& pipelineLayout,
				const EngineRenderers::Vulkan::Device& device
			);

			static VkPipeline InitGraphicsPipeline(
				const EngineRenderers::Vulkan::Device& device,
				const EngineRenderers::Vulkan::Swapchain& swapchain,
				const VkPipelineLayout& pipelineLayout,
				const VkRenderPass& renderPass
			);

			static void DeinitGraphicsPipeline(
				VkPipeline& pipeline,
				const EngineRenderers::Vulkan::Device& device
			);

			static VkDescriptorSetLayout InitDescriptorSetLayout(
				const EngineRenderers::Vulkan::Device& device,
				const std::vector<VkDescriptorSetLayoutBinding>& binding
			);

			static void DeinitDescriptorSetLayout(
				VkDescriptorSetLayout& layout,
				const EngineRenderers::Vulkan::Device& device
			);

			static VkDescriptorPool InitDescriptorPool(
				const EngineRenderers::Vulkan::Device& device,
				const std::vector<VkDescriptorPoolSize> descriptorPoolSizes
			);

			static void DeinitDescriptorPool(
				VkDescriptorPool& descriptorPool,
				const EngineRenderers::Vulkan::Device& device
			);

			static VkDescriptorSet InitDescriptorSet(
				const EngineRenderers::Vulkan::Device& device,
				const VkDescriptorPool& descriptorPool,
				const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
				const std::vector<EngineRenderers::Vulkan::UniformBuffer*>& buffers
			);

			static void DeinitDescriptorSet(
				VkDescriptorSet& descriptorSet,
				const VkDescriptorPool& descriptorPool,
				const EngineRenderers::Vulkan::Device& device);

		private:
			/*VkInstance m_instance;
			VkSurfaceKHR m_surface;
			VkRenderPass m_renderPass;
			VkPipelineLayout m_pipelineLayout;
			VkPipeline m_graphicsPipeline;
			VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT;
			Render::Swapchain m_swapchain;
			Render::Device m_device;*/
		};
	}
}

#endif // _RENDERER_VULKAN_H_

