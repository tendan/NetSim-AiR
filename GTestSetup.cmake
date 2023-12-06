include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_DIR ./tests)
set(SOURCE_TESTS_FILES
        ${TEST_DIR}/NetSimBasic_Tests.cpp
)

add_executable(
        ${PROJECT_NAME}_Tests ${SOURCE_FILES} ${SOURCE_TESTS_FILES}
)

target_link_libraries(
        ${PROJECT_NAME}_Tests GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_Tests)