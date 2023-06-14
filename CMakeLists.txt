cmake_minimum_required(VERSION 3.20)

include(CreateTestBinaries.cmake)
 
project(TestStlContainers)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

set(GTest_DIR "/lhome/rajasar/TestProject/googletest/build/install/lib/cmake/GTest" CACHE PATH "GTestConfig.cmake")
find_package(GTest REQUIRED)

add_subdirectory(Containers/SequenceContainers)
add_subdirectory(Containers/AssociativeContainers)
add_subdirectory(Containers/UnorderedAssociativeContainers)
add_subdirectory(Strings)