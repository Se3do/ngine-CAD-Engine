add_library(ngine_interface
    document.cpp
    command_parser.cpp
    cli_app.cpp
)

target_include_directories(ngine_interface PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_interface PUBLIC
    ngine_core
    ngine_intersection
    ngine_constraints
    ngine_commands
)

ngine_set_compiler_warnings(ngine_interface)
ngine_enable_sanitizers(ngine_interface)
