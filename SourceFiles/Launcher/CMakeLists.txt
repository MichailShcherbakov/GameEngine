cmake_minimum_required(VERSION 3.8)

project(Launcher)

set(CMAKE_CXX_STANDARD 17)

set(DEBUG ON)

if (DEBUG) 
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
else()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
endif()

add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${ENGINE_ROOT}/SourceFiles/Common
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	${ENGINE_ROOT}/SourceFiles/SDK/glwf/Include 
	${ENGINE_ROOT}/SourceFiles/SDK/glm/Include 
)

if ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PUBLIC ${ENGINE_ROOT}/SourceFiles/SDK/glwf/Lib)
elseif ("${CMAKE_GENERATOR_PLATFORM}" STREQUAL "x32")
	target_link_directories(${PROJECT_NAME} PUBLIC ${ENGINE_ROOT}/SourceFiles/SDK/glwf/Lib32)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC 
	glfw3.lib
)