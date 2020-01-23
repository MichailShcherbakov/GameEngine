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
	#include "vulkan/vulkan_win32.h"
#endif

namespace EngineRenderers
{
	namespace Vulkan
	{
		class Renderer
		{
		public:
#if WIN32
			Renderer(HINSTANCE hInstance, HWND hwnd);
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

			static Render::Device InitDevice(
				const VkInstance& instance,
				const VkSurfaceKHR& surface,
				const std::vector<const char*>& requestedLayersValidation,
				const std::vector<const char*>& requestedExtension,
				bool uniqueQueueFamilyIndexes = true
			);

			static void DeinitDevice(
				Render::Device& device
			);

			static VkRenderPass InitRenderPass(
				const Render::Device& device,
				const VkSurfaceKHR& surface,
				const VkFormat& colorAttachmentFormat,
				const VkFormat& depthStencilFormat
			);

			static void DeinitRenderPass(
				VkRenderPass& renderPass,
				const Render::Device& device
			);

			static Render::Swapchain InitSwapchain(
				const Render::Device& device,
				const VkSurfaceKHR& surface,
				const VkRenderPass& renderPass,
				const VkSurfaceFormatKHR& surfaceFormat,
				uint32_t imageCount,
				Render::Swapchain* oldSwapchain = nullptr
			);

			static void DeinitSwapchain(
				Render::Swapchain& swapchain,
				const Render::Device& device
			);

			static VkCommandPool InitCommandPool(
				const Render::Device& device,
				uint32_t queueFamilyIndex
			);

			static void DeInitCommandPool(
				VkCommandPool& commandPool,
				const Render::Device& device
			);

			static std::vector<VkCommandBuffer> InitCommandBuffers(
				const Render::Device& device,
				const VkCommandPool& commandPool,
				uint32_t count
			);

			static void DeInitCommandBuffers(
				std::vector<VkCommandBuffer>& buffers,
				const Render::Device& device,
				const VkCommandPool& commandPool
			);

			static VkPipelineLayout InitPipelineLayout(
				const Render::Device& device,
				const std::vector<VkDescriptorSetLayout>& layouts,
				const std::vector<VkPushConstantRange>& constants
			);

			static void DeinitPipelineLayout(
				VkPipelineLayout& pipelineLayout,
				const Render::Device& device
			);

			static VkPipeline InitGraphicsPipeline(
				const Render::Device& device,
				const Render::Swapchain& swapchain,
				const VkPipelineLayout& pipelineLayout,
				const VkRenderPass& renderPass
			);

			static void DeinitGraphicsPipeline(
				VkPipeline& pipeline,
				const Render::Device& device
			);

			static VkDescriptorSetLayout InitDescriptorSetLayout(
				const Render::Device& device,
				const std::vector<VkDescriptorSetLayoutBinding>& binding
			);

			static void DeinitDescriptorSetLayout(
				VkDescriptorSetLayout& layout,
				const Render::Device& device
			);

			static VkDescriptorPool InitDescriptorPool(
				const Render::Device& device,
				const std::vector<VkDescriptorPoolSize> descriptorPoolSizes
			);

			static void DeinitDescriptorPool(
				VkDescriptorPool& descriptorPool,
				const Render::Device& device
			);

			static VkDescriptorSet InitDescriptorSet(
				const Render::Device& device,
				const VkDescriptorPool& descriptorPool,
				const std::vector<VkDescriptorSetLayout>& descriptorSetLayouts,
				const std::vector<Render::UniformBuffer*>& buffers
			);

			static void DeinitDescriptorSet(
				VkDescriptorSet& descriptorSet,
				const VkDescriptorPool& descriptorPool,
				const Render::Device& device);

		private:
			/*VkInstance m_instance;
			static VkSurfaceKHR m_surface;
			static VkRenderPass m_renderPass;
			static VkPipelineLayout m_pipelineLayout;
			static VkPipeline m_graphicsPipeline;
			static VkDebugUtilsMessengerEXT m_debugUtilsMessengerEXT;
			static Render::Swapchain m_swapchain;
			static Render::Device m_device;*/
		};
	}
}

#endif // _RENDERER_VULKAN_H_

