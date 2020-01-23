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
	${ENGINE_ROOT}/SourceFiles/SDK/glm/Include 
)