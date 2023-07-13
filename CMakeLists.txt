cmake_minimum_required(VERSION 3.12)
project(VulkanTriangle)

# Specify the C language standard
set(CMAKE_C_STANDARD 11)

# Add default compilation options (without -Wall and -Wextra)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g3")

# Find Vulkan packages
find_package(Vulkan REQUIRED)
if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan packages not found. Make sure Vulkan is correctly installed.")
endif()

# Include Vulkan header files
include_directories(${Vulkan_INCLUDE_DIRS})

# Get all source files from the 'src' directory
file(GLOB SOURCES src/*.c)

# Add external libraries (GLFW and CGLM)
add_subdirectory(external/glfw)
add_subdirectory(external/cglm)

# Create the executable for your project
add_executable(VulkanTriangle ${SOURCES})

# Set compilation options for files in the 'src' directory (with warnings)
set_source_files_properties(${SOURCES} PROPERTIES COMPILE_FLAGS "-Wall -Wextra")

# Link Vulkan, GLFW, and CGLM libraries to your executable
target_link_libraries(VulkanTriangle ${Vulkan_LIBRARIES} glfw cglm)
