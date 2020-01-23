#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "vulkan/vulkan.h"
#include <vector>

namespace Render
{
	struct QueueFamilyInfo
	{
		int32_t present = -1;
		int32_t graphics = -1;
		int32_t compute = -1;
		int32_t transfer = -1;

		bool IsRenderingCompatible() const
		{
			return graphics != -1 && present != -1;
		}
	};

	class Device
	{
	public:
		Device();
		~Device();

	public:
		void DeInit();

	public:
		void SetQueueFamilyInfo(const Render::QueueFamilyInfo& queueFamilyInfo);
		void SetPhysicalDevice(const VkPhysicalDevice& physicalDevice);
		void SetLogicalDevice(const VkDevice& logicalDevice); 
		void SetGraphicsQueue(const VkQueue& queue);
		void SetPresentQueue(const VkQueue& queue);

	public:
		const Render::QueueFamilyInfo& GetQueueFamilyInfo() const;
		const VkPhysicalDevice& GetPhysicalDevice() const;
		const VkDevice& GetLogicalDevice() const;
		const VkQueue& GetGraphicsQueue() const;
		const VkQueue& GetPresentQueue() const;

	private:
		Render::QueueFamilyInfo m_queueFamilyInfo;
		VkPhysicalDevice m_physicalDevice;
		VkDevice m_logicalDevice;
		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
	};
}

#endif // _DEVICE_H_