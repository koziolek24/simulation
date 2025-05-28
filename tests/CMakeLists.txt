# Inside tests/CMakeLists.txt
add_executable(test_agent test_agent.cc)
add_executable(test_worker test_worker.cc)
target_link_libraries(test_agent PRIVATE 
    zpr25l_lib 
    gtest_main  # <-- Link to gtest_main (provides main())
    gmock       # If needed
)
target_link_libraries(test_worker PRIVATE 
    zpr25l_lib 
    gtest_main  # <-- Link to gtest_main (provides main())
    gmock       # If needed
)

add_test(NAME test_agent COMMAND test_agent)
add_test(NAME test_worker COMMAND test_worker)