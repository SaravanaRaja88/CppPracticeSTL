#include <iostream>
#include <gtest/gtest.h>
#include <map>

TEST(Map, MemberFunctions)
{
    std::map<int, std::string> map;
    EXPECT_TRUE(map.empty());

    auto cmp = [](const int a, const int b) { return a > b; };
    std::map<int,std::string,decltype(cmp)> map1;
    EXPECT_TRUE(map1.empty());

    std::array<std::pair<int,std::string>, 4> initialValues{{{49,"Germany"},{91,"India"},{36,"Hungary"},{33,"France"}}};
    std::map<int, std::string> map2(initialValues.begin(), initialValues.end());
    EXPECT_EQ(map2.size(),4);

    std::map<int, std::string> expected{{49,"Germany"},{91,"India"},{36,"Hungary"},{33,"France"}};
    EXPECT_TRUE(std::ranges::equal(map2,expected));

    std::map<int, std::string> map3(map2);
    EXPECT_TRUE(std::ranges::equal(map3,expected));
    std::map<int, std::string> map4(std::move(map3));
    EXPECT_TRUE(std::ranges::equal(map4,expected));

    std::map<int, std::string> map5{{12,"John"},{22,"Sven"},{33,"White"}};
    EXPECT_TRUE(std::ranges::equal(map5,(std::map<int,std::string>{{12,"John"},{22,"Sven"},{33,"White"}})));

    //use custom comparator to sort keys in descending order
    std::map<int,std::string,decltype(cmp)> map6{{1,"are"},{-2,"you"},{20,"Hi"},{11,"how"}};
    EXPECT_TRUE(std::ranges::equal(map6,(std::map<int,std::string,decltype(cmp)>{{20,"Hi"},{11,"how"},{1,"are"},{-2,"you"}})));

    //= operator
    map = map2;
    EXPECT_TRUE(std::ranges::equal(map,expected));

    map1 = std::move(map6);
    EXPECT_TRUE(std::ranges::equal(map1,(std::map<int,std::string,decltype(cmp)>{{20,"Hi"},{11,"how"},{1,"are"},{-2,"you"}})));

    const auto initList = { std::pair<const int, int>{4,4}, {5,5}, {6,6}, {7,7} };
    std::map<const int, int> map7(initList);
    EXPECT_TRUE(std::ranges::equal(map7,(std::map<const int, int>{{4,4}, {5,5}, {6,6}, {7,7}})));
}

TEST(Map, ElementAccess)
{
    std::map<int, char> map{{1,'a'},{4,'b'},{2,'c'},{3,'d'}};

    EXPECT_EQ(map.at(4),'b');
    EXPECT_EQ(map.at(2),'c');

    EXPECT_EQ(map[1],'a');
    EXPECT_EQ(map[3],'d');

    map[1] = 'e';
    map[3] = 'f';

    EXPECT_EQ(map[1],'e');
    EXPECT_EQ(map[3],'f');
}

TEST(Map, Iterators)
{
    std::map<int, char> map{{1,'a'},{4,'b'},{2,'c'},{3,'d'}};

    //The map after sorting would look like {1,'a'},{2,'c'},{3,'d'},{4,'b'}
    EXPECT_EQ(map.begin()->second,'a');
    EXPECT_EQ((--map.end())->second,'b');

    auto it = map.begin();
    std::advance(it, 2);
    EXPECT_EQ(it->first,3);
    EXPECT_EQ(it->second,'d');
    it++;
    EXPECT_EQ(it->first,4);
    EXPECT_EQ(it->second,'b');

    EXPECT_EQ(map.rbegin()->first,4);
    EXPECT_EQ(map.rbegin()->second,'b');

    EXPECT_EQ((--map.rend())->first,1);
    EXPECT_EQ((--map.rend())->second,'a');

    auto it2 = map.rend();
    std::advance(it2,-3);
    EXPECT_EQ(it2->first,3);
    EXPECT_EQ(it2->second,'d');

}

TEST(Map, Capacity)
{
    std::map<std::string, std::vector<int>> map;
    EXPECT_TRUE(map.empty());

    map = {{"First",{1,2,3}},{"Second",{2,3,4}}};
    EXPECT_EQ(map.size(),2);
}

TEST(map, Modifiers)
{
    std::map<int, std::vector<int>> map{{1,{1,2,3}},{3,{2,3,4}}};
    EXPECT_EQ(map.size(),2);
    EXPECT_FALSE(map.empty());

    map.clear();
    EXPECT_EQ(map.size(),0);
    EXPECT_TRUE(map.empty());

    //rvalue
    map.insert({2,{1,4,5}});
    map.insert({1,{1,2,3}});
    map.insert({3,{2,3,4}});
    std::map<int, std::vector<int>> expectedMap{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}}};
    EXPECT_TRUE(std::ranges::equal(map,expectedMap));
    
    //lvalue
    std::pair<int,std::vector<int>> val{5,{2,4,5}};
    map.insert(val);
    std::map<int, std::vector<int>> expectedMap1{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{5,{2,4,5}}};
    EXPECT_TRUE(std::ranges::equal(map,expectedMap1));

    //rvalue with emplace
    map.insert(std::pair<int,std::vector<int>>{7,{2,4,5}});
    std::map<int, std::vector<int>> expectedMap2{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{5,{2,4,5}},{7,{2,4,5}}};
    EXPECT_TRUE(std::ranges::equal(map,expectedMap2));

    //Same lvalue, rvalue and emplace with position insertion
    std::map<int,char> map1{{1,'a'},{4,'b'}};
    map1.insert(++map1.begin(),{2,'c'});

    auto it = map1.begin();
    it++;
    map1.insert(it, std::pair<int,char>{7,'e'});

    it++;
    std::pair<int,char> val2{3,'f'};
    map1.insert(it,val2);
    std::map<int,char> expectedMap3{{1,'a'},{2,'c'},{3,'f'},{4,'b'},{7,'e'}};
    EXPECT_TRUE(std::ranges::equal(map1,expectedMap3));

    //range insertion
    std::map<int,char> map2;
    map2.insert(map1.find(2),map1.find(4));
    std::map<int,char> expectedMap4{{2,'c'},{3,'f'}};
    EXPECT_TRUE(std::ranges::equal(map2,expectedMap4));

    map2.insert({{1,'a'},{5,'o'}});
    std::map<int,char> expectedMap5{{1,'a'},{2,'c'},{3,'f'},{5,'o'}};
    EXPECT_TRUE(std::ranges::equal(map2,expectedMap5));

    //node_type insertion
    auto it1 = map1.begin();
    std::advance(it1,3);
    auto node = map1.extract(it1);
    map2.insert(std::move(node));
    map2.insert(map1.extract(7));
    std::map<int,char> expectedMap6{{1,'a'},{2,'c'},{3,'f'},{4,'b'},{5,'o'},{7,'e'}};
    EXPECT_TRUE(std::ranges::equal(map2,expectedMap6));

    //insert_or_assign
    map2.insert_or_assign(1,'s');
    map2.insert_or_assign(++map2.begin(),2,'h');
    std::map<int,char> expectedMap7{{1,'s'},{2,'h'},{3,'f'},{4,'b'},{5,'o'},{7,'e'}};
    EXPECT_TRUE(std::ranges::equal(map2,expectedMap7));

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

    std::map<int, TestConstructor> cMap;
    cMap.emplace(1,11);
    cMap.emplace(2,22);
    cMap.emplace(3,33);

    auto compare = [](const std::pair<int, TestConstructor>  &obj1, const std::pair<int, TestConstructor>  &obj2){ return (obj1.first == obj2.first) && (obj1.second.getValue() == obj2.second.getValue());};
    std::map<int,TestConstructor> expectedMap8{{1,11},{2,22},{3,33}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap8,compare));

    //Just inserts to the nearest possible iterator position
    cMap.emplace_hint(cMap.begin(),4,44);
    cMap.emplace_hint(cMap.begin(),-1,-11);
    std::map<int,TestConstructor> expectedMap9{{-1,-11},{1,11},{2,22},{3,33},{4,44}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap9,compare));

    //try_emplace does nothing if the key is already present.
    cMap.try_emplace(3,33);
    cMap.try_emplace(7,77);
    cMap.try_emplace(cMap.end(),8,88);
    std::map<int,TestConstructor> expectedMap10{{-1,-11},{1,11},{2,22},{3,33},{4,44},{7,77},{8,88}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap10,compare));

    cMap.erase(cMap.begin());
    std::map<int,TestConstructor> expectedMap11{{1,11},{2,22},{3,33},{4,44},{7,77},{8,88}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap11,compare));

    auto cIt = cMap.begin();
    std::advance(cIt,3);
    cMap.erase(cMap.begin(),cIt);
    std::map<int,TestConstructor> expectedMap12{{4,44},{7,77},{8,88}};
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap12,compare));

    //swap
    std::map<int, TestConstructor> cMap1{{-1,11},{-2,22}};
    cMap1.swap(cMap);
    std::map<int,TestConstructor> expectedMap13{{-2,22},{-1,11}};
    EXPECT_TRUE(std::ranges::equal(cMap1,expectedMap12,compare));
    EXPECT_TRUE(std::ranges::equal(cMap,expectedMap13,compare));

    //extract
    auto node2 = cMap1.extract(4);
    node2.key() = 5;
    node2.mapped() = 33;
    cMap1.insert(std::move(node2));
    std::map<int,TestConstructor> expectedMap14{{5,33},{7,77},{8,88}};
    EXPECT_TRUE(std::ranges::equal(cMap1,expectedMap14,compare));
  
    //merge
    cMap1.merge(cMap);
    std::map<int,TestConstructor> expectedMap15{{-2,22},{-1,11},{5,33},{7,77},{8,88}};
    EXPECT_TRUE(std::ranges::equal(cMap1,expectedMap15,compare));
}

TEST(Map, LookUp)
{
    std::map<int,char> map;
    map.insert({{1,'a'},{3,'b'},{2,'c'}});
    EXPECT_EQ(map.count(3),1);

    auto foundElement = map.find(2);
    EXPECT_EQ(foundElement->first,2);
    EXPECT_EQ(foundElement->second,'c');

    EXPECT_TRUE(map.contains(1));
    EXPECT_FALSE(map.contains(7));

    map.insert({4,'d'});
    auto [it1,it2] = map.equal_range(2);
    EXPECT_EQ(it1->first,2);
    EXPECT_EQ(it1->second,'c');
    EXPECT_EQ(it2->first,3);
    EXPECT_EQ(it2->second,'b');

    auto lIt = map.lower_bound(1);
    EXPECT_EQ(lIt->first,1);
    EXPECT_EQ(lIt->second,'a');
    
    map.insert({8,'f'});
    auto uIt = map.upper_bound(4);
    EXPECT_EQ(uIt->first,8);
    EXPECT_EQ(uIt->second,'f');
}

TEST(Map, Observers)
{
    std::map<int,char> map{{1,'a'},{2,'b'},{4,'c'},{5,'e'}};

    auto keyCompFunc = map.key_comp();
    
    EXPECT_TRUE(keyCompFunc(1,4));
    EXPECT_FALSE(keyCompFunc(4,1));

    auto valueCompareFunc = map.value_comp();
    const std::pair<int,char> p1 = {2,'b'};
    const std::pair<const int,char> p2 = {-1,'e'};

    auto it = map.begin();
    EXPECT_TRUE(valueCompareFunc(*it,p1));
    EXPECT_FALSE(valueCompareFunc(*it,p2));
}

TEST(Map, NonMemberFunctions)
{
    std::map<int,char> map1{{1,'a'},{2,'b'},{3,'c'},{4,'d'}};
    std::map<int,char> map2{{1,'a'},{2,'b'},{3,'c'},{4,'d'},{5,'f'}};
    std::map<int,char> map3{{1,'a'},{2,'b'},{3,'c'},{4,'d'}};

    EXPECT_TRUE(map1==map3);
    EXPECT_FALSE(map1==map2);

    EXPECT_TRUE(map1 < map2);
    EXPECT_TRUE(map2 > map1);

    EXPECT_TRUE(map1 <= map2);
    EXPECT_TRUE(map2 >= map1);

    std::swap(map1, map2);
    std::map<int,char> expected1{{1,'a'},{2,'b'},{3,'c'},{4,'d'},{5,'f'}};
    std::map<int,char> expected2{{1,'a'},{2,'b'},{3,'c'},{4,'d'}};

    EXPECT_TRUE(std::equal(map1.begin(), map1.end(), expected1.begin()));
    EXPECT_TRUE(std::equal(map2.begin(), map2.end(), expected2.begin()));

    //The map3 would have values of {1,'a'},{2,'b'},{3,'c'},{4,'d'}
    //Now delete all keys with even numbers by using predicate
    std::map<int,char> expected3{{1,'a'},{3,'c'}};
    auto deleteEvenKeysNode = [](const std::pair<int,char>& key){return (key.first % 2 ) == 0;};
    std::erase_if(map3, deleteEvenKeysNode);
    EXPECT_TRUE(std::equal(map3.begin(), map3.end(), expected3.begin()));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}