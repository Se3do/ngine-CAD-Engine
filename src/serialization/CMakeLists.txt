add_library(ngine_serialization
    json_serializer.cpp
)

target_include_directories(ngine_serialization PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_serialization PUBLIC
    ngine_interface
    nlohmann_json::nlohmann_json
)

ngine_set_compiler_warnings(ngine_serialization)
ngine_enable_sanitizers(ngine_serialization)
