#include <iostream>
#include <gtest/gtest.h>
#include <set>

TEST(MultiSet, MemberFunctions)
{
    std::multiset<int> mSet;
    EXPECT_TRUE(mSet.empty());

    std::array<int, 5> arr{1,2,3,4,5};
    std::multiset<int> mSet2(arr.begin(), arr.end());

    EXPECT_EQ(mSet2.size(),5);
    EXPECT_TRUE(std::ranges::equal(mSet2,std::initializer_list<int>({1,2,3,4,5})));

    std::array<int, 5> arr1{6,8,12,1,0};
    std::multiset<int> mSet3(arr1.begin(), arr1.end());

    EXPECT_EQ(mSet3.size(),5);
    EXPECT_TRUE(std::ranges::equal(mSet3,std::initializer_list<int>({0,1,6,8,12})));

    //use the custom comparator
    auto cmp = [](const int a, const int b) { return a > b; };
    std::multiset<int,decltype(cmp)> mSet4(arr1.begin(), arr1.end(),cmp);
    EXPECT_EQ(mSet4.size(),5);
    EXPECT_TRUE(std::ranges::equal(mSet4,std::initializer_list<int>({12,8,6,1,0})));

    std::multiset<int,decltype(cmp)> mSet5(mSet4);
    EXPECT_EQ(mSet5.size(),5);
    EXPECT_TRUE(std::ranges::equal(mSet5,std::initializer_list<int>({12,8,6,1,0})));

    //passing rvalue
    using iSet = std::multiset<int>;
    std::multiset<int> mSet6(iSet{1,2,3,4,5});
    EXPECT_EQ(mSet6.size(),5);
    EXPECT_TRUE(std::ranges::equal(mSet6,std::initializer_list<int>({1,2,3,4,5})));

    std::multiset<int> mSet7{11,33,55,88};
    EXPECT_EQ(mSet7.size(),4);
    EXPECT_TRUE(std::ranges::equal(mSet7,std::initializer_list<int>({11,33,55,88})));

    //operator = 
    std::multiset<int> mSet8 = mSet7;
    EXPECT_EQ(mSet8.size(),4);
    EXPECT_TRUE(std::ranges::equal(mSet8,std::initializer_list<int>({11,33,55,88})));

    std::multiset<int> mSet9 = std::move(mSet8);
    EXPECT_EQ(mSet9.size(),4);
    EXPECT_TRUE(std::ranges::equal(mSet9,std::initializer_list<int>({11,33,55,88})));

    std::multiset<int> mSet10 = {0,7,2,6,0,7};
    EXPECT_EQ(mSet10.size(),6);
    EXPECT_TRUE(std::ranges::equal(mSet10,std::initializer_list<int>({0,0,2,6,7,7})));
}

TEST(MultiSet, Iterators)
{
    std::multiset<int> mSet{1,3,5,6,8,9,0,3};
    //The set after sorting would look like {0,1,3,3,5,6,8,9}
    EXPECT_EQ(*mSet.begin(),0);
    EXPECT_EQ(*--mSet.end(),9);

    auto it = mSet.begin();
    std::advance(it, 3);
    EXPECT_EQ(*it,3);
    it++;
    EXPECT_EQ(*it,5);

    EXPECT_EQ(*mSet.rbegin(),9);
    EXPECT_EQ(*--mSet.rend(),0);

    auto it2 = mSet.rend();
    std::advance(it2,-3);
    EXPECT_EQ(*it2,3);
}

TEST(MultiSet, Capacity)
{
    std::multiset<int> mSet;
    EXPECT_TRUE(mSet.empty());

    mSet.insert(1);
    mSet.insert(2);
    mSet.insert(2);

    EXPECT_FALSE(mSet.empty());

    EXPECT_EQ(mSet.size(),3);
}

TEST(MultiSet, Modifiers)
{
    std::multiset<int> mSet{1,3,0,4,4};
    EXPECT_EQ(mSet.size(),5);
    mSet.clear();
    EXPECT_EQ(mSet.size(),0);

    //insert
    mSet.insert(0);
    mSet.insert(1);
    mSet.insert(2);
    EXPECT_EQ(mSet.size(),3);
    EXPECT_TRUE(std::ranges::equal(mSet,std::initializer_list<int>({0,1,2})));

    //Even though we specify the postion, the insertion happens to nearest possible position
    //and then the container is sorted. so we can expect the output as 0,1,2,4
    auto it = mSet.begin();
    std::advance(it, 1);
    mSet.insert(it,2);
    EXPECT_TRUE(std::ranges::equal(mSet,std::initializer_list<int>({0,1,2,2})));

    auto arr = {1,2,4,7,8};
    mSet.insert(arr.begin(),arr.end());
    EXPECT_TRUE(std::ranges::equal(mSet,std::initializer_list<int>({0,1,1,2,2,2,4,7,8})));

    //Extract the node and insert
    std::multiset<int> mSet1;
    mSet1.insert(mSet.extract(2));
    mSet1.insert(mSet.extract(4));
    EXPECT_TRUE(std::ranges::equal(mSet1,std::initializer_list<int>({2,4})));

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

    std::multiset<TestConstructor> cMSet;
    cMSet.emplace(1);
    cMSet.emplace(2);
    cMSet.emplace(3);

    auto compare = [](const TestConstructor &obj, const int &val){ return obj.getValue() == val;};
    EXPECT_TRUE(std::ranges::equal(cMSet,std::initializer_list<int>({1,2,3}),compare));

    //emplace_hint give the best position to insert directly. Bit faster  when compared with emplace
    cMSet.emplace_hint(cMSet.end(),0);
    cMSet.emplace_hint(cMSet.begin(),-1);
    cMSet.emplace_hint(cMSet.begin(),-2);
    cMSet.emplace_hint(cMSet.begin(),0);
    EXPECT_TRUE(std::ranges::equal(cMSet,std::initializer_list<int>({-2,-1,0,0,1,2,3}),compare));

    //erase
    cMSet.erase(cMSet.begin());
    EXPECT_TRUE(std::ranges::equal(cMSet,std::initializer_list<int>({-1,0,0,1,2,3}),compare));
    
    auto startIt = cMSet.begin();
    std::advance(startIt,1);
    auto endIt = cMSet.end();
    std::advance(endIt, -2);

    cMSet.erase(startIt,endIt);
    EXPECT_TRUE(std::ranges::equal(cMSet,std::initializer_list<int>({-1,2,3}),compare));

    cMSet.erase(2);
    EXPECT_TRUE(std::ranges::equal(cMSet,std::initializer_list<int>({-1,3}),compare));

    //swap
    std::multiset<TestConstructor> cMSet2;
    cMSet2.emplace(5);
    cMSet2.emplace(7);
    cMSet2.emplace(1);
    cMSet2.emplace(3);
    cMSet2.emplace(3);

    cMSet2.swap(cMSet);
    EXPECT_TRUE(std::ranges::equal(cMSet2,std::initializer_list<int>({-1,3}),compare));
    EXPECT_TRUE(std::ranges::equal(cMSet,std::initializer_list<int>({1,3,3,5,7}),compare));

    //merge
    cMSet2.merge(cMSet);
    EXPECT_TRUE(std::ranges::equal(cMSet2,std::initializer_list<int>({-1,1,3,3,3,5,7}),compare));

    //extract
    auto eIt = cMSet2.begin();
    eIt++;
    auto node = cMSet2.extract(eIt);
    EXPECT_TRUE(std::ranges::equal(cMSet2,std::initializer_list<int>({-1,3,3,3,5,7}),compare));
    EXPECT_EQ(node.value().getValue(),1);

    auto node2 = cMSet2.extract(5);
    EXPECT_TRUE(std::ranges::equal(cMSet2,std::initializer_list<int>({-1,3,3,3,7}),compare));
    EXPECT_EQ(node2.value().getValue(),5);

    node2.value() = TestConstructor(9);
    cMSet2.insert(std::move(node2));
    EXPECT_TRUE(std::ranges::equal(cMSet2,std::initializer_list<int>({-1,3,3,3,7,9}),compare));
}

TEST(MultiSet, LookUp)
{
    std::multiset<int> mSet{1,0,2,1,2,3,5,7,12,45};
    EXPECT_EQ(mSet.count(9),0);
    EXPECT_EQ(mSet.count(1),2);

    auto it = mSet.find(2);
    EXPECT_EQ(*it,2);
    it++;
    EXPECT_EQ(*it,2);
    it = mSet.find(10);
    EXPECT_EQ(it,mSet.end());

    EXPECT_TRUE(mSet.contains(3));
    EXPECT_FALSE(mSet.contains(10));

    //returns the iterator for first element not equal to key and greater than key 
    auto [itr1, itr2] = mSet.equal_range(2);
    EXPECT_EQ(*itr1,2);
    EXPECT_EQ(*itr2,3);

    auto lItr = mSet.lower_bound(12);
    auto uItr = mSet.upper_bound(12);
    EXPECT_EQ(*lItr,12);
    EXPECT_EQ(*uItr,45);

}

TEST(MultiSet,Observers)
{
    std::multiset<int> mSet{1,2,0,3,5,4};
    auto key_comp = mSet.key_comp();
    //Returns the default comparator used in set. std::less<<>> is the standard comparator
    EXPECT_TRUE(key_comp(1,2));
    EXPECT_FALSE(key_comp(2,1));

    auto value_comp = mSet.value_comp();
    EXPECT_TRUE(value_comp(1,2));
    EXPECT_FALSE(value_comp(2,1));
}

TEST(MultiSet, NonMemberFunctions)
{
    std::multiset<int> mSet1{1,2,3,3,4};
    std::multiset<int> mSet2{1,2,3,4,5};
    std::multiset<int> mSet3{1,2,3,3,4};

    EXPECT_TRUE(mSet1==mSet3);
    EXPECT_FALSE(mSet1==mSet2);

    EXPECT_TRUE(mSet1 < mSet2);
    EXPECT_TRUE(mSet2 > mSet1);

    EXPECT_TRUE(mSet1 <= mSet2);
    EXPECT_TRUE(mSet2 >= mSet1);

    std::swap(mSet1, mSet2);
    EXPECT_TRUE(std::equal(mSet1.begin(), mSet1.end(), std::begin({1,2,3,4,5})));
    EXPECT_TRUE(std::equal(mSet2.begin(), mSet2.end(), std::begin({1,2,3,3,4})));

    //The set3 would have values of {1,2,3,4}
    // Now delete all even numbers by using predicate
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(mSet3, deleteEvenNumbers);
    EXPECT_TRUE(std::equal(mSet3.begin(), mSet3.end(), std::begin({1,3,3})));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}