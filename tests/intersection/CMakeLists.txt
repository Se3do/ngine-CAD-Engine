add_executable(test_intersection
    test_intersection.cpp
    test_arc_intersection.cpp
)

target_link_libraries(test_intersection PRIVATE ngine_intersection GTest::gtest_main)
gtest_discover_tests(test_intersection)
ngine_enable_sanitizers(test_intersection)
