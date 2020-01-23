cmake_minimum_required(VERSION 3.8)

project(Engine.Core)

set(CMAKE_CXX_STANDARD 17)

set(CORE_MODULE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/CoreModule.h
	${CMAKE_CURRENT_SOURCE_DIR}/CoreModule.cpp
)

set(CORE_STDAFX_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
)

set(CORE_CONSOLE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Console/Console.h
	${CMAKE_CURRENT_SOURCE_DIR}/Console/Console.cpp
)

set(CORE_LOG_SYSTEM_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/LogSystem/LogSystem.h
	${CMAKE_CURRENT_SOURCE_DIR}/LogSystem/LogSystem.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
	${CORE_MODULE_SOURCES}
	${CORE_LOG_SYSTEM_SOURCES}
	${CORE_STDAFX_SOURCES}
	${CORE_CONSOLE_SOURCES}
)

add_library(${PROJECT_NAME} SHARED
	${CORE_MODULE_SOURCES}
	${CORE_LOG_SYSTEM_SOURCES}
	${CORE_STDAFX_SOURCES}
	${CORE_CONSOLE_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${PROJECT_SOURCE_DIR}
	${ENGINE_ROOT}/SourceFiles/Common
	${ENGINE_ROOT}/SourceFiles/RenderOpengl
	${ENGINE_ROOT}/SourceFiles/RenderVulkan
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${ENGINE_ROOT}/SourceFiles/SDK/glm/Include 
	${ENGINE_ROOT}/SourceFiles/SDK/stb/Include 
	${ENGINE_ROOT}/SourceFiles/SDK/boost/Include
	${ENGINE_ROOT}/SourceFiles/SDK/vulkan/Include 
)

if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC ${ENGINE_ROOT}/SourceFiles/SDK/boost/Lib)
elseif ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x32")
	target_link_directories(${PROJECT_NAME} PUBLIC ${ENGINE_ROOT}/SourceFiles/SDK/boost/Lib32)
endif()

target_link_libraries(${PROJECT_NAME} dxgi.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/YuStdAfx.h")

set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/YcStdAfx.h")