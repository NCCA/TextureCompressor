cmake_minimum_required(VERSION 3.12)
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND DEFINED ENV{CMAKE_TOOLCHAIN_FILE})
   set(CMAKE_TOOLCHAIN_FILE $ENV{CMAKE_TOOLCHAIN_FILE})
endif()

project(TextureCompressorAll)
add_subdirectory(${PROJECT_SOURCE_DIR}/Compressor/ )
add_subdirectory(${PROJECT_SOURCE_DIR}/DXTViewer/ )
add_subdirectory(${PROJECT_SOURCE_DIR}/PackTool/ )
add_subdirectory(${PROJECT_SOURCE_DIR}/PackViewer/ )

