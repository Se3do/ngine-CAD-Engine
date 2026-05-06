add_executable(test_spatial
    test_spatial.cpp
)

target_link_libraries(test_spatial PRIVATE ngine_spatial GTest::gtest_main)
gtest_discover_tests(test_spatial)
