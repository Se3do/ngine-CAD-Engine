add_library(ngine_commands
    command_history.cpp
    geometry_commands.cpp
)

target_include_directories(ngine_commands PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_commands PUBLIC ngine_core)

ngine_set_compiler_warnings(ngine_commands)
ngine_enable_sanitizers(ngine_commands)
