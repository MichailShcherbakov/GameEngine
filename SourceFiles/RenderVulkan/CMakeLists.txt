cmake_minimum_required(VERSION 3.5)

project(Engine.RenderVulkan)

set(RENDER_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererVulkan.h
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererVulkan.cpp
)

set(RENDER_MODULE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/RendererVulkanModule.h
	${CMAKE_CURRENT_SOURCE_DIR}/RendererVulkanModule.cpp
)

set(RENDER_DEVICE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Device/Device.h
	${CMAKE_CURRENT_SOURCE_DIR}/Device/Device.cpp
)

set(RENDER_SWAPCHAIN_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Swapchain/Swapchain.h
	${CMAKE_CURRENT_SOURCE_DIR}/Swapchain/Swapchain.cpp
)

set(RENDER_BUFFER_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Buffer/Buffer.h
	${CMAKE_CURRENT_SOURCE_DIR}/Buffer/Buffer.cpp
)

set(RENDER_TOOLKIT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Toolkit/Toolkit.h
	${CMAKE_CURRENT_SOURCE_DIR}/Toolkit/Toolkit.cpp
)

set(RENDER_STDAFX_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
	${RENDER_SOURCES}
	${RENDER_TOOLKIT_SOURCES}
	${RENDER_DEVICE_SOURCES}
	${RENDER_SWAPCHAIN_SOURCES}
	${RENDER_BUFFER_SOURCES}
	${RENDER_MODULE_SOURCES}
	${RENDER_STDAFX_SOURCES}
)

add_library(${PROJECT_NAME} SHARED
	${RENDER_SOURCES}
	${RENDER_DEVICE_SOURCES}
	${RENDER_SWAPCHAIN_SOURCES}
	${RENDER_TOOLKIT_SOURCES}
	${RENDER_BUFFER_SOURCES}
	${RENDER_MODULE_SOURCES}
	${RENDER_STDAFX_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/SourceFiles/Common
	${CMAKE_SOURCE_DIR}/SourceFiles/Core
)

target_include_directories(${PROJECT_NAME} PRIVATE 
	${ENGINE_ROOT}/SourceFiles/SDK/vulkan/Include
	${ENGINE_ROOT}/SourceFiles/SDK/glm/Include
	${ENGINE_ROOT}/SourceFiles/SDK/stb/Include
	${ENGINE_ROOT}/SourceFiles/SDK/boost/Include
)

if(("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64"))
    target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/vulkan/Lib)
    target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/boost/Lib)
elseif (("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x32"))
	target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/vulkan/Lib32)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/SourceFiles/Common)

target_link_libraries(${PROJECT_NAME} PRIVATE vulkan-1.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/YuStdAfx.h")

set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/YcStdAfx.h")

