#include <iostream>
#include <gtest/gtest.h>
#include <array>

TEST(TestArray, MemberFunctions)
{
    std::array<int, 5> arr{1,2,3,4,5};
    int expected[] = {1,2,3,4,5};
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected));

    auto arr2 = arr;
    EXPECT_TRUE(std::equal(arr2.begin(), arr2.end(), expected));
}

TEST(TestArray, ElementAccess)
{
    std::array<int,5> arr{1,2,3,4,5};

    EXPECT_EQ(arr.at(3),4);
    EXPECT_ANY_THROW(arr.at(7));

    EXPECT_EQ(arr[2], 3);
    arr[2] = 22;
    EXPECT_EQ(arr[2],22);

    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(),5);

    auto rawData = arr.data();
    EXPECT_EQ(*rawData,1);
    rawData++;
    EXPECT_EQ(*rawData,2);
    rawData++;
    EXPECT_EQ(*rawData,22);

}

TEST(TestArray, Iterators)
{
    std::array<int,5> arr{1,7,3,4,5};
    auto iter = arr.begin();
    EXPECT_EQ(*iter, 1);
    iter++;
    EXPECT_EQ(*iter,7);
    --iter;
    EXPECT_EQ(*iter, 1);

    auto iterEnd = arr.end();
    EXPECT_GE(*iterEnd, 32764);
    --iterEnd;
    EXPECT_EQ(*iterEnd, 5);

    EXPECT_EQ(*arr.rbegin(),5);
    EXPECT_EQ(*arr.rend(),0);
    EXPECT_EQ(*--arr.rend(), 1);
}

TEST(TestArray, Capacity)
{
    std::array<int,5> arr;
    std::array<int,0> arr2;

    EXPECT_FALSE(arr.empty());
    EXPECT_TRUE(arr2.empty());

    EXPECT_EQ(arr.size(),5);
    EXPECT_EQ(arr2.size(), 0);

    EXPECT_EQ(arr.max_size(),5);
    EXPECT_EQ(arr2.max_size(),0);
}

TEST(TestArray, Operations)
{
    std::array<int, 4> arr;
    arr.fill(4);

    int expected[] = {4,4,4,4};
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected));

    std::array<int,4> arr2{1,1,1,1};
    arr2.swap(arr);
    int expected2[] = {1,1,1,1};
    EXPECT_TRUE(std::equal(arr.begin(), arr.end(), expected2));
    EXPECT_TRUE(std::equal(arr2.begin(), arr2.end(), expected));

}

TEST(TestArray, NonMemberFunctions)
{
    std::array<int,4> arr1{1,2,3,4};
    std::array<int,4> arr2{1,2,3,3};

    EXPECT_FALSE(arr1 == arr2);

    EXPECT_FALSE(arr1 < arr2);
    EXPECT_TRUE(arr1 > arr2);

    EXPECT_FALSE(arr1 <= arr2);
    EXPECT_FALSE(arr2 >= arr1);

    EXPECT_EQ(std::get<0>(arr1),1);
    EXPECT_EQ(std::get<3>(arr1),4);

    std::swap(arr1,arr2);
    int expected[] = {1,2,3,4};
    int expected1[] = {1,2,3,3};
    EXPECT_TRUE(std::equal(arr1.begin(), arr1.end(),expected1));
    EXPECT_TRUE(std::equal(arr2.begin(), arr2.end(),expected));

    auto arr3 = std::to_array({11,22,33,44});
    EXPECT_EQ(arr3.size(),4);
    int expected3[] = {11,22,33,44};
    EXPECT_TRUE(std::equal(arr3.begin(), arr3.end(),expected3));
}

TEST(TestArray, HelperClass)
{
    std::array<int, 5> arr{1,2,2,2,4};

    EXPECT_EQ(std::tuple_size<decltype(arr)>{},5);
    auto val = std::is_same<std::tuple_element<0,decltype(arr)>::type,int>::value;
    EXPECT_TRUE(val);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}