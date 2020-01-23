cmake_minimum_required(VERSION 3.5)

project(Engine.GLFW)

set(GLFW_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/glfwModule.h
	${CMAKE_CURRENT_SOURCE_DIR}/glfwModule.cpp
)

set(GLFW_STDAFX_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.h
	${CMAKE_CURRENT_SOURCE_DIR}/StdAfx.cpp
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
	${GLFW_SOURCES}
	${GLFW_STDAFX_SOURCES}
)

add_library(${PROJECT_NAME} SHARED
	${GLFW_SOURCES}
	${GLFW_STDAFX_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	${PROJECT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/SourceFiles/Common
	${CMAKE_SOURCE_DIR}/SourceFiles/Core
)


target_include_directories(${PROJECT_NAME} PRIVATE 
	${ENGINE_ROOT}/SourceFiles/SDK/glfw/Include
)

if(("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64"))
    target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/glfw/Lib)
elseif (("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x32"))
	target_link_directories(${PROJECT_NAME} PRIVATE ${ENGINE_ROOT}/SourceFiles/SDK/glfw/Lib32)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE glfw3.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/YuStdAfx.h")

set_source_files_properties(StdAfx.cpp PROPERTIES COMPILE_FLAGS "/YcStdAfx.h")

