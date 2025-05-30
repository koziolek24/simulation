# Inside tests/CMakeLists.txt
add_executable(test_agent test_agent.cc)
add_executable(test_city test_city.cc)
target_link_libraries(test_agent PRIVATE
    zpr25l_lib
    gtest_main
    gmock
)

target_link_libraries(test_city PRIVATE
    zpr25l_lib
    gtest_main
    gmock
)
add_test(NAME test_agent COMMAND test_agent)
add_test(NAME test_city COMMAND test_city)
