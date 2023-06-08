#include <iostream>
#include <gtest/gtest.h>
#include <unordered_set>

bool compareContainers(auto &container1, auto &&container2)
{
    for(auto &key:container1)
        if(!container2.contains(key))
            return false;
    return true;
}

bool compareUserDefinedValuesContainers(auto &container1, auto &&container2)
{
    for(auto &key:container1)
        if(!container2.contains(key.getVal()))
            return false;
    return true;
}

TEST(UnorderedSet, MemberFunctions)
{
    std::unordered_set<int> uSet;
    EXPECT_TRUE(uSet.empty());

    std::array<int, 5> arr{1,3,5,2,4};
    std::unordered_set<int> uSet2(arr.begin(), arr.end());

    EXPECT_EQ(uSet2.size(),5);
    
    //To check the elements of the unordered_set, we just compare each element with another 
    //container which stores the expected result
    EXPECT_TRUE(compareContainers(uSet2,std::set<int>({1,2,3,4,5})));

    //User defined type
    class Test
    {
    public:
        int val;
    public:
        Test(int v):val(v){};
        ~Test(){};

        int getVal() const{return val;}
    };

    //use the custom comparator
    auto cmp = [](const Test &a, const Test &b) { return a.getVal() == b.getVal(); };

    //use the custom hash
    struct hasher
    {
        size_t operator()(const Test &x) const
        {
            return std::hash<int>()(x.getVal());
        }
    };

    std::array<Test,4> tArr{Test(1),Test(2),Test(3),Test(3)};

    //unordered set to work with custom data type the hash and comparator has to be given
    std::unordered_set<Test,hasher,decltype(cmp)> uSet3(tArr.begin(), tArr.end());
    uSet3.insert(Test(2));
    EXPECT_EQ(uSet3.size(),3);
    EXPECT_TRUE(compareUserDefinedValuesContainers(uSet3,std::set<int>({1,2,3})));

    std::unordered_set<Test,hasher,decltype(cmp)> uSet4(uSet3);
    EXPECT_EQ(uSet4.size(),3);
    EXPECT_TRUE(compareUserDefinedValuesContainers(uSet3,std::set<int>({1,2,3})));

    //passing rvalue
    using iSet = std::unordered_set<int>;
    std::unordered_set<int> uSet5(iSet{1,2,3,4,5});
    EXPECT_EQ(uSet5.size(),5);
    EXPECT_TRUE(compareContainers(uSet5,std::set<int>({1,2,3,4,5})));

    std::unordered_set<Test,hasher,decltype(cmp)> uSet6{Test(11),Test(22),Test(33),Test(33)};
    EXPECT_EQ(uSet6.size(),3);
    EXPECT_TRUE(compareUserDefinedValuesContainers(uSet6,std::set<int>({11,22,33})));

    //operator = 
    std::unordered_set<int> uSet7 = uSet5;
    EXPECT_EQ(uSet7.size(),5);
    EXPECT_TRUE(compareContainers(uSet7,std::set<int>({1,2,3,4,5})));

    std::unordered_set<int> uSet8 = std::move(uSet7);
    EXPECT_EQ(uSet8.size(),5);
    EXPECT_TRUE(compareContainers(uSet8,std::set<int>({1,2,3,4,5})));

    std::unordered_set<int> uSet9 = {0,7,2,6};
    EXPECT_EQ(uSet9.size(),4);
    EXPECT_TRUE(compareContainers(uSet9,std::set<int>({0,7,6,2})));
}

TEST(UnorderedSet, Iterators)
{
    std::unordered_set<int> uSet{1,3,5,6,8,9,0};

    //Most of the time the last element in unorder set will be first on my system.
    //so just using this value.  But there is not true in unordered set.
    EXPECT_EQ(*uSet.begin(),0);

    //Due to no reverse iterators we cannot advance using the end() iterator.
    //so the uSet.end() can be used only for boundary checks. 

}

TEST(UnorderedSet, Capacity)
{
    std::unordered_set<int> uSet;
    EXPECT_TRUE(uSet.empty());

    uSet.insert(1);
    uSet.insert(2);
    //will be not inserted since key is same
    uSet.insert(2);

    EXPECT_FALSE(uSet.empty());

    EXPECT_EQ(uSet.size(),2);
}


TEST(UnorderedSet, Modifiers)
{
    std::unordered_set<int> uSet{1,3,0,4};
    EXPECT_EQ(uSet.size(),4);
    uSet.clear();
    EXPECT_EQ(uSet.size(),0);

    //insert
    uSet.insert(0);
    uSet.insert(1);
    uSet.insert(2);
    EXPECT_EQ(uSet.size(),3);
    EXPECT_TRUE(compareContainers(uSet,std::set<int>({0,1,2})));

    //Even though we specify the postion, the insertion happens to nearest possible position
    auto it = uSet.begin();
    std::advance(it, 1);
    uSet.insert(it,4);
    EXPECT_TRUE(compareContainers(uSet,std::set<int>({0,1,2,4})));

    auto arr = {1,2,4,7,8};
    uSet.insert(arr.begin(),arr.end());
    EXPECT_TRUE(compareContainers(uSet,std::set<int>({0,1,2,4,7,8})));

    //Extract the node and insert
    std::unordered_set<int> uSet1;
    uSet1.insert(uSet.extract(2));
    uSet1.insert(uSet.extract(4));
    EXPECT_TRUE(compareContainers(uSet1,std::set<int>({2,4})));

    //emplace
    class TestConstructor
    {
        public:
            //We can also implement all other constructors like copy, move and assignment operators
            TestConstructor(int val): m_Value(val){};
            ~TestConstructor() = default; 
            
            int getVal () const{return m_Value;}

        protected:
            int m_Value = 0;
    };

    //use the custom comparator
    auto cmp = [](const TestConstructor &a, const TestConstructor &b) { return a.getVal() == b.getVal(); };

    //use the custom hash
    struct hasher
    {
        size_t operator()(const TestConstructor &x) const
        {
            return std::hash<int>()(x.getVal());
        }
    };

    std::unordered_set<TestConstructor,hasher,decltype(cmp)> cUSet;
    cUSet.emplace(1);
    cUSet.emplace(2);
    cUSet.emplace(3);

    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet,std::set<int>({1,2,3})));

    //emplace_hint give the best position to insert directly. Bit faster  when compared with emplace
    cUSet.emplace_hint(cUSet.end(),0);
    cUSet.emplace_hint(cUSet.begin(),-1);
    cUSet.emplace_hint(cUSet.begin(),-2);
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet,std::set<int>({1,2,3,0,-1,-2})));

    //erase delets the first key
    cUSet.erase(cUSet.begin());
    EXPECT_EQ(cUSet.size(),5);
    auto startIt = cUSet.begin();
    std::advance(startIt,2);

    //deletes full set leaving 2 elements.
    auto endIt = cUSet.end();
    cUSet.erase(startIt,endIt);
    EXPECT_EQ(cUSet.size(),2);

    //after manually checking for my run i see 3 and -1 elements left. This values can be differenct since 
    //we are operating on unsorted set.
    //This is just demo to see if we can erase by using the key also.
    cUSet.erase(3);
    EXPECT_EQ(cUSet.size(),1);


    //swap
    std::unordered_set<TestConstructor,hasher,decltype(cmp)> cUSet2;
    cUSet2.emplace(5);
    cUSet2.emplace(7);
    cUSet2.emplace(1);
    cUSet2.emplace(3);

    cUSet2.swap(cUSet);
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet2,std::set<int>({-1})));
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet,std::set<int>({1,3,5,7})));

    //merge
    cUSet2.merge(cUSet);
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet,std::set<int>({-1,1,3,5,7})));

    //extract
    auto eIt = cUSet2.begin();
    eIt++;
    auto node = cUSet2.extract(eIt);
    //Not sure which element is deleted in un sorted set. So just check the size of set
    EXPECT_EQ(cUSet2.size(), 4);

    auto node2 = cUSet2.extract(5);
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet,std::set<int>({-1,1,3})));
    EXPECT_EQ(node2.value().getVal(),5);

    node2.value() = TestConstructor(9);
    cUSet2.insert(std::move(node2));
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUSet,std::set<int>({-1,1,3,9})));
}

TEST(UnorderedSet, LookUp)
{
    std::unordered_set<int> uSet{1,0,2,1,2,3,5,7,12,45};
    EXPECT_EQ(uSet.count(9),0);
    EXPECT_EQ(uSet.count(1),1);

    auto it = uSet.find(2);
    EXPECT_EQ(*it,2);
    it = uSet.find(10);
    EXPECT_EQ(it,uSet.end());

    EXPECT_TRUE(uSet.contains(3));
    EXPECT_FALSE(uSet.contains(10));

    auto [itr1, itr2] = uSet.equal_range(5);
    EXPECT_EQ(*itr1,5);
    EXPECT_EQ(*itr2,3);
}

TEST(UnorderedSet, BucketInterface)
{
    //Just print some stats because to check this values with tests
    //are quit difficult since they are bit random.
    std::unordered_set<int> uSet{1,2,0,-1,4,3,7,100,400,4000,50000};

    std::cout<<"Bucket count = "<<uSet.bucket_count()<<"\n";

    std::cout<<"Bucket Size = "<<uSet.bucket_size(4)<<"\n";

    std::cout<<"Bucket for key 3 = "<<uSet.bucket(3)<<"\n";

    auto it = uSet.begin(1);
    std::cout<<"Element in bucket 1 beginning "<<*it<<"\n";
}


TEST(UnorderedSet,Observers)
{
    std::unordered_set<int> uSet{1,2,0,3,5,4};
    auto key_eq = uSet.key_eq();
    EXPECT_TRUE(key_eq(1,1));
    EXPECT_FALSE(key_eq(2,1));

    auto hash_func = uSet.hash_function();
    EXPECT_EQ(hash_func(1),1);
    EXPECT_EQ(hash_func(100),100);

}

TEST(UnorderedSet, NonMemberFunctions)
{
    std::unordered_set<int> uSet1{1,2,3,4};
    std::unordered_set<int> uSet2{1,2,3,4,5};
    std::unordered_set<int> uSet3{1,2,3,4};

    EXPECT_TRUE(uSet1==uSet3);
    EXPECT_FALSE(uSet1==uSet2);

    std::swap(uSet1, uSet2);

    for(auto i:uSet1)
        std::cout<<i<<" ";

    for(auto j:uSet2)
        std::cout<<j<<" ";
    
    EXPECT_TRUE(compareContainers(uSet1,std::set<int>({1,2,3,4,5})));
    EXPECT_TRUE(compareContainers(uSet2,std::set<int>({1,2,3,4})));

    //The set3 would have values of {1,2,3,4}
    //Now delete all even numbers by using predicate
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(uSet3, deleteEvenNumbers);
    EXPECT_TRUE(compareContainers(uSet3,std::set<int>({1,3})));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}