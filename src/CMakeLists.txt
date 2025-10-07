
set(RENDER_HEADER_LIST "${worldgen_SOURCE_DIR}/include/worldgen/render.h")
add_library(render render.c ${RENDER_HEADER_LIST})
target_include_directories(render PUBLIC ../include)
target_compile_options(render PRIVATE -Wall -Wextra)
target_compile_features(render PUBLIC c_std_23)

set(CLENAUP_HEADER_LIST "${worldgen_SOURCE_DIR}/include/worldgen/cleanup.h")
add_library(cleanup cleanup.c ${CLENAUP_HEADER_LIST})
target_include_directories(cleanup PUBLIC ../include)
target_compile_options(cleanup PRIVATE -Wall -Wextra)
target_compile_features(cleanup PUBLIC c_std_23)
