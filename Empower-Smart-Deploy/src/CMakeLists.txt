cmake_minimum_required(VERSION 3.6)

project(ESD_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(imgui)

add_executable(${PROJECT_NAME} WIN32  main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui d3d11.lib dxgi.lib d3dcompiler.lib)