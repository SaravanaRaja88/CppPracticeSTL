#include <iostream>
#include <gtest/gtest.h>
#include <list>

TEST(List, MemberFunctions)
{
    //consturctors
    std::list<int> lst;
    EXPECT_TRUE(lst.empty());

    std::list<int> lst1(3,4);
    EXPECT_EQ(lst1.size(),3);
    EXPECT_TRUE(std::equal(lst1.begin(), lst1.end(), std::begin({4,4,4,4})));

    std::list<int> lst2(5);
    EXPECT_EQ(lst2.size(),5);
    EXPECT_TRUE(std::equal(lst2.begin(), lst2.end(), std::begin({0,0,0,0,0})));

    std::list<int> lst3(lst1.begin(), lst1.end());
    EXPECT_EQ(lst3.size(),3);
    EXPECT_TRUE(std::equal(lst3.begin(), lst3.end(), std::begin({4,4,4,4})));

    std::list<int> lst4(lst2);
    EXPECT_EQ(lst4.size(),5);
    EXPECT_TRUE(std::equal(lst4.begin(), lst4.end(), std::begin({0,0,0,0,0})));

    std::list<int> lst5(std::list<int>(3,1));
    EXPECT_EQ(lst5.size(),3);
    EXPECT_TRUE(std::equal(lst5.begin(), lst5.end(), std::begin({1,1,1})));

    std::list<int> lst6{1,2,3,4,5};
    EXPECT_EQ(lst6.size(),5);
    EXPECT_TRUE(std::equal(lst6.begin(), lst6.end(), std::begin({1,2,3,4,5})));

    //operator = 
    std::list<int> lst7;
    lst7 = lst6;
    EXPECT_EQ(lst7.size(),5);
    EXPECT_TRUE(std::equal(lst7.begin(), lst7.end(), std::begin({1,2,3,4,5})));

    std::list<int> lst8;
    lst8 = std::move(lst7);
    EXPECT_EQ(lst8.size(),5);
    EXPECT_TRUE(std::equal(lst8.begin(), lst8.end(), std::begin({1,2,3,4,5})));

    std::list<int> lst9 = {11,22,33,44,55}; 
    EXPECT_EQ(lst9.size(),5);
    EXPECT_TRUE(std::equal(lst9.begin(), lst9.end(), std::begin({11,22,33,44,55})));

    //assign
    std::list<int> assignlst;
    assignlst.assign(5,4);
    EXPECT_EQ(assignlst.size(),5);
    EXPECT_TRUE(std::equal(assignlst.begin(), assignlst.end(), std::begin({4,4,4,4,4})));

    auto arr = std::to_array({5,5,5});
    assignlst.assign(arr.begin(),arr.end());
    EXPECT_EQ(assignlst.size(),3);
    EXPECT_TRUE(std::equal(assignlst.begin(), assignlst.end(), std::begin({5,5,5})));

    assignlst.assign({11,22,33,44,55});
    EXPECT_EQ(assignlst.size(),5);
    EXPECT_TRUE(std::equal(assignlst.begin(), assignlst.end(), std::begin({11,22,33,44,55})));
}

TEST(List, ElementAccess)
{
    std::list<std::string> lst{"Berlin", "Paris", "Brussels"};
    EXPECT_STREQ(lst.front().c_str(), "Berlin");

    EXPECT_STREQ(lst.back().c_str(), "Brussels");

}

TEST(Test, Iterators)
{
    //empty list both the begin and end iterators pointing to the same location.
    std::list<int> iList;
    EXPECT_EQ(*iList.begin(), *iList.end());

    std::list<std::string> lst{"Germany", "France", "Spain", "Italy"};
    EXPECT_STREQ(lst.begin()->c_str(),"Germany");

    //Just fetch and compare same strings by two different pointers for same container
    for(auto it1 = lst.begin(), it2 = lst.begin(); it1 != lst.end(), it2 != lst.end(); it1++, it2++ )
        EXPECT_STREQ(it1->c_str(),it2->c_str());
    
    auto it = lst.end(); --it;
    EXPECT_STREQ(it->c_str(), "Italy");

    EXPECT_STREQ(lst.rbegin()->c_str(), "Italy");
    EXPECT_STREQ((++lst.rbegin())->c_str(), "Spain");

    EXPECT_STREQ((--lst.rend())->c_str(), "Germany");
    auto it2 = lst.rend(); --it;
    EXPECT_STREQ((--it)->c_str(), "France");
}

TEST(List, Capacity)
{
    std::list<int> lst;
    EXPECT_TRUE(lst.empty());

    lst.assign({1,2,3,4,5});
    EXPECT_EQ(lst.size(), 5);
}

TEST(List, Modifiers)
{   
    //clear
    std::list<std::string> lst{"1", "2", "3", "4"};
    EXPECT_EQ(lst.size(), 4);
    lst.clear();
    EXPECT_EQ(lst.begin(), lst.end());
    EXPECT_EQ(lst.size(), 0);

    //insert
    lst.assign({"Have", "Fun", "With", "List"});
    lst.insert(lst.begin(),"Testing");
    EXPECT_STREQ(lst.begin()->c_str(),"Testing");

    auto it = lst.begin();it++; it++;
    lst.insert(it,2,"->");
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"Testing","Have","->","->","Fun","With","List"})));

    std::array<std::string,3> arr{"Hope","Its", "Working"};
    lst.insert(lst.begin(),arr.begin(), arr.end());
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"Hope","Its", "Working","Testing","Have","->","->", "Fun", "With", "List"})));

    lst.clear();
    lst.insert(lst.begin(),{"Good", "It", "Works", "Fine"});
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"Good", "It", "Works", "Fine"})));

    //erase
    lst.erase(lst.begin());
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"It", "Works", "Fine"})));

    lst.insert(lst.end(),{"Bit", "Extra"});
    auto pos1 = std::next(lst.begin(),2);
    auto pos2 = std::next(lst.end(),-1);
    lst.erase(pos1,pos2);
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"It", "Works", "Extra"})));

    //emplace
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

    std::list<TestConstructor> list;
    list.emplace(list.begin(),1);
    list.emplace(list.begin(),2);
    list.emplace(list.begin(),3);
    auto compare = [](TestConstructor &obj, int val){ return obj.getValue() == val;};
    EXPECT_TRUE(std::ranges::equal(list, std::initializer_list<int>({3,2,1}),compare));

    list.emplace(list.end(),4);
    list.emplace(list.end(),5);
    EXPECT_TRUE(std::ranges::equal(list, std::initializer_list<int>({3,2,1,4,5}),compare));

    list.clear();
    list.emplace_back(1);
    list.emplace_back(2);
    list.emplace_back(3);
    EXPECT_TRUE(std::ranges::equal(list, std::initializer_list<int>({1,2,3}),compare));

    list.emplace_front(0);
    list.emplace_front(-1);
    list.emplace_front(-2);
    EXPECT_TRUE(std::ranges::equal(list, std::initializer_list<int>({-2,-1,0,1,2,3}),compare));

    auto it1 = list.begin();
    std::advance(it1,3);
    list.emplace(it1,555);
    EXPECT_TRUE(std::ranges::equal(list, std::initializer_list<int>({-2,-1,0,555,1,2,3}),compare));

    //push and pop we use the lst above with intgers.
    lst.clear();
    lst.push_back("Hello");
    lst.push_back("World");
    lst.push_back("List");
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"Hello", "World","List"})));

    lst.push_front("Good");
    lst.push_front("Hi");
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"Hi","Good","Hello", "World","List"})));

    lst.pop_back();
    lst.pop_back();
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"Hi","Good","Hello"})));

    lst.pop_front();
    lst.pop_front();
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"Hello"})));

    //resize
    lst.insert(lst.end(),{"Welcome", "Again","For","Resize","Testing"});
    EXPECT_EQ(lst.size(),6);

    lst.resize(3);
    EXPECT_EQ(lst.size(),3);
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"Hello","Welcome","Again"})));

    //swap
    std::list<std::string> lst2{"We", "Are", "Swapped"};
    lst2.swap(lst);
    EXPECT_TRUE(std::ranges::equal(lst, std::initializer_list<std::string>({"We", "Are", "Swapped"})));
    EXPECT_TRUE(std::ranges::equal(lst2, std::initializer_list<std::string>({"Hello","Welcome","Again"})));
}

TEST(List, Operations)
{
    //merge
    std::list<int> lst1 = {1,4,3};
    std::list<int> lst2 = {2,6,5};
    
    lst1.sort();
    lst2.sort();

    lst1.merge(lst2);
    EXPECT_TRUE(std::ranges::equal(lst1, std::initializer_list<int>{1,2,3,4,5,6}));

    //splice
    std::list<int> lst3{11,22,33};
    lst3.splice(std::next(lst3.begin(),1),lst1);
    EXPECT_TRUE(std::ranges::equal(lst3, std::initializer_list<int>{11,1,2,3,4,5,6,22,33}));

    std::list<int> lst4{111,222,333};
    lst3.splice(std::next(lst3.begin()),lst4,std::next(lst4.begin(),1));
    EXPECT_TRUE(std::ranges::equal(lst3, std::initializer_list<int>{11,222,1,2,3,4,5,6,22,33}));

    std::list<int> lst5{555};
    lst5.splice(lst5.begin(),lst3,std::next(lst3.begin(),2),std::next(lst3.begin(),9));
    EXPECT_TRUE(std::ranges::equal(lst5, std::initializer_list<int>{1,2,3,4,5,6,22,555}));

    lst1.clear();
    lst1.assign({777,888,999});

    auto it = lst1.begin();
    std::advance(it,2);

    auto it2 = lst5.begin();
    auto it3 = lst5.end();
    std::advance(it2,2);
    std::advance(it3,-1);
    lst1.splice(it,lst5,it2,it3);
    EXPECT_TRUE(std::ranges::equal(lst1, std::initializer_list<int>{777,888,3,4,5,6,22,999}));

    //remove
    std::list<int> rList{1,2,3,2,4,5,4,6,7};
    rList.remove(2);
    EXPECT_TRUE(std::ranges::equal(rList, std::initializer_list<int>{1,3,4,5,4,6,7}));
    rList.remove(4);
    EXPECT_TRUE(std::ranges::equal(rList, std::initializer_list<int>{1,3,5,6,7}));
    
    rList.clear();
    rList.assign({1,2,3,4,5,6,7,8,9,10});
    auto removeEven = [](int num){ return num%2 == 0;};
    rList.remove_if(removeEven);
    EXPECT_TRUE(std::ranges::equal(rList, std::initializer_list<int>{1,3,5,7,9}));
    
    //reverse
    rList.reverse();
    EXPECT_TRUE(std::ranges::equal(rList, std::initializer_list<int>{9,7,5,3,1}));

    //unique
    std::list<int> uList{1,22,22,5,6,77,88};
    uList.unique();
    EXPECT_TRUE(std::ranges::equal(uList, std::initializer_list<int>{1,22,5,6,77,88}));

    uList.clear();
    uList.assign({1,2,2,3,4,4,5,6,6,7});
    uList.unique([](int a, int b){return ((a%2 == 0) && (b%2==0));});
    EXPECT_TRUE(std::ranges::equal(uList, std::initializer_list<int>{1,2,3,4,5,6,7}));

    //sort
    uList.clear();
    uList.assign({1,4,6,8,9,0,10});
    uList.sort();
    EXPECT_TRUE(std::ranges::equal(uList, std::initializer_list<int>{0,1,4,6,8,9,10}));
}


TEST(F_List, NonMemberFunctions)
{
    std::list<int> lst1{1,2,3,4};
    std::list<int> lst2{1,2,3,4,5};
    std::list<int> lst3{1,2,3,4};

    EXPECT_TRUE(lst1==lst3);
    EXPECT_FALSE(lst1==lst2);

    EXPECT_TRUE(lst1 < lst2);
    EXPECT_TRUE(lst2 > lst1);

    EXPECT_TRUE(lst1 <= lst2);
    EXPECT_TRUE(lst2 >= lst1);

    std::swap(lst1, lst2);
    EXPECT_TRUE(std::equal(lst1.begin(), lst1.end(), std::begin({1,2,3,4,5})));
    EXPECT_TRUE(std::equal(lst2.begin(), lst2.end(), std::begin({1,2,3,4})));

    lst3.push_back(4);
    lst3.push_back(2);
    std::erase(lst3,4);
    EXPECT_TRUE(std::equal(lst3.begin(), lst3.end(), std::begin({1,2,3,2})));

    lst3.push_back(6);
    lst3.push_back(8);;
    //The list would have values of {1,2,3,2,6,8}
    // Now delete all even numbers by using predicate
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(lst3, deleteEvenNumbers);
    EXPECT_TRUE(std::equal(lst3.begin(), lst3.end(), std::begin({1,3})));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}