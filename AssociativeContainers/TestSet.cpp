#include <iostream>
#include <gtest/gtest.h>
#include <set>

TEST(Set, MemberFunctions)
{
    std::set<int> set;
    EXPECT_TRUE(set.empty());

    std::array<int, 5> arr{1,2,3,4,5};
    std::set<int> set2(arr.begin(), arr.end());

    EXPECT_EQ(set2.size(),5);
    EXPECT_TRUE(std::ranges::equal(set2,std::initializer_list<int>({1,2,3,4,5})));

    std::array<int, 5> arr1{6,8,12,1,0};
    std::set<int> set3(arr1.begin(), arr1.end());

    EXPECT_EQ(set3.size(),5);
    EXPECT_TRUE(std::ranges::equal(set3,std::initializer_list<int>({0,1,6,8,12})));

    //use the custom comparator
    auto cmp = [](const int a, const int b) { return a > b; };
    std::set<int,decltype(cmp)> set4(arr1.begin(), arr1.end(),cmp);
    EXPECT_EQ(set4.size(),5);
    EXPECT_TRUE(std::ranges::equal(set4,std::initializer_list<int>({12,8,6,1,0})));

    std::set<int,decltype(cmp)> set5(set4);
    EXPECT_EQ(set5.size(),5);
    EXPECT_TRUE(std::ranges::equal(set5,std::initializer_list<int>({12,8,6,1,0})));

    //passing rvalue
    using iSet = std::set<int>;
    std::set<int> set6(iSet{1,2,3,4,5});
    EXPECT_EQ(set6.size(),5);
    EXPECT_TRUE(std::ranges::equal(set6,std::initializer_list<int>({1,2,3,4,5})));

    std::set<int> set7{11,33,55,88};
    EXPECT_EQ(set7.size(),4);
    EXPECT_TRUE(std::ranges::equal(set7,std::initializer_list<int>({11,33,55,88})));

    //operator = 
    std::set<int> set8 = set7;
    EXPECT_EQ(set8.size(),4);
    EXPECT_TRUE(std::ranges::equal(set8,std::initializer_list<int>({11,33,55,88})));

    std::set<int> set9 = std::move(set8);
    EXPECT_EQ(set9.size(),4);
    EXPECT_TRUE(std::ranges::equal(set9,std::initializer_list<int>({11,33,55,88})));

    std::set<int> set10 = {0,7,2,6};
    EXPECT_EQ(set10.size(),4);
    EXPECT_TRUE(std::ranges::equal(set10,std::initializer_list<int>({0,2,6,7})));
}

TEST(Set, Iterators)
{
    std::set<int> set{1,3,5,6,8,9,0};
    //The set after sorting would look like {0,1,3,5,6,8,9}
    EXPECT_EQ(*set.begin(),0);
    EXPECT_EQ(*--set.end(),9);

    auto it = set.begin();
    std::advance(it, 3);
    EXPECT_EQ(*it,5);
    it++;
    EXPECT_EQ(*it,6);

    EXPECT_EQ(*set.rbegin(),9);
    EXPECT_EQ(*--set.rend(),0);

    auto it2 = set.rend();
    std::advance(it2,-3);
    EXPECT_EQ(*it2,3);
}

TEST(Set, Capacity)
{
    std::set<int> set;
    EXPECT_TRUE(set.empty());

    set.insert(1);
    set.insert(2);

    EXPECT_FALSE(set.empty());

    EXPECT_EQ(set.size(),2);
}

TEST(Set, Modifiers)
{
    std::set<int> set{1,3,0,4};
    EXPECT_EQ(set.size(),4);
    set.clear();
    EXPECT_EQ(set.size(),0);

    //insert
    set.insert(0);
    set.insert(1);
    set.insert(2);
    EXPECT_EQ(set.size(),3);
    EXPECT_TRUE(std::ranges::equal(set,std::initializer_list<int>({0,1,2})));

    //Even though we specify the postion, the insertion happens to nearest possible position
    //and then the container is sorted. so we can expect the output as 0,1,2,4
    auto it = set.begin();
    std::advance(it, 1);
    set.insert(it,4);
    EXPECT_TRUE(std::ranges::equal(set,std::initializer_list<int>({0,1,2,4})));

    auto arr = {1,2,4,7,8};
    set.insert(arr.begin(),arr.end());
    EXPECT_TRUE(std::ranges::equal(set,std::initializer_list<int>({0,1,2,4,7,8})));

    //Extract the node and insert
    std::set<int> set1;
    set1.insert(set.extract(2));
    set1.insert(set.extract(4));
    EXPECT_TRUE(std::ranges::equal(set1,std::initializer_list<int>({2,4})));

    //emplace
    class TestConstructor
    {
        public:
            //We can also implement all other constructors like copy, move and assignment operators
            TestConstructor(int val): m_Value(val){};
            ~TestConstructor() = default; 
            
            int getValue () const{return m_Value;}
 
            bool operator<(const TestConstructor &other) const
            {
              if (m_Value < other.m_Value)
                return true;
              
              return false;
            }
        protected:
            int m_Value = 0;
    };

    std::set<TestConstructor> cSet;
    cSet.emplace(1);
    cSet.emplace(2);
    cSet.emplace(3);

    auto compare = [](const TestConstructor &obj, const int &val){ return obj.getValue() == val;};
    EXPECT_TRUE(std::ranges::equal(cSet,std::initializer_list<int>({1,2,3}),compare));

    //emplace_hint give the best position to insert directly. Bit faster  when compared with emplace
    cSet.emplace_hint(cSet.end(),0);
    cSet.emplace_hint(cSet.begin(),-1);
    cSet.emplace_hint(cSet.begin(),-2);
    EXPECT_TRUE(std::ranges::equal(cSet,std::initializer_list<int>({-2,-1,0,1,2,3}),compare));

    //erase
    cSet.erase(cSet.begin());
    EXPECT_TRUE(std::ranges::equal(cSet,std::initializer_list<int>({-1,0,1,2,3}),compare));
    auto startIt = cSet.begin();
    std::advance(startIt,1);

    auto endIt = cSet.end();
    std::advance(endIt, -2);
    cSet.erase(startIt,endIt);
    EXPECT_TRUE(std::ranges::equal(cSet,std::initializer_list<int>({-1,2,3}),compare));

    cSet.erase(2);
    EXPECT_TRUE(std::ranges::equal(cSet,std::initializer_list<int>({-1,3}),compare));

    //swap
    std::set<TestConstructor> cSet2;
    cSet2.emplace(5);
    cSet2.emplace(7);
    cSet2.emplace(1);
    cSet2.emplace(3);

    cSet2.swap(cSet);
    EXPECT_TRUE(std::ranges::equal(cSet2,std::initializer_list<int>({-1,3}),compare));
    EXPECT_TRUE(std::ranges::equal(cSet,std::initializer_list<int>({1,3,5,7}),compare));

    //merge
    cSet2.merge(cSet);
    EXPECT_TRUE(std::ranges::equal(cSet2,std::initializer_list<int>({-1,1,3,5,7}),compare));

    //extract
    auto eIt = cSet2.begin();
    eIt++;
    auto node = cSet2.extract(eIt);
    EXPECT_TRUE(std::ranges::equal(cSet2,std::initializer_list<int>({-1,3,5,7}),compare));
    EXPECT_EQ(node.value().getValue(),1);

    auto node2 = cSet2.extract(5);
    EXPECT_TRUE(std::ranges::equal(cSet2,std::initializer_list<int>({-1,3,7}),compare));
    EXPECT_EQ(node2.value().getValue(),5);

    node2.value() = TestConstructor(9);
    cSet2.insert(std::move(node2));
    EXPECT_TRUE(std::ranges::equal(cSet2,std::initializer_list<int>({-1,3,7,9}),compare));
}

TEST(Set, LookUp)
{
    std::set<int> set{1,0,2,1,2,3,5,7,12,45};
    EXPECT_EQ(set.count(9),0);
    EXPECT_EQ(set.count(1),1);

    auto it = set.find(2);
    EXPECT_EQ(*it,2);
    it = set.find(10);
    EXPECT_EQ(it,set.end());

    EXPECT_TRUE(set.contains(3));
    EXPECT_FALSE(set.contains(10));

    //returns the iterator for first element not equal to key and greater than key 
    auto [itr1, itr2] = set.equal_range(5);
    EXPECT_EQ(*itr1,5);
    EXPECT_EQ(*itr2,7);

    auto lItr = set.lower_bound(12);
    auto uItr = set.upper_bound(12);
    EXPECT_EQ(*lItr,12);
    EXPECT_EQ(*uItr,45);

}

TEST(Set,Observers)
{
    std::set<int> set{1,2,0,3,5,4};
    auto key_comp = set.key_comp();
    //Returns the default comparator used in set. std::less<<>> is the standard comparator
    EXPECT_TRUE(key_comp(1,2));
    EXPECT_FALSE(key_comp(2,1));

    auto value_comp = set.value_comp();
    EXPECT_TRUE(value_comp(1,2));
    EXPECT_FALSE(value_comp(2,1));
}

TEST(Set, NonMemberFunctions)
{
    std::set<int> set1{1,2,3,4};
    std::set<int> set2{1,2,3,4,5};
    std::set<int> set3{1,2,3,4};

    EXPECT_TRUE(set1==set3);
    EXPECT_FALSE(set1==set2);

    EXPECT_TRUE(set1 < set2);
    EXPECT_TRUE(set2 > set1);

    EXPECT_TRUE(set1 <= set2);
    EXPECT_TRUE(set2 >= set1);

    std::swap(set1, set2);
    EXPECT_TRUE(std::equal(set1.begin(), set1.end(), std::begin({1,2,3,4,5})));
    EXPECT_TRUE(std::equal(set2.begin(), set2.end(), std::begin({1,2,3,4})));

    //The set3 would have values of {1,2,3,4}
    // Now delete all even numbers by using predicate
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(set3, deleteEvenNumbers);
    EXPECT_TRUE(std::equal(set3.begin(), set3.end(), std::begin({1,3})));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}