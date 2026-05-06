add_executable(test_core
    test_point_vector.cpp
    test_line_segment.cpp
    test_shapes_transform.cpp
)

target_link_libraries(test_core PRIVATE ngine_core GTest::gtest_main)
gtest_discover_tests(test_core)
ngine_enable_sanitizers(test_core)
