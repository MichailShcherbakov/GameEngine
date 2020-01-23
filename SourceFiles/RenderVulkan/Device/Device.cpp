#include "StdAfx.h"
#include "Device.h"
#include <vector>

Render::Device::Device()
{
}

Render::Device::~Device()
{
}

void Render::Device::DeInit()
{
	if (m_logicalDevice != VK_NULL_HANDLE)
		vkDestroyDevice(m_logicalDevice, nullptr);

	m_physicalDevice = VK_NULL_HANDLE;
	m_logicalDevice = VK_NULL_HANDLE;
	m_graphicsQueue = VK_NULL_HANDLE;
	m_presentQueue = VK_NULL_HANDLE;
	m_queueFamilyInfo = {};
}

void Render::Device::SetPhysicalDevice(const VkPhysicalDevice& physicalDevice)
{
	m_physicalDevice = physicalDevice;
}

const VkPhysicalDevice& Render::Device::GetPhysicalDevice() const
{
	return m_physicalDevice;
}

void Render::Device::SetLogicalDevice(const VkDevice& logicalDevice)
{
	m_logicalDevice = logicalDevice;
}

const VkDevice& Render::Device::GetLogicalDevice() const
{
	return m_logicalDevice;
}

void Render::Device::SetQueueFamilyInfo(const Render::QueueFamilyInfo& queueFamilyInfo)
{
	m_queueFamilyInfo = queueFamilyInfo;
}

void Render::Device::SetGraphicsQueue(const VkQueue& queue)
{
	m_graphicsQueue = queue;
}

void Render::Device::SetPresentQueue(const VkQueue& queue)
{
	m_presentQueue = queue;
}

const Render::QueueFamilyInfo& Render::Device::GetQueueFamilyInfo() const
{
	return m_queueFamilyInfo;
}

const VkQueue& Render::Device::GetGraphicsQueue() const
{
	return m_graphicsQueue;
}

const VkQueue& Render::Device::GetPresentQueue() const
{
	return m_presentQueue;
}

