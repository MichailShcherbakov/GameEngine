cmake_minimum_required(VERSION 3.5)

project(Engine.RenderOpengl)

set(RENDER_MODULE_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/RendererOpenglModule.h
	${CMAKE_CURRENT_SOURCE_DIR}/RendererOpenglModule.cpp
)

set(RENDER_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererOpengl.h
	${CMAKE_CURRENT_SOURCE_DIR}/Renderer/RendererOpengl.cpp
)

set(RENDER_STDAFX_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
	${RENDER_MODULE_SOURCES}
	${RENDER_SOURCES}
	${RENDER_STDAFX_SOURCES}
)

add_library(${PROJECT_NAME} SHARED
	${RENDER_MODULE_SOURCES}
	${RENDER_SOURCES}
	${RENDER_STDAFX_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/SourceFiles/Common
	${CMAKE_SOURCE_DIR}/SourceFiles/Core
)

target_include_directories(${PROJECT_NAME} PRIVATE 
	${ENGINE_ROOT}/SourceFiles/SDK/glew/Include
	${ENGINE_ROOT}/SourceFiles/SDK/glm/Include
	${ENGINE_ROOT}/SourceFiles/SDK/stb/Include
	${ENGINE_ROOT}/SourceFiles/SDK/boost/Include
	${ENGINE_ROOT}/SourceFiles/SDK/assimp/Include
)

if(("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64"))
	target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/boost/Lib)
    target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/assimp/Lib)
    target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/glew/Lib)
elseif (("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x32"))
	target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/glew/Lib32)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE assimp-vc142-mtd.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE glew32.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE glew32s.lib)
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/YuStdAfx.h")

set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/YcStdAfx.h")