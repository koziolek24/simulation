# Inside tests/CMakeLists.txt
add_executable(test_agent test_agent.cc)
add_executable(test_city test_city.cc)
add_executable(test_disease test_disease.cc)
add_executable(all_tests
    test_agent.cc
    test_city.cc
    test_disease.cc
)
target_link_libraries(all_tests PRIVATE
    zpr25l_lib
    gtest_main
    gmock
)
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

target_link_libraries(test_disease PRIVATE
    zpr25l_lib
    gtest_main
    gmock
)
add_test(NAME test_agent COMMAND test_agent)
add_test(NAME test_city COMMAND test_city)
add_test(NAME test_disease COMMAND test_disease)
add_test(NAME all_tests COMMAND all_tests)
