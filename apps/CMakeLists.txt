add_executable(worldgen_app main.c)
target_compile_features(worldgen_app PRIVATE c_std_23)
target_compile_options(worldgen_app PRIVATE -Wall -Wextra)

target_link_libraries(worldgen_app PRIVATE render)
