cmake_minimum_required(VERSION 3.10)
project(CANListener)

set(CMAKE_CXX_STANDARD 11)

# Add app executable
add_executable(can_listener src/main.cpp src/service/CANService.cpp)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
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
