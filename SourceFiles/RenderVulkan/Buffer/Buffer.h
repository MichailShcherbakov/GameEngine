#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "vulkan/vulkan.h"

namespace Render
{
	class Device;

	class Buffer
	{
	public:
		Buffer() = delete;
		Buffer(const Render::Device& device, const VkDeviceSize& size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties, VkSharingMode sharingMode);
		virtual ~Buffer();

	public:
		VkBuffer GetHandle() const;
		VkDeviceMemory GetMemory() const;
		VkDeviceSize GetSize() const;

	public:
		VkResult Map(const VkDeviceSize& size = VK_WHOLE_SIZE, const VkDeviceSize offset = 0);
		void* MemoryCopy(void const* data, size_t size);
		void UnMap();

	public:
		void operator=(const Buffer& buffer) = delete;

	protected:
		VkBuffer m_handle = VK_NULL_HANDLE;
		VkDeviceMemory m_memory = VK_NULL_HANDLE;
		VkDeviceSize m_size = 0;
		const VkDevice m_device = VK_NULL_HANDLE;
		void* m_pMap = nullptr;
	};
	
	class UniformBuffer : public Buffer
	{
	public:
		UniformBuffer() = delete;
		UniformBuffer(const Render::Device& device, const VkDeviceSize& size, VkBufferUsageFlags flags, VkMemoryPropertyFlags properties, VkSharingMode sharingMode);
		~UniformBuffer() override;

	public:
		VkDescriptorBufferInfo GetDescriptorBufferInfo() const;

	public:
		void operator=(const UniformBuffer& uniformbuffer) = delete;

	protected:
		VkDescriptorBufferInfo m_descriptorBufferInfo = {};
	};
}

#endif // _BUFFER_H_