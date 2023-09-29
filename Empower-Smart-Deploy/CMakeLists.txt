cmake_minimum_required(VERSION  3.6  FATAL_ERROR)

project(ESD LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(STAGED_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/stage)

add_subdirectory(src)

