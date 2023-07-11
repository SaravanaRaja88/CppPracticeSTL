#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>

TEST(MinMaxOperations, max)
{
    using namespace std::string_view_literals;

    EXPECT_EQ(std::max(4,5),5);

    EXPECT_EQ(std::ranges::max(4,5),5);

    EXPECT_EQ(std::max({1,22,3,4,5,6}),22);

    EXPECT_EQ(std::ranges::max({1,22,3,4,5,6}),22);

    EXPECT_EQ(std::ranges::max({"Helo"sv,"How"sv,"Are"sv,"U"sv},{},&std::string_view::size),"Helo");
}

TEST(MinMaxOperations, max_element)
{
    std::vector<int> vec{1,2,3,44,5,6,7};

    EXPECT_EQ(*(std::max_element(vec.begin(),vec.end())),44);

    EXPECT_EQ(*(std::ranges::max_element(vec.begin(),vec.end())),44);

    EXPECT_EQ(*(std::ranges::max_element(vec)),44);
}

TEST(MinMaxOperations, min)
{
    EXPECT_EQ(std::min(4,5),4);

    EXPECT_EQ(std::ranges::min(4,5),4);

    EXPECT_EQ(std::ranges::min({0,-1,2,4,6}),-1);
}

TEST(MinMaxOperations, min_element)
{
    std::vector<int> vec{-1,-2,1,2,3,4,5};

    EXPECT_EQ(*(std::min_element(vec.begin(),vec.end())),-2);

    EXPECT_EQ(*(std::ranges::min_element(vec.begin(),vec.end())),-2);

    EXPECT_EQ(*(std::ranges::min_element(vec)),-2);
}

TEST(MinMaxOperations, minmax)
{
    auto value = std::minmax(4,6);
    //TODO:
    //With gtest for some reason accessing first and then second gives undefined values
    //to the second. so due to this i had to swap the checking for second value first 
    // and then the first value like below.  Need to research the reason.
    EXPECT_EQ(value.second, 6);
    EXPECT_EQ(value.first, 4);

    auto value2 = std::ranges::minmax(4,20);
    EXPECT_EQ(value2.max, 20);
    EXPECT_EQ(value2.min, 4);
}

TEST(MinMaxOperations, minmax_element)
{
    std::vector<int> vec{-4, -3, 1, 2, 5, 9};

    auto value = std::minmax_element(vec.begin(),vec.end());
    EXPECT_EQ(*value.first, -4);
    EXPECT_EQ(*value.second, 9);

    auto value2 = std::ranges::minmax_element(vec);
    EXPECT_EQ(*value2.min, -4);
    EXPECT_EQ(*value2.max, 9);

}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}