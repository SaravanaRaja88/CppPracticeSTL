#include <iostream>
#include <gtest/gtest.h>
#include <unordered_map>

bool compareContainers(auto &container1, auto &&container2)
{
    for(auto &key:container1)
        if(!container2.contains(key.first))
            return false;
    return true;
}

bool compareUserDefinedValuesContainers(auto &container1, auto &&container2)
{
    for(auto &value:container1)
        if(!container2.contains(value.first.getValue()))
            return false;
    return true;
}
TEST(UnorderedMap, MemberFunctions)
{
    std::unordered_map<int, std::string> uMap;
    EXPECT_TRUE(uMap.empty());

    //User defined type
    class Key
    {
    public:
        std::string val;
    public:
        Key(std::string v):val(v){};
        ~Key(){};

        std::string getVal() const{return val;}
    };

    //use the custom comparator
    auto keyCmp = [](const Key &a, const Key &b) { return a.getVal() == b.getVal(); };

    //use the custom hash
    struct Hasher
    {
        size_t operator()(const Key &x) const
        {
            return std::hash<std::string>()(x.getVal());
        }
    };

    //The value type is also custom type. Just to check that values doesn't need hash
    class Value
    {
        public:
            int val;
        public:
            Value(int v):val(v){};
            ~Value(){};
        
        int getVal() const{return val;}
    };

    std::unordered_map<Key,Value,Hasher,decltype(keyCmp)> uMap1{{Key("Str1"),Value(1)}};
    EXPECT_EQ(uMap1.size(),1);

    std::array<std::pair<int,std::string>, 4> initialValues{{{49,"Germany"},{91,"India"},{36,"Hungary"},{33,"France"}}};
    std::unordered_map<int, std::string> uMap2(initialValues.begin(), initialValues.end());
    EXPECT_EQ(uMap2.size(),4);

    std::unordered_map<int, std::string> uMap3(uMap2);
    EXPECT_TRUE(compareContainers(uMap3,uMap2));
    std::unordered_map<int, std::string> uMap4(std::move(uMap3));
    EXPECT_TRUE(compareContainers(uMap4,uMap2));

    std::unordered_map<int, std::string> uMap5{{12,"John"},{22,"Sven"},{33,"White"}};
    EXPECT_TRUE(compareContainers(uMap5,(std::map<int,std::string>{{12,"John"},{22,"Sven"},{33,"White"}})));

    //= operator
    uMap = uMap2;
    EXPECT_TRUE(compareContainers(uMap,uMap2));

    std::unordered_map<int, std::string> uMap6{{20,"Hi"},{11,"how"},{1,"are"},{-2,"you"}};
    uMap2 = std::move(uMap6);
    EXPECT_TRUE(compareContainers(uMap2,(std::map<int,std::string>{{20,"Hi"},{11,"how"},{1,"are"},{-2,"you"}})));

    const auto initList = { std::pair<const int, int>{4,4}, {5,5}, {6,6}, {7,7} };
    std::map<const int, int> uMap7(initList);
    EXPECT_TRUE(compareContainers(uMap7,(std::map<const int, int>{{4,4}, {5,5}, {6,6}, {7,7}})));
}

TEST(UnorderedMap, ElementAccess)
{
    std::unordered_map<int, char> uMap{{1,'a'},{4,'b'},{2,'c'},{3,'d'}};

    EXPECT_EQ(uMap.at(4),'b');
    EXPECT_EQ(uMap.at(2),'c');

    EXPECT_EQ(uMap[1],'a');
    EXPECT_EQ(uMap[3],'d');

    uMap[1] = 'e';
    uMap[3] = 'f';

    EXPECT_EQ(uMap[1],'e');
    EXPECT_EQ(uMap[3],'f');
}

TEST(UnorderedMap, Iterators)
{
    std::unordered_map<int, char> uMap{{1,'a'},{4,'b'},{2,'c'},{3,'d'}};

    //Since there is no guarantee which element will be in the first
    //i just assume the last element is inserted first.
    //This is just for testing purpose. 
    EXPECT_EQ(uMap.begin()->second,'d');
    auto it = uMap.begin();
    std::advance(it, 2);
    EXPECT_EQ(it->first,4);     
    EXPECT_EQ(it->second,'b');

    //since no reverse iterators supported we cannot do --uMap.end()
    //uMap.end() here gives the iterator pass all the elements. 
    
}

TEST(UnorderedMap, Capacity)
{
    std::unordered_map<std::string, std::vector<int>> uMap;
    EXPECT_TRUE(uMap.empty());

    uMap = {{"First",{1,2,3}},{"Second",{2,3,4}}};
    EXPECT_EQ(uMap.size(),2);
}

TEST(UnorderedMap, Modifiers)
{
    std::unordered_map<int, std::vector<int>> uMap{{1,{1,2,3}},{3,{2,3,4}}};
    EXPECT_EQ(uMap.size(),2);
    EXPECT_FALSE(uMap.empty());

    uMap.clear();
    EXPECT_EQ(uMap.size(),0);
    EXPECT_TRUE(uMap.empty());

    //rvalue
    uMap.insert({2,{1,4,5}});
    uMap.insert({1,{1,2,3}});
    uMap.insert({3,{2,3,4}});
    std::map<int, std::vector<int>> expectedMap{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}}};
    EXPECT_TRUE(compareContainers(uMap,expectedMap));
    
    //lvalue
    std::pair<int,std::vector<int>> val{5,{2,4,5}};
    uMap.insert(val);
    std::map<int, std::vector<int>> expectedMap1{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{5,{2,4,5}}};
    EXPECT_TRUE(compareContainers(uMap,expectedMap1));

    //rvalue with emplace
    uMap.insert(std::pair<int,std::vector<int>>{7,{2,4,5}});
    std::map<int, std::vector<int>> expectedMap2{{1,{1,2,3}},{2,{1,4,5}},{3,{2,3,4}},{5,{2,4,5}},{7,{2,4,5}}};
    EXPECT_TRUE(compareContainers(uMap,expectedMap2));

    //Same lvalue, rvalue and emplace with position insertion
    std::unordered_map<int,char> uMap1{{1,'a'},{4,'b'}};
    uMap1.insert(++uMap1.begin(),{2,'c'});

    auto it = uMap1.begin();
    it++;
    uMap1.insert(it, std::pair<int,char>{7,'e'});

    it++;
    std::pair<int,char> val2{3,'f'};
    uMap1.insert(it,val2);
    std::map<int,char> expectedMap3{{1,'a'},{2,'c'},{3,'f'},{4,'b'},{7,'e'}};
    EXPECT_TRUE(compareContainers(uMap1,expectedMap3));

    //range insertion
    std::unordered_map<int,char> uMap2;
    uMap2.insert(uMap1.find(2),uMap1.find(4));
    //Be aware same values we might not get in expected range because of 
    //unsorted map
    std::map<int,char> expectedMap4{{2,'c'},{1,'a'}};
    EXPECT_TRUE(compareContainers(uMap2,expectedMap4));

    uMap2.insert({{1,'a'},{5,'o'}});
    std::map<int,char> expectedMap5{{1,'a'},{2,'c'},{5,'o'}};
    EXPECT_TRUE(compareContainers(uMap2,expectedMap5));

    //node_type insertion here we dont know which key and value is picked 
    //since its unsorted map. This is to demo how to extract node and insert back
    auto it1 = uMap1.begin();
    std::advance(it1,3);
    auto node = uMap1.extract(it1);
    uMap2.insert(std::move(node));
    EXPECT_EQ(uMap2.size(),3);

    //insert_or_assign since the keys are same both are not inserted into map. so the size is same
    uMap2.insert_or_assign(1,'s');
    uMap2.insert_or_assign(++uMap2.begin(),2,'h');
    EXPECT_EQ(uMap2.size(),3);

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

    //use the custom comparator
    auto keyCmp = [](const TestConstructor &a, const TestConstructor &b) { return a.getValue() == b.getValue(); };

    //use the custom hash
    struct Hasher
    {
        size_t operator()(const TestConstructor &x) const
        {
            return std::hash<int>()(x.getValue());
        }
    };

    std::unordered_map<TestConstructor,int,Hasher,decltype(keyCmp)> cUMap;
    cUMap.emplace(1,11);
    cUMap.emplace(2,22);
    cUMap.emplace(3,33);

    EXPECT_TRUE(compareUserDefinedValuesContainers(cUMap,(std::map<TestConstructor,int>{{1,11},{2,22},{3,33}})));

    //Just inserts to the nearest possible iterator position
    cUMap.emplace_hint(cUMap.begin(),4,44);
    cUMap.emplace_hint(cUMap.begin(),-1,-11);
    std::map<int,TestConstructor> expectedMap9{{-1,-11},{1,11},{2,22},{3,33},{4,44}};
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUMap,expectedMap9));

    //try_emplace does nothing if the key is already present.
    cUMap.try_emplace(3,33);
    cUMap.try_emplace(7,77);
    cUMap.try_emplace(cUMap.end(),8,88);
    std::map<int,TestConstructor> expectedMap10{{-1,-11},{1,11},{2,22},{3,33},{4,44},{7,77},{8,88}};
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUMap,expectedMap10));

    //Note the last element inserted will be in beginning mostly but not surely all the time
    //{8,88} gets deleted.
    cUMap.erase(cUMap.begin());
    std::map<int,TestConstructor> expectedMap11{{1,11},{2,22},{3,33},{4,44},{7,77},{-1,11}};
    EXPECT_TRUE(compareUserDefinedValuesContainers(cUMap,expectedMap11));

    auto cIt = cUMap.begin();
    std::advance(cIt,3);
    cUMap.erase(cUMap.begin(),cIt);
    EXPECT_EQ(cUMap.size(),3);

    //swap
    std::unordered_map<TestConstructor,int,Hasher,decltype(keyCmp)> cMap1{{-1,11},{-2,22}};
    cMap1.swap(cUMap);
    EXPECT_EQ(cUMap.size(),2);
    EXPECT_EQ(cMap1.size(),3);

    //extract check if the key is present and then extract just to be safe
    //the extracted node is removed from container
    if(cMap1.contains(2))
    {
        auto node2 = cMap1.extract(2);
        node2.key() = TestConstructor(5);
        node2.mapped() = 33;
        EXPECT_EQ(cMap1.size(),2);
    }

    cMap1.merge(cUMap);
    EXPECT_EQ(cMap1.size(),3);

}

TEST(UnorderedMap, LookUp)
{
    std::unordered_map<int,char> map;
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
}

TEST(UnorderedMap, Observers)
{
    std::unordered_map<int,char> uMap{{1,'a'},{2,'b'},{4,'c'},{5,'e'}};

    auto key_eq = uMap.key_eq();
    EXPECT_TRUE(key_eq(1,1));
    EXPECT_FALSE(key_eq(2,1));

    auto hash_func = uMap.hash_function();
    EXPECT_EQ(hash_func(1),1);
    EXPECT_EQ(hash_func(100),100);
}

TEST(UnorderedMap, NonMemberFunctions)
{
    std::unordered_map<int,char> uMap1{{1,'a'},{2,'b'},{3,'c'},{4,'d'}};
    std::unordered_map<int,char> uMap2{{1,'a'},{2,'b'},{3,'c'},{4,'d'},{5,'f'}};
    std::unordered_map<int,char> uMap3{{1,'a'},{2,'b'},{3,'c'},{4,'d'}};

    EXPECT_TRUE(uMap1==uMap3);
    EXPECT_FALSE(uMap1==uMap2);

    std::swap(uMap1, uMap2);
    std::map<int,char> expected1{{1,'a'},{2,'b'},{3,'c'},{4,'d'},{5,'f'}};
    std::map<int,char> expected2{{1,'a'},{2,'b'},{3,'c'},{4,'d'}};

    EXPECT_TRUE(compareContainers(uMap1, expected1));
    EXPECT_TRUE(compareContainers(uMap2, expected2));

    //The map3 would have values of {1,'a'},{2,'b'},{3,'c'},{4,'d'}
    //Now delete all keys with even numbers by using predicate
    std::map<int,char> expected3{{1,'a'},{3,'c'}};
    auto deleteEvenKeysNode = [](const std::pair<int,char>& key){return (key.first % 2 ) == 0;};
    std::erase_if(uMap3, deleteEvenKeysNode);
    EXPECT_TRUE(compareContainers(uMap3, expected3));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}