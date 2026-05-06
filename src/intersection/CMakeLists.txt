add_library(ngine_intersection
    standard_intersection.cpp
    intersection_engine.cpp
)

target_include_directories(ngine_intersection PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_intersection PUBLIC ngine_core)

ngine_set_compiler_warnings(ngine_intersection)
ngine_enable_sanitizers(ngine_intersection)
