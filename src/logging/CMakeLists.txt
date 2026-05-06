add_library(ngine_logging
    logger.cpp
    console_sink.cpp
    file_sink.cpp
)

target_include_directories(ngine_logging PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

ngine_set_compiler_warnings(ngine_logging)
ngine_enable_sanitizers(ngine_logging)
