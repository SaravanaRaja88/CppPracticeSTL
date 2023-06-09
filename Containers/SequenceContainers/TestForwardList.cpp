#include <iostream>
#include <gtest/gtest.h>
#include <forward_list>

TEST(F_List, MemberFunctions)
{
    //consturctors
    std::forward_list<int> lst;
    EXPECT_TRUE(lst.empty());

    std::forward_list<int> lst1(3,4);
    EXPECT_EQ(std::distance(lst1.begin(), lst1.end()),3);
    EXPECT_TRUE(std::equal(lst1.begin(), lst1.end(), std::begin({4,4,4,4})));

    std::forward_list<int> lst2(5);
    EXPECT_EQ(std::distance(lst2.begin(), lst2.end()),5);
    EXPECT_TRUE(std::equal(lst2.begin(), lst2.end(), std::begin({0,0,0,0,0})));

    std::forward_list<int> lst3(lst1.begin(), lst1.end());
    EXPECT_EQ(std::distance(lst3.begin(), lst3.end()),3);
    EXPECT_TRUE(std::equal(lst3.begin(), lst3.end(), std::begin({4,4,4,4})));

    std::forward_list<int> lst4(lst2);
    EXPECT_EQ(std::distance(lst4.begin(), lst4.end()),5);
    EXPECT_TRUE(std::equal(lst4.begin(), lst4.end(), std::begin({0,0,0,0,0})));

    std::forward_list<int> lst5(std::forward_list<int>(3,1));
    EXPECT_EQ(std::distance(lst5.begin(), lst5.end()),3);
    EXPECT_TRUE(std::equal(lst5.begin(), lst5.end(), std::begin({1,1,1})));

    std::forward_list<int> lst6{1,2,3,4,5};
    EXPECT_EQ(std::distance(lst6.begin(), lst6.end()),5);
    EXPECT_TRUE(std::equal(lst6.begin(), lst6.end(), std::begin({1,2,3,4,5})));

    //operator = 
    std::forward_list<int> lst7;
    lst7 = lst6;
    EXPECT_EQ(std::distance(lst7.begin(), lst7.end()),5);
    EXPECT_TRUE(std::equal(lst7.begin(), lst7.end(), std::begin({1,2,3,4,5})));

    std::forward_list<int> lst8;
    lst8 = std::move(lst7);
    EXPECT_EQ(std::distance(lst8.begin(), lst8.end()),5);
    EXPECT_TRUE(std::equal(lst8.begin(), lst8.end(), std::begin({1,2,3,4,5})));

    std::forward_list<int> lst9 = {11,22,33,44,55}; 
    EXPECT_EQ(std::distance(lst9.begin(), lst9.end()),5);
    EXPECT_TRUE(std::equal(lst9.begin(), lst9.end(), std::begin({11,22,33,44,55})));

    //assign
    std::forward_list<int> assignlst;
    assignlst.assign(5,4);
    EXPECT_EQ(std::distance(assignlst.begin(), assignlst.end()),5);
    EXPECT_TRUE(std::equal(assignlst.begin(), assignlst.end(), std::begin({4,4,4,4,4})));

    auto arr = std::to_array({5,5,5});
    assignlst.assign(arr.begin(),arr.end());
    EXPECT_EQ(std::distance(assignlst.begin(), assignlst.end()),3);
    EXPECT_TRUE(std::equal(assignlst.begin(), assignlst.end(), std::begin({5,5,5})));

    assignlst.assign({11,22,33,44,55});
    EXPECT_EQ(std::distance(assignlst.begin(), assignlst.end()),5);
    EXPECT_TRUE(std::equal(assignlst.begin(), assignlst.end(), std::begin({11,22,33,44,55})));
}

TEST(F_List, ElementAccess)
{
    std::forward_list<std::string> lst{"Berlin", "Paris", "Brussels"};
    EXPECT_STREQ(lst.front().c_str(), "Berlin");
}

TEST(F_Test, Iterators)
{
    std::forward_list<std::string> lst{"Germany", "France", "Spain"};
    EXPECT_STREQ(lst.begin()->c_str(),"Germany");

    //Just fetch and compare same strings by two different pointers for same container
    for(auto it1 = lst.begin(), it2 = lst.begin(); it1 != lst.end(), it2 != lst.end(); it1++, it2++ )
        EXPECT_STREQ(it1->c_str(),it2->c_str());
    
    auto it = lst.begin();
    while(std::next(it) != lst.end())++it;
   
    EXPECT_STREQ(it->c_str(), "Spain");
}

TEST(F_List, Capacity)
{
    std::forward_list<int> lst;
    EXPECT_TRUE(lst.empty());
}

TEST(F_List, Modifiers)
{   
    //clear
    std::forward_list<std::string> lst{"1", "2", "3", "4"};
    EXPECT_EQ(std::distance(lst.begin(),lst.end()), 4);
    lst.clear();
    EXPECT_EQ(lst.begin(), lst.end());
    EXPECT_EQ(std::distance(lst.begin(),lst.end()), 0);

    //insert_after
    lst.assign({"This", "Is", "Forward", "List"});
    lst.insert_after(lst.begin(),"Testing");
    auto it = lst.begin(); it++;
    EXPECT_STREQ(it->c_str(),"Testing");

    lst.insert_after(it,2,"->");
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"This","Testing","->","->","Is","Forward","List"})));

    std::array<std::string,3> arr{"practice","purpose", "only"};
    lst.insert_after(lst.begin(),arr.begin(), arr.end());
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"This","practice","purpose", "only","Testing","->","->","Is","Forward","List"})));

    //erase_after
    lst.erase_after(lst.begin());
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"This","purpose", "only","Testing","->","->","Is","Forward","List"})));

    auto pos1 = std::next(lst.begin(),1);
    auto pos2 = std::next(lst.begin(),6);
    lst.erase_after(lst.begin(),pos2);
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"This","Is","Forward","List"})));

    //emplace_after
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

    std::forward_list<TestConstructor> fList{1};
    fList.emplace_after(fList.begin(),2);
    fList.emplace_after(std::next(fList.begin(),1),3);
    fList.emplace_after(std::next(fList.begin(),2),4);

    auto compare = [](TestConstructor &obj, int val){ return obj.getValue() == val;};
    EXPECT_TRUE(std::equal(fList.begin(), fList.end(), std::begin({1,2,3,4}), compare));

    //emplace_front
    fList.emplace_front(5);
    fList.emplace_front(6);
    EXPECT_TRUE(std::equal(fList.begin(), fList.end(), std::begin({6,5,1,2,3,4}), compare));

    //push_front
    //insert_front lets use the last state of lst above which is in this state {"This","Is","Forward","List"}
    lst.push_front("STL");
    lst.push_front("Base");
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"Base","STL","This","Is","Forward","List"})));

    //pop_front
    lst.pop_front();
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"STL","This","Is","Forward","List"})));
    lst.pop_front();
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"This","Is","Forward","List"})));

    //resize
    lst.resize(2);
    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"This","Is"})));

    std::forward_list<std::string> lst2{"Great", "C++"};
    lst2.swap(lst);

    EXPECT_TRUE(std::equal(lst.begin(),lst.end(), std::begin({"Great", "C++"})));
    EXPECT_TRUE(std::equal(lst2.begin(),lst2.end(), std::begin({"This", "Is"})));
}

TEST(F_List, Operations)
{
    //merge
    std::forward_list<int> lst1 = {1,4,3};
    std::forward_list<int> lst2 = {2,6,5};
    
    lst1.sort();
    lst2.sort();

    lst1.merge(lst2);
    EXPECT_TRUE(std::ranges::equal(lst1, std::initializer_list<int>{1,2,3,4,5,6}));

    //splice
    std::forward_list<int> lst3{11,22,33};
    lst3.splice_after(std::next(lst3.begin(),1),lst1);
    EXPECT_TRUE(std::ranges::equal(lst3, std::initializer_list<int>{11,22,1,2,3,4,5,6,33}));

    std::forward_list<int> lst4{111,222,333};
    lst3.splice_after(std::next(lst3.begin()),lst4,std::next(lst4.begin(),1));
    EXPECT_TRUE(std::ranges::equal(lst3, std::initializer_list<int>{11,22,333,1,2,3,4,5,6,33}));

    std::forward_list<int> lst5{555};
    lst5.splice_after(lst5.begin(),lst3,std::next(lst3.begin(),2),std::next(lst3.begin(),9));
    EXPECT_TRUE(std::ranges::equal(lst5, std::initializer_list<int>{555,1,2,3,4,5,6}));

    //remove
    std::forward_list<int> rList{1,2,3,2,4,5,4,6,7};
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
    std::forward_list<int> uList{1,22,22,5,6,77,88};
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
    std::forward_list<int> lst1{1,2,3,4};
    std::forward_list<int> lst2{1,2,3,4,5};
    std::forward_list<int> lst3{1,2,3,4};

    EXPECT_TRUE(lst1==lst3);
    EXPECT_FALSE(lst1==lst2);

    EXPECT_TRUE(lst1 < lst2);
    EXPECT_TRUE(lst2 > lst1);

    EXPECT_TRUE(lst1 <= lst2);
    EXPECT_TRUE(lst2 >= lst1);

    std::swap(lst1, lst2);
    EXPECT_TRUE(std::equal(lst1.begin(), lst1.end(), std::begin({1,2,3,4,5})));
    EXPECT_TRUE(std::equal(lst2.begin(), lst2.end(), std::begin({1,2,3,4})));

    lst3.insert_after(std::next(lst3.begin(),3),3);
    lst3.insert_after(std::next(lst3.begin(),4),4);    
    lst3.insert_after(std::next(lst3.begin(),5),5);

    std::erase(lst3,4);
    EXPECT_TRUE(std::equal(lst3.begin(), lst3.end(), std::begin({1,2,3,3,5})));

    lst3.insert_after(std::next(lst3.begin(),3),6);
    lst3.insert_after(std::next(lst3.begin(),3),8);;
    //The list would have values of {1,2,3,3,5,6,8}
    // Now delete all even numbers by using predicate
    auto deleteEvenNumbers = [](int num){return (num % 2 ) == 0;};
    std::erase_if(lst3, deleteEvenNumbers);
    EXPECT_TRUE(std::equal(lst3.begin(), lst3.end(), std::begin({1,3,3,5})));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}