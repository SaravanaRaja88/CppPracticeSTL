#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>
#include <execution>

TEST(Algorithms, NonModifyingSequeneOperations)
{
    //all_of and ranges::all_of
    std::vector<int> vec{1,2,3,4,7,8,9,11};

    auto findEvenNumbers = [](int &num){ return (num % 2) == 0;};
    EXPECT_FALSE(std::all_of(vec.begin(),vec.end(),findEvenNumbers));
    EXPECT_FALSE(std::ranges::all_of(vec.begin(),vec.end(),findEvenNumbers));


    vec.assign({2,4,6,8,10});
    EXPECT_TRUE(std::all_of(std::execution::par,vec.begin(),vec.end(),findEvenNumbers));
    EXPECT_TRUE(std::ranges::all_of(vec.begin(),vec.end(),findEvenNumbers));

    //any_of and ranges::any_of
    vec.assign({1,3,5,7,9});
    EXPECT_FALSE(std::any_of(vec.begin(),vec.end(),findEvenNumbers));
    vec.assign({1,3,5,7,9,2});
    EXPECT_TRUE(std::any_of(std::execution::par,vec.begin(),vec.end(),findEvenNumbers));
    EXPECT_TRUE(std::ranges::any_of(vec.begin(),vec.end(),findEvenNumbers));


    //none_of and ranges::none_of
    auto noneOfNegativeNumbers = [](int &num){ return num < 0;};
    EXPECT_TRUE(std::none_of(vec.begin(),vec.end(),noneOfNegativeNumbers));
    EXPECT_TRUE(std::ranges::none_of(vec.begin(),vec.end(),noneOfNegativeNumbers));
    
    vec.assign({1,3,5,7,9,2,-1});

    EXPECT_FALSE(std::none_of(std::execution::par,vec.begin(),vec.end(),noneOfNegativeNumbers));

    //for_each vec elements are {1,3,5,7,9,2,-1} we will find the numbers divisible by 3 
    //so the result we have to get 3 and 9
    struct DivisibleNumber
    {
        std::vector<int> retDivisibleNumbers{};
        int mdivisibleNumber;

        DivisibleNumber(int divisibleNumber):mdivisibleNumber(divisibleNumber){}

        void operator()(int num){
            if(num % mdivisibleNumber == 0)
                retDivisibleNumbers.push_back(num);
        }
    };

    auto divisibleNumbersListBy3 = std::for_each(vec.begin(),vec.end(),DivisibleNumber(3));
    EXPECT_TRUE(std::ranges::equal(divisibleNumbersListBy3.retDivisibleNumbers,std::initializer_list<int>({3,9})));

    auto divisibleNumbersListBy2 = std::ranges::for_each(vec.begin(),vec.end(),DivisibleNumber(2));
    EXPECT_TRUE(std::ranges::equal(divisibleNumbersListBy2.fun.retDivisibleNumbers, std::initializer_list<int>({2})));

    //for_each_n and ranges::for_each_n
    std::for_each_n(vec.begin(),2,[](auto& n) { n *= 3; });
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({3,9,5,7,9,2,-1})));

    auto retVal = std::ranges::for_each_n(vec.begin(),3,[](auto& n) { n *= 3; });
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({9,27,15,7,9,2,-1})));

    //count and count_if
    vec.clear();
    vec.assign({1,3,4,6,7,9,4,10,4,12,2,2,34});
    EXPECT_EQ(std::count(vec.begin(),vec.end(),4),3);
    EXPECT_EQ(std::count(vec.begin(),vec.end(),2),2);
    EXPECT_EQ(std::count(vec.begin(),vec.end(),0),0);

    auto countEven = [](int &num){ return num % 2 == 0;};
    EXPECT_EQ(std::count_if(vec.begin(),vec.end(),countEven),9);

    EXPECT_EQ(std::ranges::count(vec.begin(),vec.end(),-1),0);
    EXPECT_EQ(std::ranges::count(vec.begin(),vec.end(),34),1);

    auto countOddNumbers = [](int &num){ return !(num % 2 == 0);};
    EXPECT_EQ(std::ranges::count_if(vec.begin(),vec.end(),countOddNumbers),4);

    //mismatch
    std::string str1 = "Helpo";
    std::string str2 = "Helso";

    auto retVal2 = std::mismatch(str1.begin(),str1.end(),str2.begin());
    EXPECT_EQ(*retVal2.first,'p');
    EXPECT_EQ(*retVal2.second,'s');

    auto retVal3 = std::ranges::mismatch(str1.begin(),str1.end(),str2.begin(),str2.end());
    EXPECT_EQ(*retVal3.in1,'p');
    EXPECT_EQ(*retVal3.in2,'s');

    str1.assign("helPso");
    str2.assign("helSe");

    auto findCapital = [](char first, char second){ return !((first >= 'A' && first <= 'Z') && 
                                                   (second >= 'A' && second <='Z'));};

    auto retVal4 = std::mismatch(str1.begin(),str1.end(),str2.begin(),findCapital);
    EXPECT_EQ(*retVal4.first,'P');
    EXPECT_EQ(*retVal4.second,'S');

    auto retVal5 = std::ranges::mismatch(str1.begin(),str1.end(),str2.begin(),str2.end(),findCapital);
    EXPECT_EQ(*retVal5.in1,'P');
    EXPECT_EQ(*retVal5.in2,'S');

    //find, find_if and find_if_not
    vec.clear();
    vec.assign({1,2,3,4,5,4,6});

    auto fRetVal = std::find(vec.begin(),vec.end(),2);
    EXPECT_EQ(*fRetVal,2);
    EXPECT_EQ(*std::find(vec.begin(),vec.end(),10),*vec.end());

    auto fRetVal2 = std::ranges::find(vec.begin(),vec.end(),3);
    EXPECT_EQ(*fRetVal2,3);

    auto isEven = [](int &num){ return (num %2 == 0);};
    EXPECT_EQ(*std::find_if(vec.begin(),vec.end(),isEven),2);
    EXPECT_EQ(*std::ranges::find_if(vec.begin(),vec.end(),isEven),2);

    EXPECT_EQ(*std::find_if_not(vec.begin(),vec.end(),isEven),1);
    EXPECT_EQ(*std::ranges::find_if_not(vec.begin(),vec.end(),isEven),1);
}

TEST(NonModifyingSequeneOperationsAlgorithms, find_end)
{
    std::vector<int> vec{1,2,3,1,2,3,4,1,2,3,6};
    std::vector<int> vec2{1,2,3};

    auto ret = std::find_end(vec.begin(),vec.end(),vec2.begin(),vec2.end());
    EXPECT_EQ(std::distance(vec.begin(),ret),7);

    auto compare = [](int a, int b){ return a == b;};
    auto ret2 = std::find_end(vec.begin(),vec.end(),vec2.begin(),vec2.end(),compare);
    EXPECT_EQ(std::distance(vec.begin(),ret2),7);

    vec2.assign({1,2});
    auto ret3 = std::ranges::find_end(vec.begin(),vec.end(),vec2.begin(),vec2.end());
    EXPECT_EQ(std::distance(vec.begin(),ret3.begin()),7);

    auto ret4 = std::ranges::find_end(vec.begin(),vec.end(),vec2.begin(),vec2.end(),compare);
    EXPECT_EQ(std::distance(vec.begin(),ret4.begin()),7);

}

TEST(NonModifyingSequeneOperationsAlgorithms, find_first_of)
{
    std::string str("Hello World");
    std::string compareStr("Good");

    auto it = std::find_first_of(str.begin(),str.end(),compareStr.begin(),compareStr.end());
    EXPECT_EQ(*it,'o');
    EXPECT_EQ(std::distance(str.begin(),it),4);

    auto it2 = std::find_first_of(str.begin(),str.end(),compareStr.begin(),compareStr.end(),[](char a, char b){return (a == b);});
    EXPECT_EQ(*it2,'o');
    EXPECT_EQ(std::distance(str.begin(),it2),4);

    auto it3 = std::ranges::find_first_of(str.begin(),str.end(),compareStr.begin(),compareStr.end());
    EXPECT_EQ(*it3,'o');
    EXPECT_EQ(std::distance(str.begin(),it3),4);

    auto it4 = std::ranges::find_first_of(str.begin(),str.end(),compareStr.begin(),compareStr.end(),[](char a, char b){return (a == b);});
    EXPECT_EQ(*it4,'o');
    EXPECT_EQ(std::distance(str.begin(),it4),4);

}

TEST(NonModifyingSequeneOperationsAlgorithms,adjacent_find)
{
    std::array<int,10> arr{1,2,3,4,4,5,6,6,7,10};

    auto it = std::adjacent_find(arr.begin(),arr.end());
    EXPECT_EQ(*it,4);
    EXPECT_EQ(std::distance(arr.begin(),it),3);

    auto it2 = std::ranges::adjacent_find(arr.rbegin(),arr.rend());
    EXPECT_EQ(*it2,6);
    EXPECT_EQ(std::distance(arr.rbegin(),it2),2);

    std::array<int,10> arr2{1,2,3,-3,4,5,-5};
    auto adjacentWithNegative = [](int &num1, int&num2){ return (num1 == -(num2));};
    auto it3 = std::adjacent_find(arr2.begin(),arr2.end(),adjacentWithNegative);
    EXPECT_EQ(*it3,3);
    EXPECT_EQ(std::distance(arr2.begin(),it3),2);

    auto it4 = std::ranges::adjacent_find(arr2.begin(),arr2.end(),adjacentWithNegative);
    EXPECT_EQ(*it4,3);
    EXPECT_EQ(std::distance(arr2.begin(),it4),2);
}

TEST(NonModifyingSequeneOperationsAlgorithms,search)
{
    std::string str = "Hello world good people going to search substrings";
    std::string subString = "oing";

    auto it = std::search(str.begin(),str.end(),subString.begin(),subString.end());
    EXPECT_TRUE(std::equal(it,it+subString.length(),"oing"));

    auto compare = [](char a, char b){ return (a == b);};

    auto it2 = std::search(str.begin(),str.end(),subString.begin(),subString.end(),compare);
    EXPECT_TRUE(std::equal(it2,it2+subString.length(),"oing"));

    auto it3 = std::search(str.begin(),str.end(),subString.begin(),subString.end(),compare);
    EXPECT_FALSE(std::equal(it3,it3+subString.length(),"food"));

    auto it4 = std::ranges::search(str.begin(),str.end(),subString.begin(),subString.end());
    EXPECT_TRUE(std::ranges::equal(it4,subString));

    auto it5 = std::ranges::search(str.begin(),str.end(),subString.begin(),subString.end(),compare);
    EXPECT_TRUE(std::ranges::equal(it5,subString));
}

TEST(NonModifyingSequeneOperationsAlgorithms,search_n)
{
    std::string str("Helllo wooorld");

    auto it = std::search_n(str.begin(),str.end(),3,'l');
    EXPECT_TRUE(std::equal(it,it+3,"lll"));

    auto it2 = std::ranges::search_n(str.begin(),str.end(),3,'o');
    EXPECT_TRUE(std::ranges::equal(it2,std::string{"ooo"}));

    auto it3 = std::search_n(str.begin(),str.end(),3,'l',[](char a, char b){return a == b;});
    EXPECT_TRUE(std::equal(it,it+3,"lll"));

    auto it4 = std::ranges::search_n(str.begin(),str.end(),3,'o',[](char a, char b){return a == b;});
    EXPECT_TRUE(std::ranges::equal(it4,std::string{"ooo"}));
 
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}