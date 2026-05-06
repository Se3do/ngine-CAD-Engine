add_executable(test_integration
    test_integration.cpp
)

target_link_libraries(test_integration PRIVATE
    ngine_interface
    ngine_serialization
    GTest::gtest_main
)
gtest_discover_tests(test_integration)
