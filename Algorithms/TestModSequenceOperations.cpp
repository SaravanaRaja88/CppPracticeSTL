#include <iostream>
#include <gtest/gtest.h>
#include <algorithm>

TEST(ModifyingSequeneOperationsAlgorithms, copy)
{
    std::vector<int> vec{1,2,3,4,5,6,7,8,9,10};
    std::vector<int> copyVec(vec.size());

    std::copy(vec.begin(), vec.end(),copyVec.begin());
    EXPECT_TRUE(std::ranges::equal(vec,copyVec));

    copyVec.clear();
    std::ranges::copy(vec.begin(), vec.end(),std::back_inserter(copyVec));
    EXPECT_TRUE(std::ranges::equal(vec,copyVec));

    copyVec.clear();
    auto copyEven = [](int &num){ return (num % 2 == 0);};
    std::copy_if(vec.begin(),vec.end(), std::back_inserter(copyVec),copyEven);
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({2,4,6,8,10})));

    copyVec.clear();
    std::ranges::copy_if(vec.begin(),vec.end(), std::back_inserter(copyVec),copyEven);
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({2,4,6,8,10})));
}

TEST(ModifyingSequeneOperationsAlgorithms, copy_n)
{
    std::string str("This is a string for copy_n testing");
    std::string str2;

    std::copy_n(str.begin(),7,str2.begin());
    EXPECT_STREQ(str2.c_str(),"This is");

    str2.clear();
    std::ranges::copy_n(str.begin(),7,str2.begin());
    EXPECT_STREQ(str2.c_str(),"This is");

}

TEST(ModifyingSequeneOperationsAlgorithms, copy_backward)
{
    std::vector<int> vec{1,3,7,95,3,83,34};
    std::vector<int> copyVec(7);

    std::copy_backward(vec.begin(),vec.end(),copyVec.end());
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({1,3,7,95,3,83,34})));

    copyVec.clear();
    copyVec.resize(7);
    std::ranges::copy_backward(vec.begin(),vec.end(),copyVec.end());
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({1,3,7,95,3,83,34})));

}

TEST(ModifyingSequeneOperationsAlgorithms, move)
{
    struct NotCopyable
    {
        NotCopyable() = default;
        NotCopyable(const NotCopyable&) = delete;
        NotCopyable& operator = (const NotCopyable&) = delete;

        NotCopyable(NotCopyable&&) = default;
        NotCopyable& operator = (NotCopyable&&) = default;
    };

    std::vector<NotCopyable> vec;
    vec.push_back(NotCopyable());
    vec.push_back(NotCopyable());
    vec.push_back(NotCopyable());
    std::vector<NotCopyable> copyVec(vec.size());

    std::move(vec.begin(),vec.end(),copyVec.begin());
    EXPECT_EQ(copyVec.size(),vec.size());
    
    copyVec.clear();
    vec.clear();

    vec.push_back(NotCopyable());
    vec.push_back(NotCopyable());
    vec.push_back(NotCopyable());

    copyVec.resize(vec.size());

    std::ranges::move(vec.begin(),vec.end(),copyVec.begin());
    EXPECT_EQ(copyVec.size(),vec.size());
}

TEST(ModifyingSequeneOperationsAlgorithms, fill)
{
    std::string str("Testing");

    std::fill(str.begin(),str.end(),'s');
    EXPECT_STREQ(str.c_str(),"sssssss");

    std::ranges::fill(str.begin(),str.end(),'a');
    EXPECT_STREQ(str.c_str(),"aaaaaaa");

    std::fill_n(str.begin(),3,'b');
    EXPECT_STREQ(str.c_str(),"bbbaaaa");

    std::ranges::fill_n(str.begin(),3,'-');
    EXPECT_STREQ(str.c_str(),"---aaaa");
}

TEST(ModifyingSequeneOperationsAlgorithms, transform)
{
    std::vector<char> vec{'A','A','A','A','B','B','B','B','C','C','C','C'};

    auto charReplace = [](char &c){ return c = '-';};
    std::transform(vec.begin()+1,vec.begin()+4,vec.begin()+1,charReplace);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<char>({'A','-','-','-','B','B','B','B','C','C','C','C'})));

    std::ranges::transform(vec.begin(),vec.end(),vec.begin(),charReplace);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<char>({'-','-','-','-','-','-','-','-','-','-','-','-'})));

    std::vector<int> vec1{1,2,3,4,5};
    
    auto addition = [](int a, int b){ return a+b;};
    std::transform(vec1.begin(),vec1.end(),vec1.begin(),vec1.begin(), addition);
    EXPECT_TRUE(std::ranges::equal(vec1, std::initializer_list<int>({2,4,6,8,10})));

    std::ranges::transform(vec1,vec1,vec1.begin(),addition);
    EXPECT_TRUE(std::ranges::equal(vec1, std::initializer_list<int>({4,8,12,16,20})));

}

TEST(ModifyingSequeneOperationsAlgorithms, generate)
{
    std::vector<int> evenNumbers(10);

    //For all odd numbers initilize with 0, for rest initilize with even number
    auto initilizeWithEvenNumbers = [num = 1]() mutable {  
                                                            if(num % 2 == 0)
                                                                return num++;
                                                            else  
                                                                num++;
                                                                return 0;};

    std::generate(evenNumbers.begin(),evenNumbers.end(),initilizeWithEvenNumbers);
    EXPECT_TRUE(std::ranges::equal(evenNumbers, std::initializer_list<int>({0,2,0,4,0,6,0,8,0,10})));

    //For all even numbers initilize with 0 and rest with the odd numbers
    std::vector<int> oddNumbers(10);
    auto initilizeWithOddNumbers = [num = 1]() mutable {  
                                                            if(num % 2 == 0)
                                                            {
                                                                num++;
                                                                return 0;
                                                            }
                                                            else  
                                                                return num++;};

    std::ranges::generate(oddNumbers.begin(),oddNumbers.end(),initilizeWithOddNumbers);
    EXPECT_TRUE(std::ranges::equal(oddNumbers, std::initializer_list<int>({1,0,3,0,5,0,7,0,9,0})));

    evenNumbers.clear();
    oddNumbers.clear();
    evenNumbers.resize(6);
    oddNumbers.resize(6);

    std::generate_n(evenNumbers.begin(),6,initilizeWithEvenNumbers);
    EXPECT_TRUE(std::ranges::equal(evenNumbers, std::initializer_list<int>({0,2,0,4,0,6})));

    std::ranges::generate_n(oddNumbers.begin(),6,initilizeWithOddNumbers);
    EXPECT_TRUE(std::ranges::equal(oddNumbers, std::initializer_list<int>({1,0,3,0,5,0})));
}

TEST(ModifyingSequeneOperationsAlgorithms, remove)
{
    std::string str("Hello world this is remove testing");

    auto newStrPosition = std::remove(str.begin(),str.end(),'o');
    str.erase(newStrPosition, str.end());
    EXPECT_STREQ(str.c_str(),"Hell wrld this is remve testing");

    std::vector vec{1,2,3,4,5,6,7,8,9,10};

    auto newVecPosition = std::remove_if(vec.begin(),vec.end(),[](int &num){return num % 2 == 0;});
    vec.erase(newVecPosition,vec.end());
    EXPECT_TRUE(std::ranges::equal(vec, std::initializer_list<int>({1,3,5,7,9})));

    auto ret = std::ranges::remove(str.begin(),str.end(),'i');
    str.erase(ret.begin(),ret.end());
    EXPECT_STREQ(str.c_str(),"Hell wrld ths s remve testng");

    auto retVec = std::remove_if(vec.begin(),vec.end(),[](int &num){return num % 3 == 0;});
    vec.erase(retVec,vec.end());
    EXPECT_TRUE(std::ranges::equal(vec, std::initializer_list<int>({1,5,7})));
}

TEST(ModifyingSequeneOperationsAlgorithms, remove_copy)
{
    std::string str("Testing The Remove_copy Api Now");
    std::string str2;
    
    auto ret = std::remove_copy(str.begin(),str.end(),str.begin(),'e');
    str.erase(ret,str.end());
    EXPECT_STREQ(str.c_str(),"Tsting Th Rmov_copy Api Now");

    auto ret2 = std::remove_copy_if(str.begin(),str.end(),str.begin(),[](char &ch){return islower(ch);});
    str.erase(ret2,str.end());
    EXPECT_STREQ(str.c_str(),"T T R_ A N");
    str.clear();

    str.assign({"Testing, The, std::ranges, remove_copy,"});
    auto ret3 = std::ranges::remove_copy(str.begin(),str.end(),str.begin(),',');
    str.erase(ret3.out,str.end());
    EXPECT_STREQ(str.c_str(),"Testing The std::ranges remove_copy");

    auto ret4 = std::ranges::remove_copy_if(str.begin(),str.end(),str2.begin(),[](char &ch){return islower(ch);});
    str2.erase(ret4.out,str2.end());
    EXPECT_STREQ(str2.c_str(),"T T :: _");
}

TEST(ModifyingSequeneOperationsAlgorithms, replace)
{
    std::vector<int> vec{1,2,3,4,5,6,7,8,9,10};

    std::replace(vec.begin(),vec.end(),2,22);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,22,3,4,5,6,7,8,9,10})));
    std::replace(vec.begin(),vec.end(),10,100);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,22,3,4,5,6,7,8,9,100})));

    auto evenNumberReplace = [](int &num){return num%2 == 0;};
    std::replace_if(vec.begin(),vec.end(),evenNumberReplace,0);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,0,3,0,5,0,7,0,9,0})));

    std::ranges::replace(vec,0,1);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,1,3,1,5,1,7,1,9,1})));

    auto replace1 = [](int &num){return num == 1;};
    std::ranges::replace_if(vec,replace1,100);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({100,100,3,100,5,100,7,100,9,100})));
}

TEST(ModifyingSequeneOperationsAlgorithms, replace_copy)
{
    std::string str("Testing, With, The, Replace_copy, std::Api,");
    std::string str2(str.length(),' '); 

    auto ret = std::replace_copy(str.begin(),str.end(),str2.begin(),',','_');
    str2.erase(ret,str2.end());
    EXPECT_STREQ(str2.c_str(),"Testing_ With_ The_ Replace_copy_ std::Api_");
    
    str2.clear();
    str2.resize(str.length());
    auto ret2 = std::ranges::replace_copy(str,str2.begin(),',','=');
    str2.erase(ret2.out,str2.end());
    EXPECT_STREQ(str2.c_str(),"Testing= With= The= Replace_copy= std::Api=");

    str2.clear(); 
    str2.resize(str.length());
    auto replaceChar = [](char c){ return c == ',';};
    auto ret3 = std::replace_copy_if(str.begin(),str.end(),str2.begin(),replaceChar,'-');
    str2.erase(ret3,str2.end());
    EXPECT_STREQ(str2.c_str(),"Testing- With- The- Replace_copy- std::Api-");

    str2.clear(); 
    str2.resize(str.length());
    auto ret4 = std::ranges::replace_copy_if(str,str2.begin(),replaceChar,' ');
    str2.erase(ret4.out,str2.end());
    EXPECT_STREQ(str2.c_str(),"Testing  With  The  Replace_copy  std::Api ");
}

TEST(ModifyingSequeneOperationsAlgorithms, reverse)
{
    std::string str("This is the testing for reverse api");

    std::reverse(str.begin(), str.end());
    EXPECT_STREQ(str.c_str(),"ipa esrever rof gnitset eht si sihT");

    str.clear();
    str.assign("ABCDEF");

    std::ranges::reverse(str);
    EXPECT_STREQ(str.c_str(),"FEDCBA");

}

TEST(ModifyingSequeneOperationsAlgorithms, reverse_copy)
{
    std::vector<int> vec{1,2,3,4,5};
    std::vector<int> copyVec(vec.size());

    std::reverse_copy(vec.begin(), vec.end(),copyVec.begin());
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({5,4,3,2,1})));

    std::string str("Hello 1");
    std::string copyStr; copyStr.resize(str.length());
    std::ranges::reverse_copy(str,copyStr.begin());
    EXPECT_STREQ(copyStr.c_str(),"1 olleH");
}

TEST(ModifyingSequeneOperationsAlgorithms, rotate)
{
    std::vector<int> vec{1,2,3,4,5,6,7,8,9,10};

    std::rotate(vec.begin(), vec.begin()+5, vec.end());
    EXPECT_TRUE(std::ranges::equal(vec, std::initializer_list<int>({6,7,8,9,10,1,2,3,4,5})));
    
    std::ranges::sort(vec);
    std::rotate(vec.begin(), vec.begin()+7, vec.end());
    EXPECT_TRUE(std::ranges::equal(vec, std::initializer_list<int>({8,9,10,1,2,3,4,5,6,7})));

    std::ranges::sort(vec);
    std::rotate(vec.rbegin(), vec.rbegin()+3, vec.rend());
    EXPECT_TRUE(std::ranges::equal(vec, std::initializer_list<int>({8,9,10,1,2,3,4,5,6,7})));

    std::string str("RotateString");

    std::ranges::rotate(str.begin(),str.begin()+5,str.end());
    EXPECT_STREQ(str.c_str(),"eStringRotat");

    std::ranges::rotate(str.rbegin(),str.rbegin()+3,str.rend());
    EXPECT_STREQ(str.c_str(),"tateStringRo");
}

TEST(ModifyingSequeneOperationsAlgorithms, rotate_copy)
{
    std::string str("RotateCopy");
    std::string copyStr(str.size(), ' ');

    std::rotate_copy(str.begin(),std::ranges::find(str,'e'),str.end(),copyStr.begin());
    EXPECT_STREQ(copyStr.c_str(),"eCopyRotat");
    
    copyStr.clear(); copyStr.resize(str.size());
    std::ranges::rotate_copy(str,std::ranges::find(str,'a'),copyStr.begin());
    EXPECT_STREQ(copyStr.c_str(),"ateCopyRot");

    copyStr.clear(); copyStr.resize(str.size());
    std::ranges::rotate_copy(str.begin(),std::ranges::find(str,'p'),str.end(),copyStr.begin());
    EXPECT_STREQ(copyStr.c_str(),"pyRotateCo");
}

TEST(ModifyingSequeneOperationsAlgorithms, shift_leftAndshift_right)
{
    std::vector<int> vec{1,2,3,4,5,7};

    std::shift_left(vec.begin(),vec.end(),2);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({3,4,5,7,5,7})));
    
    std::shift_right(vec.begin(),vec.end(),2);
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({3,4,3,4,5,7})));
    
}

TEST(ModifyingSequeneOperationsAlgorithms, unique)
{
    std::vector<int> vec{1,2,1,2,3,4,4,3,5};

    auto ret = std::unique(vec.begin(),vec.end());
    vec.erase(ret,vec.end());
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,2,1,2,3,4,3,5})));

    std::ranges::sort(vec);
    auto ret2 = std::unique(vec.begin(),vec.end());
    vec.erase(ret2,vec.end());
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,2,3,4,5})));

    std::string str("There");
    auto removeConsecutive = [](char a, char b){ return a == b;};
    
    std::ranges::sort(str);
    auto ret3 = std::unique(str.begin(),str.end(),removeConsecutive);
    str.erase(ret3,str.end());
    EXPECT_STREQ(str.c_str(),"Tehr");

    vec.clear();
    str.clear();

    vec.assign({1,3,4,2,4,6,7});
    //Now the vec has the elements 1,3,4,2,4,6,7 so lets sort this and make consecutive elements next to 
    //eachohter and then repeat the unique using ranges::unique
    std::ranges::sort(vec);
    auto ret4 = std::ranges::unique(vec);
    vec.erase(ret4.begin(),ret4.end());
    EXPECT_TRUE(std::ranges::equal(vec,std::initializer_list<int>({1,2,3,4,6,7})));

    str.assign("Hello");
    auto ret5 = std::ranges::unique(str.begin(),str.end(),removeConsecutive);
    str.erase(ret5.begin(),ret5.end());
    EXPECT_STREQ(str.c_str(),"Helo");

}

TEST(ModifyingSequeneOperationsAlgorithms, unique_copy)
{
    std::vector<int> vec{1,1,2,3,3,4,5,5}, copyVec;

    std::unique_copy(vec.begin(),vec.end(),std::back_inserter(copyVec));
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({1,2,3,4,5})));

    copyVec.clear(); copyVec.resize(vec.size());
    auto ret = std::ranges::unique_copy(vec,copyVec.begin());
    copyVec.erase(ret.out,copyVec.end());
    EXPECT_TRUE(std::ranges::equal(copyVec,std::initializer_list<int>({1,2,3,4,5})));
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}