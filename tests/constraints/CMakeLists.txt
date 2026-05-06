add_executable(test_constraints
    test_constraints.cpp
)

target_link_libraries(test_constraints PRIVATE ngine_constraints GTest::gtest_main)
gtest_discover_tests(test_constraints)
ngine_enable_sanitizers(test_constraints)
