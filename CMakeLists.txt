cmake_minimum_required(VERSION 3.10)
project(worldgen VERSION 1.0 LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3")

# Include directories
include_directories(stb)

# Source files
file(GLOB SOURCES "src/*.c")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Math library (required on some platforms)
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m)
endif()

# Output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "No external dependencies required - using stb libraries")
