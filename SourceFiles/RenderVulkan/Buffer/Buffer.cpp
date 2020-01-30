#include "StdAfx.h"
#include "Buffer.h"

#include "../Toolkit/Toolkit.h"

EngineRenderers::Vulkan::Buffer::Buffer(const EngineRenderers::Vulkan::Device& device, const VkDeviceSize& size, VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags properties, VkSharingMode sharingMode)
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

EngineRenderers::Vulkan::Buffer::~Buffer()
{
	Free();
}

bool EngineRenderers::Vulkan::Buffer::Allocate(const size_t& size, void* data)
{
	if (Map(size, 0) != VK_SUCCESS)
		return false;

	if (!MemoryCopy(data, size))
		return false;

	UnMap();

	return true;
}

bool EngineRenderers::Vulkan::Buffer::Free()
{
	if (m_handle != VK_NULL_HANDLE && m_device != VK_NULL_HANDLE)
	{
		vkDestroyBuffer(m_device, m_handle, nullptr);
		vkFreeMemory(m_device, m_memory, nullptr);
		m_handle = VK_NULL_HANDLE;
		return true;
	}
	
	return false;
}

VkBuffer EngineRenderers::Vulkan::Buffer::GetHandle() const
{
	return m_handle;
}

VkDeviceMemory EngineRenderers::Vulkan::Buffer::GetMemory() const
{
	return m_memory;
}

VkDeviceSize EngineRenderers::Vulkan::Buffer::GetSize() const
{ 
	return m_size;
}

VkResult EngineRenderers::Vulkan::Buffer::Map(const VkDeviceSize& size, const VkDeviceSize offset)
{
	return vkMapMemory(m_device, m_memory, offset, size, 0, &m_pMap);
}

void* EngineRenderers::Vulkan::Buffer::MemoryCopy(void const* data, size_t size)
{
	if (m_pMap == nullptr)
		return nullptr;

	return memcpy(m_pMap, data, size);
}

void EngineRenderers::Vulkan::Buffer::UnMap()
{
	if (m_pMap)
	{
		vkUnmapMemory(m_device, m_memory);
	}
}

EngineRenderers::Vulkan::UniformBuffer::UniformBuffer(const EngineRenderers::Vulkan::Device& device, const VkDeviceSize& size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties, VkSharingMode sharingMode)
	: EngineRenderers::Vulkan::Buffer(device, size, flags, properties, sharingMode)
{
	m_descriptorBufferInfo.buffer = m_handle;
	m_descriptorBufferInfo.range = size;
	m_descriptorBufferInfo.offset = 0;
}

EngineRenderers::Vulkan::UniformBuffer::~UniformBuffer()
{
}

VkDescriptorBufferInfo EngineRenderers::Vulkan::UniformBuffer::GetDescriptorBufferInfo() const
{
	return m_descriptorBufferInfo;
}
