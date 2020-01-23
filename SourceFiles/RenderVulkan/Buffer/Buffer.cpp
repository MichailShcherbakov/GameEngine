#include "StdAfx.h"
#include "Buffer.h"

#include "../Toolkit/Toolkit.h"

Render::Buffer::Buffer(const Render::Device& device, const VkDeviceSize& size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, VkSharingMode sharingMode)
	: m_device(device.GetLogicalDevice())
{
	VK_CHECK_NULL_HANDLE(m_device, "");

	VkBuffer buffer = VK_NULL_HANDLE;

	VkBufferCreateInfo bufferCreateInfo = {};
	bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferCreateInfo.usage = usageFlags;
	bufferCreateInfo.sharingMode = sharingMode;
	bufferCreateInfo.size = size;

	VK_CHECK_ERROR(vkCreateBuffer(m_device, &bufferCreateInfo, nullptr, &buffer), "");

	VkMemoryRequirements memoryRequirements = {};
	vkGetBufferMemoryRequirements(m_device, buffer, &memoryRequirements);

	bool isFound = false;
	uint32_t index = 0;
	VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
	vkGetPhysicalDeviceMemoryProperties(device.GetPhysicalDevice(), &deviceMemoryProperties);

	for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
	{
		if ((memoryRequirements.memoryTypeBits & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			isFound = true;
			index = i;
			break;
		}
	}

	if (!isFound)
		throw std::runtime_error("");

	VkMemoryAllocateInfo memoryAllocateInfo = {};
	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = memoryRequirements.size;
	memoryAllocateInfo.memoryTypeIndex = index;

	VkDeviceMemory memory = VK_NULL_HANDLE;
	VK_CHECK_ERROR(vkAllocateMemory(m_device, &memoryAllocateInfo, nullptr, &memory), "");

	VK_CHECK_ERROR(vkBindBufferMemory(m_device, buffer, memory, 0), "");

	m_handle = buffer;
	m_memory = memory;
	m_size = size;
}

Render::Buffer::~Buffer()
{
	if (m_handle != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_device, m_handle, nullptr);
		vkFreeMemory(m_device, m_memory, nullptr);
		m_handle = VK_NULL_HANDLE;
	}
}

VkBuffer Render::Buffer::GetHandle() const
{
	return m_handle;
}

VkDeviceMemory Render::Buffer::GetMemory() const
{
	return m_memory;
}

VkDeviceSize Render::Buffer::GetSize() const
{ 
	return m_size;
}

VkResult Render::Buffer::Map(const VkDeviceSize& size, const VkDeviceSize offset)
{
	return vkMapMemory(m_device, m_memory, offset, size, 0, &m_pMap);
}

void* Render::Buffer::MemoryCopy(void const* data, size_t size)
{
	if (m_pMap == nullptr)
		return nullptr;

	return memcpy(m_pMap, data, size);
}

void Render::Buffer::UnMap()
{
	if (m_pMap)
	{
		vkUnmapMemory(m_device, m_memory);
	}
}

Render::UniformBuffer::UniformBuffer(const Render::Device& device, const VkDeviceSize& size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties, VkSharingMode sharingMode)
	: Buffer(device, size, flags, properties, sharingMode)
{
	m_descriptorBufferInfo.buffer = m_handle;
	m_descriptorBufferInfo.range = size;
	m_descriptorBufferInfo.offset = 0;
}

Render::UniformBuffer::~UniformBuffer()
{
}

VkDescriptorBufferInfo Render::UniformBuffer::GetDescriptorBufferInfo() const
{
	return m_descriptorBufferInfo;
}
