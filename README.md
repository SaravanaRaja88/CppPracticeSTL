## Intro
The purpose of this repo is to practise all the STL containers and algorithms
using the google test.
This way we explore the STL as well as the google test framework

## Build google test

```bash
cd ~/work
git clone https://github.com/google/googletest
cd googletest
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=./install
cmake --build . -j4
cmake --install .
```

## Specify GTest_DIR    

Open your CMakeLists.txt, specify `GTest_DIR` to the directory that contains `GTestConfig.cmake`.

For me, it is `/home/username/work/googletest/build/install/lib/cmake/GTest`.


## find_package and link
```cmake
find_package(GTest REQUIRED)

add_executable(test
    main.cpp
)

target_link_libraries(test GTest::gtest)
```
Note, you don't have to manually specify including directory