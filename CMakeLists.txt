cmake_minimum_required(VERSION 3.12)
project(VulkanTriangle)

# Specify the C language standard
set(CMAKE_C_STANDARD 11)

# Find Vulkan packages
find_package(Vulkan REQUIRED)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan packages not found. Make sure Vulkan is correctly installed.")
endif()

# Get all source files from the 'src' directory
file(GLOB_RECURSE SOURCES src/*.c)

# Include Vulkan header files and custom include directory
include_directories(${Vulkan_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Check if GLFW submodule exists, and clone it if missing
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/CMakeLists.txt)
    message(STATUS "GLFW submodule not found. Cloning the submodule...")
    execute_process(COMMAND git submodule update --init external/glfw WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Check if CGLM submodule exists, and clone it if missing
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/CMakeLists.txt)
    message(STATUS "CGLM submodule not found. Cloning the submodule...")
    execute_process(COMMAND git submodule update --init external/cglm WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Add external libraries (GLFW)
add_subdirectory(external/glfw)

# Add external libraries (GLFW)
add_subdirectory(external/cglm)

# Create the executable for your project
add_executable(VulkanTriangle ${SOURCES})

# Set compilation options for files in the 'src' directory (with warnings)
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS "-g3 -Wall -Wextra")

# Link Vulkan, and GLFW libraries to your executable
target_link_libraries(VulkanTriangle ${Vulkan_LIBRARIES} glfw cglm)
