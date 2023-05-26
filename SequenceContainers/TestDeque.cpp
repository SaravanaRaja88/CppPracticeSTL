#include <iostream>
#include <gtest/gtest.h>
#include <deque>
#include <array>


TEST(TestDeque, MemberFunctions)
{
    //constructors
    std::deque<int> deq;
    EXPECT_TRUE(deq.empty());

    std::deque<int> deq1(3, 5);
    EXPECT_TRUE(std::equal(deq1.begin(), deq1.end(), std::begin({5,5,5})));

    std::deque<int> deq2(5);
    EXPECT_TRUE(std::equal(deq2.begin(), deq2.end(), std::begin({0,0,0,0,0})));
    EXPECT_EQ(deq2.size(),5);

    std::deque<int> deq3(deq1.begin(), deq1.end());
    EXPECT_TRUE(std::equal(deq3.begin(), deq3.end(), std::begin({5,5,5})));

    std::deque<int> deq4(deq3);
    EXPECT_TRUE(std::equal(deq4.begin(), deq4.end(), std::begin({5,5,5})));

    std::deque<int> deq5(std::move(deq4));
    EXPECT_TRUE(std::equal(deq5.begin(), deq5.end(), std::begin({5,5,5})));

    std::deque<int> deq6{1,2,3,4,5};
    EXPECT_EQ(deq6.size(),5);
    EXPECT_TRUE(std::equal(deq6.begin(), deq6.end(), std::begin({1,2,3,4,5})));

    // = operator
    std::deque<int> opDeque{1,2,3,4,5};
    std::deque<int> cpDeque;

    cpDeque = opDeque;
    EXPECT_EQ(cpDeque.size(),5);
    EXPECT_TRUE(std::equal(cpDeque.begin(), cpDeque.end(), std::begin({1,2,3,4,5})));

    std::deque<int> mvDeque = std::move(cpDeque);
    EXPECT_EQ(mvDeque.size(),5);
    EXPECT_TRUE(std::equal(mvDeque.begin(), mvDeque.end(), std::begin({1,2,3,4,5})));

    std::deque<int> iniDeque = {1,1,1,1};
    EXPECT_EQ(iniDeque.size(),4);
    EXPECT_TRUE(std::equal(iniDeque.begin(), iniDeque.end(), std::begin({1,1,1,1})));

    // Assign
    std::deque<int> assignDeque;
    assignDeque.assign(5,4);
    EXPECT_EQ(assignDeque.size(),5);
    EXPECT_TRUE(std::equal(assignDeque.begin(), assignDeque.end(), std::begin({4,4,4,4,4})));

    auto arr = std::to_array({5,5,5});
    assignDeque.assign(arr.begin(),arr.end());
    EXPECT_EQ(assignDeque.size(),3);
    EXPECT_TRUE(std::equal(assignDeque.begin(), assignDeque.end(), std::begin({5,5,5})));

    assignDeque.assign({11,22,33,44,55});
    EXPECT_EQ(assignDeque.size(),5);
    EXPECT_TRUE(std::equal(assignDeque.begin(), assignDeque.end(), std::begin({11,22,33,44,55})));

}

TEST(TestDeque, ElementAccess)
{
    std::deque<int> deq = { 1, 2, 4, 5, 5, 6 };
    deq.at(2) = 3;
    deq.at(3) = 4;
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,2,3,4,5,6})));
    EXPECT_EQ(deq.at(5),6);
    EXPECT_ANY_THROW(deq.at(10));

    deq[0] = 2;
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({2,2,3,4,5,6})));
    EXPECT_EQ(deq[5],6);

    EXPECT_EQ(deq.front(),2);
    EXPECT_EQ(deq.back(),6);
}

TEST(TestDeque, Iterators)
{
    std::deque<int> deq{11,22,33,44,55};
    EXPECT_EQ(*deq.begin(), 11);
    auto it = deq.begin(); it++;
    EXPECT_EQ(*it, 22);

    EXPECT_EQ(*--deq.end(),55);
    auto eIt = deq.end(); --eIt;
    --eIt;
    EXPECT_EQ(*eIt,44);

    EXPECT_EQ(*deq.rbegin(),55);
    EXPECT_EQ(*(deq.rend()-2),22);

}

TEST(TestDeque, Capacity)
{
    std::deque<int> deq;
    EXPECT_TRUE(deq.empty());
    EXPECT_EQ(deq.size(), 0);

    deq.assign({1,2,3});
    EXPECT_EQ(deq.size(), 3);

    std::deque<int> deq1(10);
    deq1.assign({1,2,3});
    EXPECT_EQ(deq1.size(), 3);

    deq1.pop_back();
    deq1.shrink_to_fit();
    EXPECT_EQ(deq1.size(), 2);
}

TEST(TestDeque, Modifiers)
{
    //insert
    std::deque<int> deq{1,2,3,4,5};
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,2,3,4,5})));
    EXPECT_EQ(deq.size(), 5);

    deq.clear();
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({0})));
    EXPECT_EQ(deq.size(), 0);

    deq.insert(deq.begin(), 1);
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1})));

    deq.insert(deq.end(), 11);
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,11})));

    deq.insert(deq.begin()+1, 2, 2);
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,2,2,11})));

    std::array<int,3> arr{3,3,3};
    deq.insert(deq.end(),arr.begin(), arr.end());
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,2,2,11,3,3,3})));

    deq.insert(deq.end(), {4,4,4});
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,2,2,11,3,3,3,4,4,4})));

    deq.insert(deq.begin()+1, {-1,-2,-3});
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,-1,-2,-3,2,2,11,3,3,3,4,4,4})));


    //erase
    deq.erase(deq.begin());
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({-1,-2,-3,2,2,11,3,3,3,4,4,4})));
    
    deq.erase(deq.begin(),deq.end());
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({0})));
    EXPECT_EQ(deq.size(),0);

    //emplace
    deq.emplace(deq.begin(),1);
    deq.emplace(deq.begin()+1, 2);
    EXPECT_TRUE(std::equal(deq.begin(), deq.end(), std::begin({1,2})));
    EXPECT_EQ(deq.size(),2);

    class TestConstructor
    {
        public:
            //We can also implement all other constructors like copy, move and assignment operators
            TestConstructor(int val): m_Value(val){};
            ~TestConstructor() = default; 
            
            int getValue(){return m_Value;}
        protected:
            int m_Value = 0;
    };

    std::deque<TestConstructor> conDeque;
    conDeque.emplace(conDeque.begin(),1);
    conDeque.emplace(conDeque.end(),2);
    conDeque.emplace(conDeque.end(),3);

    auto compare = [](TestConstructor &obj, int val){ return obj.getValue() == val;};
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({1,2,3}), compare));

    //The difference between the push_back and the emplace_back is that push back creates 
    //temporary copy and then moves whereas emplace forwards the args to constructor.
    conDeque.push_back(TestConstructor(4));
    conDeque.push_back(TestConstructor(5));
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({1,2,3,4,5}), compare));

    conDeque.emplace_back(6);
    conDeque.emplace_back(7);
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({1,2,3,4,5,6,7}), compare));

    conDeque.push_front(TestConstructor(0));
    conDeque.push_front(TestConstructor(-1));
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({-1,0,1,2,3,4,5,6,7}), compare));

    conDeque.emplace_front(-11);
    conDeque.emplace_front(-22);
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({-22,-11,-1,0,1,2,3,4,5,6,7}), compare));

    conDeque.pop_front();
    conDeque.pop_front();
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({-1,0,1,2,3,4,5,6,7}), compare));

    conDeque.pop_back();
    conDeque.pop_back();
    EXPECT_TRUE(std::equal(conDeque.begin(), conDeque.end(), std::begin({-1,0,1,2,3,4,5}), compare));
    
    //resize
    std::deque<int> deq1{1,2,3};
    EXPECT_EQ(deq1.size(),3);

    deq1.resize(10);
    EXPECT_EQ(deq1.size(),10);
    EXPECT_TRUE(std::equal(deq1.begin(), deq1.end(), std::begin({1,2,3,0,0,0,0,0,0,0})));

    deq1.resize(5,4);
    EXPECT_EQ(deq1.size(),5);
    EXPECT_TRUE(std::equal(deq1.begin(), deq1.end(), std::begin({1,2,3,0,0})));

    deq1.resize(10,4);
    EXPECT_EQ(deq1.size(),10);
    EXPECT_TRUE(std::equal(deq1.begin(), deq1.end(), std::begin({1,2,3,0,0,4,4,4,4,4})));

    //swap
    std::deque<int> deq2{11,22,33};
    deq2.swap(deq1);
    EXPECT_TRUE(std::equal(deq2.begin(), deq2.end(), std::begin({1,2,3,0,0,4,4,4,4,4})));
    EXPECT_TRUE(std::equal(deq1.begin(), deq1.end(), std::begin({11,22,33})));
}

TEST(TestDeque, NonMemberFunctions)
{
    std::deque<int> deq1{1,2,3,4};
    std::deque<int> deq2{1,2,3,4,5};
    std::deque<int> deq3{1,2,3,4};

    EXPECT_TRUE(deq1==deq3);
    EXPECT_FALSE(deq1==deq2);

    EXPECT_TRUE(deq1 < deq2);
    EXPECT_TRUE(deq2 > deq1);

    EXPECT_TRUE(deq1 <= deq2);
    EXPECT_TRUE(deq2 >= deq1);

    std::swap(deq1, deq2);
    EXPECT_TRUE(std::equal(deq1.begin(), deq1.end(), std::begin({1,2,3,4,5})));
    EXPECT_TRUE(std::equal(deq2.begin(), deq2.end(), std::begin({1,2,3,4})));

    deq3.push_back(4);
    deq3.push_back(4);    
    deq3.push_back(5);

    std::erase(deq3,4);
    EXPECT_TRUE(std::equal(deq3.begin(), deq3.end(), std::begin({1,2,3,5})));

    deq3.push_back(6);
    deq3.push_back(8);
    //The deque would have values of {1,2,3,5,6,8}
    // Now delete all even numbers by using predicate
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(deq3, deleteEvenNumbers);
    EXPECT_TRUE(std::equal(deq3.begin(), deq3.end(), std::begin({1,3,5})));
}


int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}   