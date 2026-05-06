add_library(ngine_spatial
    quadtree.cpp
)

target_include_directories(ngine_spatial PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_spatial PUBLIC ngine_core)

ngine_set_compiler_warnings(ngine_spatial)
ngine_enable_sanitizers(ngine_spatial)
