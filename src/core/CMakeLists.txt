add_library(ngine_core
    line.cpp
    segment.cpp
    circle.cpp
    polygon.cpp
    transform.cpp
)

target_include_directories(ngine_core PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_core PUBLIC ngine_logging)

ngine_set_compiler_warnings(ngine_core)
ngine_enable_sanitizers(ngine_core)
