#include <iostream>
#include <gtest/gtest.h>
#include <map>

TEST(MultiMap, MemberFunctions)
{
    std::multimap<int, std::string> mMap;
    EXPECT_TRUE(mMap.empty());

    auto cmp = [](const int a, const int b) { return a > b; };
    std::multimap<int,std::string,decltype(cmp)> mMap1;
    EXPECT_TRUE(mMap1.empty());

    std::array<std::pair<int,std::string>, 5> initialValues{{{49,"TestMultipleKey"},{49,"Germany"},{91,"India"},{36,"Hungary"},{33,"France"}}};
    std::multimap<int, std::string> mMap2(initialValues.begin(), initialValues.end());
    EXPECT_EQ(mMap2.size(),5);

    std::multimap<int, std::string> expected{{49,"TestMultipleKey"},{49,"Germany"},{91,"India"},{36,"Hungary"},{33,"France"}};
    EXPECT_TRUE(std::ranges::equal(mMap2,expected));

    std::multimap<int, std::string> mMap3(mMap2);
    EXPECT_TRUE(std::ranges::equal(mMap3,expected));
    std::multimap<int, std::string> mMap4(std::move(mMap3));
    EXPECT_TRUE(std::ranges::equal(mMap4,expected));

    std::multimap<int, std::string> mMap5{{22,"TestMultiKey"},{12,"John"},{22,"Sven"},{33,"White"}};
    EXPECT_TRUE(std::ranges::equal(mMap5,(std::multimap<int,std::string>{{12,"John"},{22,"TestMultiKey"},{22,"Sven"},{33,"White"}})));

    //use custom comparator to sort keys in descending order
    std::multimap<int,std::string,decltype(cmp)> mMap6{{1,"are"},{-2,"you"},{20,"Hi"},{11,"how"},{-2,"doing"}};
    EXPECT_TRUE(std::ranges::equal(mMap6,(std::multimap<int,std::string,decltype(cmp)>{{20,"Hi"},{11,"how"},{1,"are"},{-2,"you"},{-2,"doing"}})));

    //= operator
    mMap = mMap2;
    EXPECT_TRUE(std::ranges::equal(mMap,expected));

    mMap1 = std::move(mMap6);
    EXPECT_TRUE(std::ranges::equal(mMap1,(std::multimap<int,std::string,decltype(cmp)>{{20,"Hi"},{11,"how"},{1,"are"},{-2,"you"},{-2,"doing"}})));

    const auto initList = { std::pair<const int, int>{4,4}, {5,5}, {6,6}, {7,7} };
    std::multimap<const int, int> mMap7(initList);
    EXPECT_TRUE(std::ranges::equal(mMap7,(std::multimap<const int, int>{{4,4}, {5,5}, {6,6}, {7,7}})));
}

TEST(MultiMap, Iterators)
{
    std::multimap<int, char> mMap{{1,'a'},{4,'b'},{2,'c'},{3,'d'},{2,'e'}};

    //The map after sorting would look like {1,'a'},{2,'c'}{2,'e'},{3,'d'},{4,'b'}
    EXPECT_EQ(mMap.begin()->second,'a');
    EXPECT_EQ((--mMap.end())->second,'b');

    auto it = mMap.begin();
    std::advance(it, 2);
    EXPECT_EQ(it->first,2);
    EXPECT_EQ(it->second,'e');
    it++;
    EXPECT_EQ(it->first,3);
    EXPECT_EQ(it->second,'d');

    EXPECT_EQ(mMap.rbegin()->first,4);
    EXPECT_EQ(mMap.rbegin()->second,'b');

    EXPECT_EQ((--mMap.rend())->first,1);
    EXPECT_EQ((--mMap.rend())->second,'a');

    auto it2 = mMap.rend();
    std::advance(it2,-3);
    EXPECT_EQ(it2->first,2);
    EXPECT_EQ(it2->second,'e');
}

TEST(MultiMap, Capacity)
{
    std::multimap<std::string, std::vector<int>> mMap;
    EXPECT_TRUE(mMap.empty());

    mMap = {{"First",{1,2,3}},{"Second",{2,3,4}},{"First",{1,5,1}}};
    EXPECT_EQ(mMap.size(),3);
}

TEST(MultiMap, Modifiers)
{
    std::multimap<int, std::vector<int>> mMap{{1,{1,2,3}},{3,{2,3,4}}};
    EXPECT_EQ(mMap.size(),2);
    EXPECT_FALSE(mMap.empty());

    mMap.clear();
    EXPECT_EQ(mMap.size(),0);
    EXPECT_TRUE(mMap.empty());

    //rvalue
    mMap.insert({2,{1,4,5}});
    mMap.insert({1,{1,2,3}});
    mMap.insert({3,{2,3,4}});
    mMap.insert({3,{2,3,4}});

    std::multimap<int, std::vector<int>> expectedMap{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{3,{2,3,4}}};
    EXPECT_TRUE(std::ranges::equal(mMap,expectedMap));
    
    //lvalue
    std::pair<int,std::vector<int>> val{5,{2,4,5}};
    mMap.insert(val);
    std::multimap<int, std::vector<int>> expectedMap1{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{3,{2,3,4}},{5,{2,4,5}}};
    EXPECT_TRUE(std::ranges::equal(mMap,expectedMap1));

    //rvalue with emplace
    mMap.insert(std::pair<int,std::vector<int>>{7,{2,4,5}});
    std::multimap<int, std::vector<int>> expectedMap2{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{3,{2,3,4}},{5,{2,4,5}},{7,{2,4,5}}};
    EXPECT_TRUE(std::ranges::equal(mMap,expectedMap2));

    //Same lvalue, rvalue and emplace with position insertion
    std::multimap<int,char> mMap1{{1,'a'},{4,'b'}};
    mMap1.insert(++mMap1.begin(),{2,'c'});

    auto it = mMap1.begin();
    it++;
    mMap1.insert(it, std::pair<int,char>{7,'e'});

    it++;
    std::pair<int,char> val2{3,'f'};
    mMap1.insert(it,val2);
    std::multimap<int,char> expectedMap3{{1,'a'},{2,'c'},{3,'f'},{4,'b'},{7,'e'}};
    EXPECT_TRUE(std::ranges::equal(mMap1,expectedMap3));

    //range insertion
    std::multimap<int,char> mMap2;
    mMap2.insert(mMap1.find(2),mMap1.find(4));
    std::multimap<int,char> expectedMap4{{2,'c'},{3,'f'}};
    EXPECT_TRUE(std::ranges::equal(mMap2,expectedMap4));

    mMap2.insert({{1,'a'},{1,'b'},{5,'o'}});
    std::multimap<int,char> expectedMap5{{1,'a'},{1,'b'},{2,'c'},{3,'f'},{5,'o'}};
    EXPECT_TRUE(std::ranges::equal(mMap2,expectedMap5));

    //node_type insertion
    auto it1 = mMap1.begin();
    std::advance(it1,3);
    auto node = mMap1.extract(it1);
    mMap2.insert(std::move(node));
    mMap2.insert(mMap1.extract(7));
    std::multimap<int,char> expectedMap6{{1,'a'},{1,'b'},{2,'c'},{3,'f'},{4,'b'},{5,'o'},{7,'e'}};
    EXPECT_TRUE(std::ranges::equal(mMap2,expectedMap6));

    //emplace
    class TestConstructor
    {
        public:
            //We can also implement all other constructors like copy, move and assignment operators
            TestConstructor(int val): m_Value(val){};
            ~TestConstructor() = default; 
            
            int getValue () const{return m_Value;}
 
            // bool operator<(const TestConstructor &other) const
            // {
            //   if (m_Value < other.m_Value)
            //     return true;
              
            //   return false;
            // }
        protected:
            int m_Value = 0;
    };

    std::multimap<int, TestConstructor> cMap;
    cMap.emplace(1,11);
    cMap.emplace(2,22);
    cMap.emplace(3,33);

    auto compare = [](const std::pair<int, TestConstructor>  &obj1, const std::pair<int, TestConstructor>  &obj2){ return (obj1.first == obj2.first) && (obj1.second.getValue() == obj2.second.getValue());};
    std::multimap<int,TestConstructor> expectedMap8{{1,11},{2,22},{3,33}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap8,compare));

    //Just inserts to the nearest possible iterator position
    cMap.emplace_hint(cMap.begin(),4,44);
    cMap.emplace_hint(cMap.begin(),-1,-11);
    cMap.emplace_hint(cMap.begin(),-1,-11);
    std::multimap<int,TestConstructor> expectedMap9{{-1,-11},{-1,-11},{1,11},{2,22},{3,33},{4,44}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap9,compare));

    cMap.erase(cMap.begin());
    std::multimap<int,TestConstructor> expectedMap11{{-1,-11},{1,11},{2,22},{3,33},{4,44}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap11,compare));

    auto cIt = cMap.begin();
    std::advance(cIt,3);
    cMap.erase(cMap.begin(),cIt);
    std::multimap<int,TestConstructor> expectedMap12{{3,33},{4,44}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap12,compare));

    //swap
    std::multimap<int, TestConstructor> cMap1{{-1,11},{-2,22}};
    cMap1.swap(cMap);
    std::multimap<int,TestConstructor> expectedMap13{{-2,22},{-1,11}};
    EXPECT_TRUE(std::ranges::equal(cMap1,expectedMap12,compare));
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap13,compare));

    //extract
    auto node2 = cMap1.extract(4);
    node2.key() = 5;
    node2.mapped() = 33;
    cMap1.insert(std::move(node2));
    std::multimap<int,TestConstructor> expectedMap14{{3,33},{5,33}};
    EXPECT_TRUE(std::ranges::equal(cMap1,expectedMap14,compare));
  
    //merge
    cMap1.merge(cMap);
    std::multimap<int,TestConstructor> expectedMap15{{-2,22},{-1,11},{3,33},{5,33}};
    EXPECT_TRUE(std::ranges::equal(cMap1,expectedMap15,compare));
}

TEST(MultiMap, LookUp)
{
    std::map<int,char> mMap;
    mMap.insert({{1,'a'},{3,'b'},{3,'o'},{2,'c'},{2,'f'}});
    EXPECT_EQ(mMap.count(3),1);

    auto foundElement = mMap.find(2);
    EXPECT_EQ(foundElement->first,2);
    EXPECT_EQ(foundElement->second,'c');

    EXPECT_TRUE(mMap.contains(1));
    EXPECT_FALSE(mMap.contains(7));

    mMap.insert({4,'d'});
    auto [it1,it2] = mMap.equal_range(2);
    EXPECT_EQ(it1->first,2);
    EXPECT_EQ(it1->second,'c');
    EXPECT_EQ(it2->first,3);
    EXPECT_EQ(it2->second,'b');

    auto lIt = mMap.lower_bound(1);
    EXPECT_EQ(lIt->first,1);
    EXPECT_EQ(lIt->second,'a');
    
    mMap.insert({8,'f'});
    auto uIt = mMap.upper_bound(4);
    EXPECT_EQ(uIt->first,8);
    EXPECT_EQ(uIt->second,'f');
}

TEST(MultiMap, Observers)
{
    std::map<int,char> mMap{{1,'a'},{2,'b'},{4,'c'},{5,'e'}};

    auto keyCompFunc = mMap.key_comp();
    
    EXPECT_TRUE(keyCompFunc(1,4));
    EXPECT_FALSE(keyCompFunc(4,1));

    auto valueCompareFunc = mMap.value_comp();
    const std::pair<int,char> p1 = {2,'b'};
    const std::pair<const int,char> p2 = {-1,'e'};

    auto it = mMap.begin();
    EXPECT_TRUE(valueCompareFunc(*it,p1));
    EXPECT_FALSE(valueCompareFunc(*it,p2));
}

TEST(MultiMap, NonMemberFunctions)
{
    std::multimap<int,char> mMap1{{1,'a'},{2,'b'},{2,'f'},{3,'c'},{4,'d'}};
    std::multimap<int,char> mMap2{{1,'a'},{2,'b'},{2,'f'},{3,'c'},{4,'d'},{5,'f'}};
    std::multimap<int,char> mMap3{{1,'a'},{2,'b'},{2,'f'},{3,'c'},{4,'d'}};

    EXPECT_TRUE(mMap1==mMap3);
    EXPECT_FALSE(mMap1==mMap2);

    EXPECT_TRUE(mMap1 < mMap2);
    EXPECT_TRUE(mMap2 > mMap1);

    EXPECT_TRUE(mMap1 <= mMap2);
    EXPECT_TRUE(mMap2 >= mMap1);

    std::swap(mMap1, mMap2);
    std::multimap<int,char> expected1{{1,'a'},{2,'b'},{2,'f'},{3,'c'},{4,'d'},{5,'f'}};
    std::multimap<int,char> expected2{{1,'a'},{2,'b'},{2,'f'},{3,'c'},{4,'d'}};

    EXPECT_TRUE(std::equal(mMap1.begin(), mMap1.end(), expected1.begin()));
    EXPECT_TRUE(std::equal(mMap2.begin(), mMap2.end(), expected2.begin()));

    //The map3 would have values of {1,'a'},{2,'b'},{3,'c'},{4,'d'}
    //Now delete all keys with even numbers by using predicate
    std::multimap<int,char> expected3{{1,'a'},{3,'c'}};
    auto deleteEvenKeysNode = [](const std::pair<int,char>& key){return (key.first % 2 ) == 0;};
    std::erase_if(mMap3, deleteEvenKeysNode);
    EXPECT_TRUE(std::equal(mMap3.begin(), mMap3.end(), expected3.begin()));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}