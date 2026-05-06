add_executable(test_serialization
    test_serialization.cpp
)

target_link_libraries(test_serialization PRIVATE ngine_serialization GTest::gtest_main)
gtest_discover_tests(test_serialization)
ngine_enable_sanitizers(test_serialization)
