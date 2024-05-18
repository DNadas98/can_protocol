cmake_minimum_required(VERSION 3.25.1)
project(CAN_CPP)

set(CMAKE_CXX_STANDARD 23)

# Add app executable
add_executable(can_cpp
  src/main.cpp
  src/service/CANService.cpp)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Set up GoogleTest and GoogleMock
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

add_executable(run_tests tests/service/CANServiceTest.cpp src/service/CANService.cpp)
target_link_libraries(run_tests gtest gmock gtest_main pthread)
add_test(NAME CANServiceTest COMMAND run_tests)

# Add a custom target to run the tests
add_custom_target(
  run_tests_all
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  DEPENDS run_tests
)

# Make sure the tests run after the build
add_dependencies(can_cpp run_tests_all)
