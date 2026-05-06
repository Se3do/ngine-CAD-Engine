add_executable(test_commands
    test_commands.cpp
)

target_link_libraries(test_commands PRIVATE ngine_commands ngine_interface GTest::gtest_main)
gtest_discover_tests(test_commands)
