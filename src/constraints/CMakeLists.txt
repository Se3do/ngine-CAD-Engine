add_library(ngine_constraints
    constraint_types.cpp
    sequential_solver.cpp
    constraint_system.cpp
)

target_include_directories(ngine_constraints PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(ngine_constraints PUBLIC ngine_core)

ngine_set_compiler_warnings(ngine_constraints)
ngine_enable_sanitizers(ngine_constraints)
