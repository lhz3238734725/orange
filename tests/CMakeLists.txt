set(TEST_LOG test_log)
add_executable(${TEST_LOG} test_log.cpp)
add_dependencies(${TEST_LOG} orange)
target_link_libraries(${TEST_LOG} orange)

set(TEST_CONFIG test_config)
add_executable(${TEST_CONFIG} test_config.cpp)
add_dependencies(${TEST_CONFIG} orange)
target_link_libraries(${TEST_CONFIG} orange)