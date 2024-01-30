cmake_minimum_required(VERSION 3.20)

include(CreateTestBinaries.cmake)
 
project(PractiseStlContainersAndAlgorithms)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(GTest_DIR "/lhome/rajasar/TestProject/googletest/build/install/lib/cmake/GTest" CACHE PATH "GTestConfig.cmake")
find_package(GTest REQUIRED)

add_subdirectory(Containers)
add_subdirectory(Strings)
add_subdirectory(Algorithms)

#Just a small test application to test something quickly with std::out printout's
add_executable(mainOut main.cpp)