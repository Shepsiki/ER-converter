cmake_minimum_required(VERSION 3.22)
project(er-converter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/ec7b38672c04c8af3cfb078214edf6751b841014.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(source_dir ${PROJECT_SOURCE_DIR}/src)
set(include_dir ${PROJECT_SOURCE_DIR}/include)
set(tests_dir ${PROJECT_SOURCE_DIR}/tests)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${source_dir}/*.cpp")

include_directories(${include_dir})

add_library(${PROJECT_NAME}_lib SHARED ${sources})

enable_testing()

add_executable(${PROJECT_NAME}_exe main.cpp)
target_link_libraries(
    ${PROJECT_NAME}_exe 
    PRIVATE ${PROJECT_NAME}_lib
)

add_executable(${PROJECT_NAME}_test ${tests_dir}/test.cpp)
target_link_libraries(
    ${PROJECT_NAME}_test
    PRIVATE ${PROJECT_NAME}_lib
    PRIVATE GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_test)
