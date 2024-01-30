#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>

TEST(ComparisionOperations, equal)
{
    std::vector<int> vec{1,2,3,4,5};
    std::array<int,5> arr{1,2,3,4,5};

    EXPECT_TRUE(std::equal(vec.begin(), vec.end(),arr.begin()));
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(),arr.begin()));
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(),arr.begin(),arr.end()));

    auto checkEqual = [](int a, int b){ return a == b;};
    EXPECT_TRUE(std::equal(vec.begin(), vec.end(),arr.begin(),checkEqual));

    EXPECT_TRUE(std::ranges::equal(vec.begin(), vec.end(),arr.begin(),arr.end()));
    EXPECT_TRUE(std::ranges::equal(vec,arr));
}

TEST(ComparisionOperations, lexicographical_compare)
{
    std::string s1{"Germany"};
    std::string s2{"France"};

    EXPECT_FALSE(std::lexicographical_compare(s1.begin(),s1.end(), s2.begin(), s2.end()));

    struct Employee
    {
        std::string firstName;
        std::string lastName;
    };

    std::vector<Employee> v1{{"Alex","Don"}, {"John","Double Don"}};
    std::vector<Employee> v2{{"Kumar","Don"}, {"Leo", "Double Don"}};

    auto compare = [](const Employee&first, const Employee&second)
    {
        return (first.firstName < second.firstName);
    };

    EXPECT_TRUE(std::lexicographical_compare(v1.begin(),v1.end(),v2.begin(),v2.end(),compare));
}

int main(int argc, char* argv[])
{       
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}