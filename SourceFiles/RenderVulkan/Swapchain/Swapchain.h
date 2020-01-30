#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "vulkan/vulkan.h"
#include <vector>

namespace EngineRenderers
{
	namespace Vulkan
	{
		struct SurfaceInfo
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> peresentModes;

			bool IsSurfaceFormatSupported(const VkSurfaceFormatKHR& surfaceFormat);
			bool IsFormatSupporred(const VkFormat& format);
			bool IsColorSpaceSupported(const VkColorSpaceKHR& colorSpace);
		};

		class Swapchain
		{
		public:
			Swapchain();
			~Swapchain();

		public:
			void Deinit(const VkDevice& device);

		public:
			void SetImages(const std::vector<VkImage>& images);
			void SetImageViews(const std::vector<VkImageView>& imageViews);
			void SetFramebuffers(const std::vector<VkFramebuffer>& framebuffers);
			void SetHandle(const VkSwapchainKHR& swapchain);
			void SetImageFormat(const VkFormat& imageFormat);
			void SetImageExtent(const VkExtent2D& imageExtent);

		public:
			std::vector<VkImage> GetImages() const;
			std::vector<VkImageView> GetImageViews() const;
			std::vector<VkFramebuffer> GetFramebuffers() const;
			VkSwapchainKHR GetHandle() const;
			VkFormat GetImageFormat() const;
			VkExtent2D GetImageExtent() const;

		private:
			std::vector<VkImage> m_images;
			std::vector<VkImageView> m_imageViews;
			std::vector<VkFramebuffer> m_framebuffers;
			VkSwapchainKHR m_swapchain;
			VkFormat m_imageFormat;
			VkExtent2D m_imageExtent;
		};
	}
}

#endif // _DEVICE_H_