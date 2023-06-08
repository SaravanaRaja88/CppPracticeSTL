cmake_minimum_required(VERSION 3.20)

include(CreateTestBinaries.cmake)
 
project(TestStlContainers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(GTest_DIR "/lhome/rajasar/TestProject/googletest/build/install/lib/cmake/GTest" CACHE PATH "GTestConfig.cmake")
find_package(GTest REQUIRED)

add_subdirectory(SequenceContainers)
add_subdirectory(AssociativeContainers)
add_subdirectory(UnorderedAssociativeContainers)