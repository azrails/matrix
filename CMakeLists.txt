cmake_minimum_required(VERSION 3.14)

#compiler settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

#project settings
project(matrix)
set(SOURCES matrix.cpp)

#build shared or static lib
option(STATICLIB "BUILD STATIC LIBRARY" OFF)
if (STATICLIB)
    add_library(matrix STATIC ${SOURCES})
else()
    add_library(matrix SHARED ${SOURCES})
endif(STATICLIB)

#testing block
option(TEST "BUILD TESTS" OFF)
if(TEST)
    #include Gtest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY  https://github.com/google/googletest.git
        GIT_TAG         v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
    enable_testing()
    add_executable(test_matrix test_matrix.cpp)
    target_link_libraries(test_matrix matrix GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(test_matrix)
endif(TEST)