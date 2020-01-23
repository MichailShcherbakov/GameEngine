#define DEBUG

#ifdef RELEASE

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int mCmdShow)
{
	return 0;
}

#endif

#ifdef DEBUG

#define GLFW_EXPOSE_NATIVE_WIN32

#include "Engine/Engine.h"
#include "Engine/Engine.inl"

#include "glfw3.h"
#include "glfw3native.h"

#include "Engine/Engine.h"

int main(int argc, char** argv)
{
	EngineModule module = LoadModule("Engine.Core");
	ICoreModule* pEngineCore = EngineGetProcAddress<ICoreModule::TModulePoint>(module, "CreateEngineCoreModule")();
	
	pEngineCore->Initialize();
	pEngineCore->RunCoreLoop();
	pEngineCore->Release();

	// FreeLibrary

	/*GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1280, 920, "RenderVulkan", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	float color_rotator = 0.0f;

	glfwMakeContextCurrent(window);

	VkDebugUtilsMessengerEXT debugUtilsMessenger = VK_NULL_HANDLE;

	VkInstance instance = Vulkan::Renderer::InitInstance(
		"Union",
		"UnionEngine",
		{ "VK_LAYER_LUNARG_standard_validation" },
		{ VK_KHR_SURFACE_EXTENSION_NAME, VK_KHR_WIN32_SURFACE_EXTENSION_NAME, VK_EXT_DEBUG_UTILS_EXTENSION_NAME },
		debugUtilsMessenger
	);

#if WIN32
	VkSurfaceKHR surface = Vulkan::Renderer::InitWin32Surface(instance, GetModuleHandle(nullptr), glfwGetWin32Window(window));
#endif

	Render::Device device = Vulkan::Renderer::InitDevice(
		instance,
		surface,
		{ "VK_LAYER_LUNARG_standard_validation" },
		{ VK_KHR_SWAPCHAIN_EXTENSION_NAME },
		false
	);

	VkRenderPass renderPass = Vulkan::Renderer::InitRenderPass(
		device,
		surface,
		VK_FORMAT_B8G8R8A8_UNORM,
		VK_FORMAT_D32_SFLOAT_S8_UINT
	);

	Render::Swapchain swapchain = Vulkan::Renderer::InitSwapchain(
		device,
		surface,
		renderPass,
		{ VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR },
		1
	);

	std::vector<VkDescriptorSetLayoutBinding> layoutBinding =
	{
		{
			0,
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			nullptr
		}
	};

	VkDescriptorSetLayout layout = Vulkan::Renderer::InitDescriptorSetLayout(device, layoutBinding);

	std::vector<VkDescriptorPoolSize> layoutPoolSize =
	{
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			1,
		}
	};

	VkDescriptorPool descriptorPool = Vulkan::Renderer::InitDescriptorPool(device, layoutPoolSize);

	VkPipelineLayout pipelineLayout = Vulkan::Renderer::InitPipelineLayout(device, { layout }, {});

	VkPipeline graphicsPipeline = Vulkan::Renderer::InitGraphicsPipeline(device, swapchain, pipelineLayout, renderPass);

	VkCommandPool commandPool = Vulkan::Renderer::InitCommandPool(device, device.GetQueueFamilyInfo().present);
	std::vector<VkCommandBuffer> buffers = Vulkan::Renderer::InitCommandBuffers(device, commandPool, 1);

	VkCommandBufferBeginInfo commandBufferBeginInfo = {};
	commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	commandBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;// VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VkRect2D renderArea;
	renderArea.offset.x = 0;
	renderArea.offset.y = 0;
	renderArea.extent = swapchain.GetImageExtent();





	VkSemaphoreCreateInfo semaphore_create_info = {};
	semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkSemaphore readyToPresent = VK_NULL_HANDLE;
	VK_CHECK_ERROR(vkCreateSemaphore(device.GetLogicalDevice(), &semaphore_create_info, nullptr, &readyToPresent), "");

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	VkFence fence;
	VK_CHECK_ERROR(vkCreateFence(device.GetLogicalDevice(), &fenceInfo, nullptr, &fence), "");

	std::vector<Graphics::Vertex> vertices =
	{
		{ glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), },
		{ glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), },
		{ glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), },
	};

	uint64_t size = (uint64_t)(vertices.size() * sizeof(Graphics::Vertex));
	Render::Buffer* vertexBuffer = new Render::Buffer(
		device,
		size,
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);

	vertexBuffer->Map(size, 0);
	vertexBuffer->MemoryCopy(vertices.data(), (size_t)(vertices.size() * sizeof(Graphics::Vertex)));
	vertexBuffer->UnMap();



	glm::vec3 mainColor = { 1.0f, 0.0f, 0.0f };
	size = (uint64_t)sizeof(mainColor);

	Render::UniformBuffer* colorUniformBuffer = new Render::UniformBuffer(
		device,
		size,
		VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		VK_SHARING_MODE_EXCLUSIVE
	);

	colorUniformBuffer->Map(size, 0);
	colorUniformBuffer->MemoryCopy(&mainColor, size);
	colorUniformBuffer->UnMap();

	VkDescriptorSet descriptorSet = Vulkan::Renderer::InitDescriptorSet(device, descriptorPool, { layout }, { colorUniformBuffer });




	VK_CHECK_ERROR(vkBeginCommandBuffer(buffers[0], &commandBufferBeginInfo), "");

	VkClearValue clearValue;
	clearValue.color = { 0.1f, 0.1f, 0.1f, 1.0f };

	VkRenderPassBeginInfo renderPassBeginInfo = {};
	renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassBeginInfo.framebuffer = swapchain.GetFramebuffers()[0];
	renderPassBeginInfo.renderPass = renderPass;
	renderPassBeginInfo.renderArea = renderArea;
	renderPassBeginInfo.clearValueCount = 1;
	renderPassBeginInfo.pClearValues = &clearValue;

	vkCmdBeginRenderPass(buffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

	VkDeviceSize offsets[1] = { 0 };
	VkBuffer vertexBuffers = vertexBuffer->GetHandle();
	vkCmdBindVertexBuffers(buffers[0], 0, 1, &vertexBuffers, offsets);

	vkCmdBindDescriptorSets(buffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);

	vkCmdDraw(buffers[0], (uint32_t)vertices.size(), 1, 0, 0);

	vkCmdEndRenderPass(buffers[0]);

	VK_CHECK_ERROR(vkEndCommandBuffer(buffers[0]), "");


	while (!glfwWindowShouldClose(window))
	{
		uint32_t imageIndex = UINT16_MAX;

		VK_CHECK_ERROR(vkAcquireNextImageKHR(device.GetLogicalDevice(), swapchain.GetHandle(), UINT64_MAX, VK_NULL_HANDLE, fence, &imageIndex), "Error");
		VK_CHECK_ERROR(vkWaitForFences(device.GetLogicalDevice(), 1, &fence, VK_TRUE, UINT64_MAX), "");
		VK_CHECK_ERROR(vkResetFences(device.GetLogicalDevice(), 1, &fence), "");
		VK_CHECK_ERROR(vkQueueWaitIdle(device.GetPresentQueue()), "");

		color_rotator += 0.01;

		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSubmitInfo submit_info = {};
		submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount = 0;
		submit_info.pWaitSemaphores = nullptr;
		submit_info.pWaitDstStageMask = waitStages;
		submit_info.commandBufferCount = 1;
		submit_info.pCommandBuffers = &buffers[0];
		submit_info.signalSemaphoreCount = 1;
		submit_info.pSignalSemaphores = &readyToPresent;

		VK_CHECK_ERROR(vkQueueSubmit(device.GetPresentQueue(), 1, &submit_info, VK_NULL_HANDLE), "");

		VkResult result = VkResult::VK_RESULT_MAX_ENUM;

		VkSwapchainKHR s = swapchain.GetHandle();

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &readyToPresent;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &s;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = &result;

		VK_CHECK_ERROR(vkQueuePresentKHR(device.GetPresentQueue(), &presentInfo), "");
		VK_CHECK_ERROR(result, "");

		glfwPollEvents();
	}

	vkDeviceWaitIdle(device.GetLogicalDevice());
	vkDestroyFence(device.GetLogicalDevice(), fence, nullptr);
	vkDestroySemaphore(device.GetLogicalDevice(), readyToPresent, nullptr);
	Vulkan::Renderer::DeInitCommandBuffers(buffers, device, commandPool);
	Vulkan::Renderer::DeInitCommandPool(commandPool, device);
	Vulkan::Renderer::DeinitDescriptorSet(descriptorSet, descriptorPool, device);
	Vulkan::Renderer::DeinitGraphicsPipeline(graphicsPipeline, device);
	Vulkan::Renderer::DeinitPipelineLayout(pipelineLayout, device);
	Vulkan::Renderer::DeinitDescriptorPool(descriptorPool, device);
	Vulkan::Renderer::DeinitDescriptorSetLayout(layout, device);
	Vulkan::Renderer::DeinitSwapchain(swapchain, device);
	Vulkan::Renderer::DeinitRenderPass(renderPass, device);
	delete colorUniformBuffer;
	delete vertexBuffer;
	Vulkan::Renderer::DeinitDevice(device);
	Vulkan::Renderer::DeinitWin32Surface(instance, surface);
	Vulkan::Renderer::DeinitInstance(instance, debugUtilsMessenger);

	glfwTerminate();*/
}

#endif