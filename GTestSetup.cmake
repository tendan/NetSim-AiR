include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG 58d77fa8070e8cec2dc1ed015d66b454c8d78850
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_DIR ./tests)
set(SOURCE_TESTS_FILES
        ${TEST_DIR}/NetSimBasic_Tests.cpp
        ${TEST_DIR}/test_storage_types.cpp
        ${TEST_DIR}/test_nodes.cpp
        ${TEST_DIR}/test_factory.cpp
)

add_executable(
        ${PROJECT_NAME}_Tests ${SOURCE_FILES} ${SOURCE_TESTS_FILES}
)

target_link_libraries(
        ${PROJECT_NAME}_Tests GTest::gmock_main
)

include_directories(mocks)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Tests)