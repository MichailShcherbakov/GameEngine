#include "StdAfx.h"
#include "Swapchain.h"

bool Render::SurfaceInfo::IsSurfaceFormatSupported(const VkSurfaceFormatKHR& surfaceFormat)
{
	return IsFormatSupporred(surfaceFormat.format) && IsColorSpaceSupported(surfaceFormat.colorSpace);
}

bool Render::SurfaceInfo::IsFormatSupporred(const VkFormat& format)
{
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		return true;
	else if (formats.size() > 1)
	{
		for (auto formatEntry : formats)
		{
			if (formatEntry.format == format)
				return true;
		}
	}

	return false;
}

bool Render::SurfaceInfo::IsColorSpaceSupported(const VkColorSpaceKHR& colorSpace)
{
	if (formats.size() == 1 && formats[0].format == VK_FORMAT_UNDEFINED)
		return true;
	else if (formats.size() > 1)
	{
		for (auto formatEntry : formats)
		{
			if (formatEntry.colorSpace == colorSpace)
				return true;
		}
	}

	return false;
}

Render::Swapchain::Swapchain()
{
}

Render::Swapchain::~Swapchain()
{
}

void Render::Swapchain::Deinit(const VkDevice& device)
{
	if (!m_framebuffers.empty())
	{
		for (auto const& framebuffer : m_framebuffers)
			vkDestroyFramebuffer(device, framebuffer, nullptr);

		m_framebuffers.clear();
	}

	if (!m_imageViews.empty())
	{
		for (auto const& imageView : m_imageViews)
			vkDestroyImageView(device, imageView, nullptr);

		m_imageViews.clear();
	}

	if (m_swapchain != VK_NULL_HANDLE)
	{
		vkDestroySwapchainKHR(device, m_swapchain, nullptr);
		m_swapchain = VK_NULL_HANDLE;
	}
}

void Render::Swapchain::SetImages(const std::vector<VkImage>& images)
{
	m_images = images;
}

void Render::Swapchain::SetImageViews(const std::vector<VkImageView>& imageViews)
{
	m_imageViews = imageViews;
}

void Render::Swapchain::SetFramebuffers(const std::vector<VkFramebuffer>& framebuffers)
{
	m_framebuffers = framebuffers;
}

void Render::Swapchain::SetHandle(const VkSwapchainKHR& swapchain)
{
	m_swapchain = swapchain;
}

void Render::Swapchain::SetImageFormat(const VkFormat& imageFormat)
{
	m_imageFormat = imageFormat;
}

void Render::Swapchain::SetImageExtent(const VkExtent2D& imageExtent)
{
	m_imageExtent = imageExtent;
}

std::vector<VkImage> Render::Swapchain::GetImages() const
{
	return m_images;
}

std::vector<VkImageView> Render::Swapchain::GetImageViews() const
{
	return m_imageViews;
}

std::vector<VkFramebuffer> Render::Swapchain::GetFramebuffers() const
{
	return m_framebuffers;
}

VkSwapchainKHR Render::Swapchain::GetHandle() const
{
	return m_swapchain;
}

VkFormat Render::Swapchain::GetImageFormat() const
{
	return m_imageFormat;
}

VkExtent2D Render::Swapchain::GetImageExtent() const
{
	return m_imageExtent;
}
