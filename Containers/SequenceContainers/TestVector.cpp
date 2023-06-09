#include <iostream>
#include <gtest/gtest.h>
#include <vector>

TEST(VectorTest, Constructor)
{   
    // Uncomment the below macro to run this test
    //GTEST_SKIP();

    //Empty constructor
    std::vector<int> vec1;
    EXPECT_EQ(vec1.size(), 0);

    //constructor with 3 elements with value 5
    std::vector<int> vec2(3, 5);
    EXPECT_EQ(vec2.size(), 3);
    for(auto val:vec2)
        ASSERT_EQ(val, 5);

    //constructor with vector size
    std::vector<int> vec3(3);
    EXPECT_EQ(vec3.size(), 3);
    for(auto val:vec3)
        ASSERT_EQ(val, 0);

    //constructor with first and last range
    std::vector<int> vec4(vec3.begin(), vec3.end());
    EXPECT_EQ(vec4.size(), 3);
    for(auto val:vec4)
        ASSERT_EQ(val, 0);

    //vector copy consturctor
    std::vector<int> vec5(vec2);
    EXPECT_EQ(vec5.size(), 3);
    for(auto val:vec5)
        ASSERT_EQ(val, 5);

    //constructor with initilizer list
    std::vector<int> vec6{};
    EXPECT_EQ(vec6.size(), 0);

    //constructor with initilizer list with values
    std::vector<int> vec7{1,2,3,4,5};
    EXPECT_EQ(vec7.size(), 5);
    
    //copy the vector using initilizer list
    std::vector<int> vec8{vec7};
    EXPECT_EQ(vec8.size(), 5);
    
}

TEST(VectorTest, AssignmentOperator)
{
    GTEST_SKIP();

    //Assignment operator 
    std::vector<int> vec1(5, 5);
    auto vec2 = vec1;
    EXPECT_EQ(vec2.size(), 5);
    for(auto &val:vec2)
        ASSERT_EQ(val,5);

    //using move assignment 
    auto vec3 = std::move(vec2);
    EXPECT_EQ(vec3.size(), 5);
    for(auto &val:vec3)
        ASSERT_EQ(val,5);

    //using initilizer list assignment
    auto vec4 = {1,2,3,4,5,6};
    auto vecCopy = {1,2,4,5,6,3};
    EXPECT_EQ(vec4.size(), 6);
    EXPECT_TRUE(std::is_permutation(vec4.begin(), vec4.end(), vecCopy.begin()));   
}

TEST(VectorTest, Assign)
{
    GTEST_SKIP();

    std::vector<int> vec;
    vec.assign(5,3);
    EXPECT_EQ(vec.size(), 5);
    for(auto &val:vec)
        ASSERT_EQ(val,3);

    std::vector<int> vec2;
    vec2.assign(vec.begin(), vec.end());
    EXPECT_EQ(vec2.size(), 5);
    for(auto &val:vec2)
        ASSERT_EQ(val,3);

    std::vector<int> vec3;
    vec3.assign({1,1,1,1,1});
    EXPECT_EQ(vec3.size(), 5);
    for(auto &val:vec3)
        ASSERT_EQ(val,1);
}

TEST(VectorTest, ElementsAccess)
{
    GTEST_SKIP();

    std::vector<int> vec{1,12,13,14,15};
    EXPECT_EQ(vec.at(2), 13);
    vec.at(3) = 0;
    EXPECT_EQ(vec.at(3), 0);
    EXPECT_ANY_THROW(vec.at(8));

    EXPECT_EQ(vec[0], 1);
    vec[0] = 100;
    EXPECT_EQ(vec[0], 100);

    EXPECT_EQ(vec.front(), 100);
    EXPECT_EQ(vec.back(), 15);

    auto rawData = vec.data();
    EXPECT_EQ(rawData[0], 100);
    EXPECT_EQ(rawData[vec.size() - 1], 15);
}

TEST(VectorTest, Iterators)
{
    GTEST_SKIP();

    std::vector<int> vec{1,7,3,4,5};
    auto iter = vec.begin();
    EXPECT_EQ(*iter, 1);
    iter++;
    EXPECT_EQ(*iter,7);
    --iter;
    EXPECT_EQ(*iter, 1);

    auto iterEnd = vec.end();
    EXPECT_EQ(*iterEnd, 0);
    --iterEnd;
    EXPECT_EQ(*iterEnd, 5);

    EXPECT_EQ(*vec.rbegin(),5);
    EXPECT_EQ(*vec.rend(),0);
    EXPECT_EQ(*--vec.rend(), 1);
}

TEST(VectorTest, Capacity)
{
    GTEST_SKIP();

    std::vector<int> vec;
    
    EXPECT_TRUE(vec.empty());

    vec.assign({1,2,3,4,5});
    EXPECT_EQ(vec.size(), 5);

    vec.reserve(10);
    EXPECT_EQ(vec.capacity(),10);
    EXPECT_EQ(vec.size(), 5);

    vec.clear();
    EXPECT_EQ(vec.capacity(), 10);
    EXPECT_EQ(vec.size(), 0);

    vec.reserve(100);
    vec.assign({1,1,1});
    vec.shrink_to_fit();
    EXPECT_EQ(vec.capacity(),3);
    EXPECT_EQ(vec.size(), 3);
}

TEST(VectorTest, Modifiers)
{
    GTEST_SKIP();

    std::vector<int> vec{1,2,3};
    //Test insert all types
    EXPECT_EQ(vec.size(), 3);
    vec.clear();
    EXPECT_EQ(vec.size(),0);

    vec.assign({1,3});
    auto it = vec.begin();
    it++;
    vec.insert(it,2);
    EXPECT_EQ(vec.size(), 3);

    auto data = 1;
    for(auto &val:vec)
    {
       ASSERT_EQ(val, data);
       data++;
    }

    auto end = vec.end();
    vec.insert(end, 4, 4);
    EXPECT_EQ(vec.size(),7);

    std::vector<int> vec1{11,22,33};
    vec1.insert(vec1.begin()+1, vec.begin(), vec.end());
    EXPECT_EQ(vec1.size(), 10);
    int expected[] = {11,1,2,3,4,4,4,4,22,33};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected));

    vec1.insert(vec1.end(), {44,55});
    EXPECT_EQ(vec1.size(),12);
    int expected1[] = {11,1,2,3,4,4,4,4,22,33,44,55};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected1));

    //Test emplace
    std::vector<int> vec2{111,222,444};
    vec2.emplace(vec2.begin()+2, 333);
    int expected2[] = {111,222,333,444};
    EXPECT_TRUE(std::equal(vec2.begin(), vec2.end(), expected2));

    //Test erase
    vec2.erase(vec2.begin());
    int expected3[] = {222,333,444};
    EXPECT_TRUE(std::equal(vec2.begin(), vec2.end(), expected3));

    vec1.erase(vec1.begin()+1, vec1.begin()+8);
    int expected4[] = {11,22,33,44,55};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected4));

    //Test push and emplace back
    vec2.push_back(555);
    vec2.push_back(666);
    int expected5[] = {222,333,444,555,666};
    EXPECT_TRUE(std::equal(vec2.begin(), vec2.end(), expected5));

    vec1.emplace_back(66);
    vec1.emplace_back(77);
    int expected6[] = {11,22,33,44,55,66,77};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected6));

    //Test pop and resize
    vec2.pop_back();
    vec2.pop_back();
    vec2.pop_back();
    vec2.pop_back();
    EXPECT_EQ(vec2.size(), 1);
    EXPECT_EQ(*vec2.begin(), 222);

    vec1.resize(2);
    EXPECT_EQ(vec1.size(),2);
    vec1.resize(5);
    int expected7[] = {11,22,0,0,0};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected7));
    vec1.resize(10,5);
    int expected8[] = {11,22,0,0,0,5,5,5,5,5};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected8));
}

TEST(VectorTest, NonMemberFunctions)
{
    std::vector<int> vec1{1,2,3,4};
    std::vector<int> vec2{1,2,3,4,5};
    std::vector<int> vec3{1,2,3,4};

    EXPECT_TRUE(vec1==vec3);
    EXPECT_FALSE(vec1==vec2);

    EXPECT_TRUE(vec1 < vec2);
    EXPECT_TRUE(vec2 > vec1);

    EXPECT_TRUE(vec1 <= vec2);
    EXPECT_TRUE(vec2 >= vec1);

    std::swap(vec1, vec2);
    int expected1[] = {1,2,3,4};
    int expected2[] = {1,2,3,4,5};
    EXPECT_TRUE(std::equal(vec1.begin(), vec1.end(), expected2));
    EXPECT_TRUE(std::equal(vec2.begin(), vec2.end(), expected1));

    vec3.push_back(4);
    vec3.push_back(4);    
    vec3.push_back(5);

    std::erase(vec3,4);
    int expected3[] = {1,2,3,5};
    EXPECT_TRUE(std::equal(vec3.begin(), vec3.end(), expected3));

    vec3.push_back(6);
    vec3.push_back(8);
    //The vector would have values of {1,2,3,5,6,8}
    // Now delete all even numbers by using predicate
    int expected4[] = {1,3,5};
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(vec3, deleteEvenNumbers);
    EXPECT_TRUE(std::equal(vec3.begin(), vec3.end(), expected4));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}