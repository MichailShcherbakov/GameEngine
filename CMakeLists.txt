cmake_minimum_required(VERSION 3.8)

project(UnionEngine)

set(ENGINE_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/.bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/Common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/Core)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/Launcher)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/GLFW)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/RenderOpengl)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/SourceFiles/RenderVulkan)