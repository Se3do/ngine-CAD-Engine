add_executable(ngine_cli ngine_cli.cpp)

target_link_libraries(ngine_cli PRIVATE
    ngine_interface
    ngine_logging
)

ngine_set_compiler_warnings(ngine_cli)
ngine_enable_sanitizers(ngine_cli)
