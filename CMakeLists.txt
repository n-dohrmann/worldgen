cmake_minimum_required(VERSION 3.13)
project(worldgen VERSION 1.0 LANGUAGES C)

# C Standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Create target
add_executable(${PROJECT_NAME} ${SOURCES})

# Include dirs
target_include_directories(${PROJECT_NAME} PRIVATE stb src/render)

# Compiler warnings
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)

# Debug/Release flags
target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Debug>:-g;-DDEBUG>"
    "$<$<CONFIG:Release>:-O3>"
)

# Link math lib on Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} m)
endif()

# Set binary output dir
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "No external dependencies required - using stb libraries")
