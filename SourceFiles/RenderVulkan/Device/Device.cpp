#include "StdAfx.h"
#include "Device.h"
#include <vector>

EngineRenderers::Vulkan::Device::Device()
{
}

EngineRenderers::Vulkan::Device::~Device()
{
}

void EngineRenderers::Vulkan::Device::DeInit()
{
	if (m_logicalDevice != VK_NULL_HANDLE)
		vkDestroyDevice(m_logicalDevice, nullptr);

	m_physicalDevice = VK_NULL_HANDLE;
	m_logicalDevice = VK_NULL_HANDLE;
	m_graphicsQueue = VK_NULL_HANDLE;
	m_presentQueue = VK_NULL_HANDLE;
	m_queueFamilyInfo = {};
}

void EngineRenderers::Vulkan::Device::SetPhysicalDevice(const VkPhysicalDevice& physicalDevice)
{
	m_physicalDevice = physicalDevice;
}

const VkPhysicalDevice& EngineRenderers::Vulkan::Device::GetPhysicalDevice() const
{
	return m_physicalDevice;
}

void EngineRenderers::Vulkan::Device::SetLogicalDevice(const VkDevice& logicalDevice)
{
	m_logicalDevice = logicalDevice;
}

const VkDevice& EngineRenderers::Vulkan::Device::GetLogicalDevice() const
{
	return m_logicalDevice;
}

void EngineRenderers::Vulkan::Device::SetQueueFamilyInfo(const EngineRenderers::Vulkan::QueueFamilyInfo& queueFamilyInfo)
{
	m_queueFamilyInfo = queueFamilyInfo;
}

void EngineRenderers::Vulkan::Device::SetGraphicsQueue(const VkQueue& queue)
{
	m_graphicsQueue = queue;
}

void EngineRenderers::Vulkan::Device::SetPresentQueue(const VkQueue& queue)
{
	m_presentQueue = queue;
}

const EngineRenderers::Vulkan::QueueFamilyInfo& EngineRenderers::Vulkan::Device::GetQueueFamilyInfo() const
{
	return m_queueFamilyInfo;
}

const VkQueue& EngineRenderers::Vulkan::Device::GetGraphicsQueue() const
{
	return m_graphicsQueue;
}

const VkQueue& EngineRenderers::Vulkan::Device::GetPresentQueue() const
{
	return m_presentQueue;
}

