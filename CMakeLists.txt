cmake_minimum_required(VERSION 3.31) # lowering for GH actions...
project(
    worldgen
    VERSION 1.0
    LANGUAGES C
)

# C Standard
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)

add_subdirectory(apps)
