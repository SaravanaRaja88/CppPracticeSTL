#include <iostream>
#include <gtest/gtest.h>
#include <string>

TEST(String, MemberFunctions)
{
    //Constructors
    std::string str;
    EXPECT_TRUE(str.empty());

    std::string str1(5,'s');
    EXPECT_EQ(str1.size(),5);
    EXPECT_STREQ(str1.c_str(),"sssss");

    str.assign({"Hello the strings world is beautiful"});
    std::string str2(str,6);
    EXPECT_STREQ(str2.c_str(),"the strings world is beautiful");

    std::string str3(std::move(str2),4);
    EXPECT_STREQ(str3.c_str(),"strings world is beautiful");

    std::string str4(str3,8,5);
    EXPECT_STREQ(str4.c_str(),"world");

    std::string str5("Hello World",5);
    EXPECT_STREQ(str5.c_str(),"Hello");

    std::string str6("Hello World");
    EXPECT_STREQ(str6.c_str(),"Hello World");
    EXPECT_EQ(str6.size(),11);

    std::array<char, 25> arr{"Hello everything is good"};
    auto endIt = arr.begin();
    std::advance(endIt,16);
    std::string str7(arr.begin(),endIt);
    EXPECT_STREQ(str7.c_str(),"Hello everything");

    std::string str8(str7);
    EXPECT_STREQ(str7.c_str(),"Hello everything");

    std::string str9("Hello Good People");
    EXPECT_STREQ(str9.c_str(),"Hello Good People");

    std::string str10({"Initilized using initilizer list"});
    EXPECT_STREQ(str10.c_str(),"Initilized using initilizer list");

    std::string_view strView{"String views"};
    std::string str11(strView);
    EXPECT_STREQ(str11.c_str(),"String views");

    std::string str12(strView,7,4);
    EXPECT_STREQ(str12.c_str(),"view");

    //operator = 
    std::string str13 = str12;
    EXPECT_STREQ(str13.c_str(),"view");

    std::string str14 = std::move(str11);
    EXPECT_STREQ(str14.c_str(),"String views");

    std::string str15 = "Testing Strings";
    EXPECT_STREQ(str15.c_str(),"Testing Strings");

    str15 = "T";
    EXPECT_STREQ(str15.c_str(),"T");

    str15 = {"Again back for testing strings"};
    EXPECT_STREQ(str15.c_str(),"Again back for testing strings");

    str15 = strView;
    EXPECT_STREQ(str15.c_str(),"String views");

    //assign
    str15.assign(5,'S');
    EXPECT_STREQ(str15.c_str(),"SSSSS");

    str15.assign(str14);
    EXPECT_STREQ(str15.c_str(),"String views");

    str15.assign(str14,6,4);
    EXPECT_STREQ(str15.c_str()," vie");

    str15.assign("Interesting !!");
    EXPECT_STREQ(str15.c_str(),"Interesting !!");

    str15.assign("Interesting",5);
    EXPECT_STREQ(str15.c_str(),"Inter");

    str15.assign(arr.begin(),endIt);
    EXPECT_STREQ(str15.c_str(),"Hello everything");

    str15.assign({"Initilizer test"});
    EXPECT_STREQ(str15.c_str(),"Initilizer test");

    str15.assign(strView);
    EXPECT_STREQ(str15.c_str(),"String views");

    str15.assign(strView,2,4);
    EXPECT_STREQ(str15.c_str(),"ring");

}

TEST(String, ElementAccess)
{
    std::string str("Hello World");

    EXPECT_EQ(str.at(2),'l');
    EXPECT_ANY_THROW(str.at(20));

    EXPECT_EQ(str[4],'o');

    EXPECT_EQ(str.front(),'H');
    EXPECT_EQ(str.back(),'d');

    EXPECT_STREQ(str.data(),"Hello World");
    EXPECT_STREQ(str.c_str(),"Hello World");

    //str is converted into non modifiable string view
    std::string_view str2 = str;
}

TEST(String, Iterators)
{
    std::string str{"Playing with strings"};

    EXPECT_EQ(*str.begin(),'P');
    EXPECT_EQ(*(str.end()-1),'s');

    EXPECT_EQ(*str.rbegin(),'s');
    EXPECT_EQ(*(str.rend()-1),'P');

    auto it = str.begin();
    it++;
    EXPECT_EQ(*it,'l');
    std::advance(it,7);
    EXPECT_EQ(*it,'w');

    auto eIt = str.end();
    --eIt;
    EXPECT_EQ(*eIt,'s');
    std::advance(eIt,-4);
    EXPECT_EQ(*eIt,'r');
}

TEST(String, Capacity)
{
    std::string str;
    EXPECT_TRUE(str.empty());

    str.assign("Hello World");
    EXPECT_EQ(str.size(),11);
    EXPECT_EQ(str.length(),11);

    //Checking current capacity 
    EXPECT_EQ(str.capacity(),15);
    
    str.reserve(100);
    //Now check new capacity after increasing
    EXPECT_EQ(str.capacity(),100);

    str.shrink_to_fit();
    EXPECT_EQ(str.capacity(),15);
}

TEST(String, Operations)
{
    //clear
    std::string str = "Hello World";
    EXPECT_FALSE(str.empty());

    str.clear();
    EXPECT_TRUE(str.empty());

    //insert
    str.assign("Hello");
    str.insert(4,3,'O');
    EXPECT_STREQ(str.c_str(),"HellOOOo");

    str.insert(2," --- ");
    EXPECT_STREQ(str.c_str(),"He --- llOOOo");

    str.insert(1,"World",2);
    EXPECT_STREQ(str.c_str(),"HWoe --- llOOOo");

    str.clear();
    str.assign("Hello");

    std::string str2(" World");
    str.insert(str.size(),str2);
    EXPECT_STREQ(str.c_str(),"Hello World");

    str2.insert(4,str,2,4);
    EXPECT_STREQ(str2.c_str()," Worllo ld");

    str2.insert((str2.begin()+6),'s');
    EXPECT_STREQ(str2.c_str()," Worllso ld");

    str2.insert((str2.begin()+6),5,'p');
    EXPECT_STREQ(str2.c_str()," Worllpppppso ld");

    str.insert(str.end(),str2.begin(),str2.end());
    EXPECT_STREQ(str.c_str(),"Hello World Worllpppppso ld");

    str.clear();
    str2.clear();

    str.insert(str.begin(),{'a','b','c'});
    EXPECT_STREQ(str.c_str(),"abc");

    std::string_view vStr = str;
    str2.insert(0,vStr);
    EXPECT_STREQ(str2.c_str(),"abc");

    vStr = "Hello World";
    str2.insert(3,vStr,6,5);
    EXPECT_STREQ(str2.c_str(),"abcWorld");

    //erase
    str2.erase(3,3);
    EXPECT_STREQ(str2.c_str(),"abcld");

    str2.erase(str2.begin()+2);
    EXPECT_STREQ(str2.c_str(),"abld");

    str2.erase(str2.begin()+1, str2.end()-2);
    EXPECT_STREQ(str2.c_str(),"ald");

    //push_back
    str2.push_back('e');
    str2.push_back('f');
    EXPECT_STREQ(str2.c_str(),"aldef");

    //pop_back
    str2.pop_back();
    EXPECT_STREQ(str2.c_str(),"alde");

    //append
    str2.append(3,'s');
    EXPECT_STREQ(str2.c_str(),"aldesss");

    str.clear();
    str = "Hello";

    str2.append(str);
    EXPECT_STREQ(str2.c_str(),"aldesssHello");

    str = "World";
    str2.append(str,2,3);
    EXPECT_STREQ(str2.c_str(),"aldesssHellorld");

    str.append("Is Beautiful",9);
    EXPECT_STREQ(str.c_str(),"WorldIs Beauti");

    str.clear();
    str2.clear();

    str.append("Hello World");
    EXPECT_STREQ(str.c_str(),"Hello World");

    str2.append(str.begin()+2, str.end()-2);
    EXPECT_STREQ(str2.c_str(),"llo Wor");

    str2.append({' ','l','o','o','k'});
    EXPECT_STREQ(str2.c_str(),"llo Wor look");

    vStr = str2;
    str.append(vStr);
    EXPECT_STREQ(str.c_str(),"Hello Worldllo Wor look");

    str.append(vStr,2,2);
    EXPECT_STREQ(str.c_str(),"Hello Worldllo Wor looko ");

    //operator+=
    str.clear();
    str2.clear();
    vStr = "";

    str = "Hello";
    str += " World";

    EXPECT_STREQ(str.c_str(),"Hello World");

    str+= 's';
    EXPECT_STREQ(str.c_str(),"Hello Worlds");

    str += " great";
    EXPECT_STREQ(str.c_str(),"Hello Worlds great");

    str += {' ','S','t','r'};
    EXPECT_STREQ(str.c_str(),"Hello Worlds great Str");

    vStr = " Is best";
    str += vStr;
    EXPECT_STREQ(str.c_str(),"Hello Worlds great Str Is best");

    //compare
    str.clear();
    str2.clear();
    vStr = "";

    str.assign("Hello");
    str2.assign("World");

    EXPECT_LE(str.compare(str2),0);
    
    str2.assign("Boo");
    EXPECT_GE(str.compare(str2),0);

    str2.assign("Hello");
    EXPECT_EQ(str.compare(str2),0);

    str.assign("This is a big string");
    str2.assign("is");   //Find is substring in str
    EXPECT_EQ(str.compare(5,2,str2),0);
    
    str2.append(" big string"); //Find big substring from str2 in str
    EXPECT_EQ(str.compare(10,3,str2,3,3),0);

    str.clear();
    str2.clear();

    str.assign("Hello");
    EXPECT_EQ(str.compare("Hello"),0);

    str.append("Its beautiful");
    EXPECT_EQ(str.compare(5,3,"Its"),0);
    EXPECT_EQ(str.compare(9,3,"beautiful",3),0);

    str = "Cello";
    vStr = "Borld";
    EXPECT_GE(str.compare(vStr),0);
    EXPECT_LE(str.compare(2,2,vStr,2,2),0);

    //starts_with
    EXPECT_TRUE(str.starts_with('C'));
    EXPECT_TRUE(str.starts_with("Cell"));
    EXPECT_FALSE(str.starts_with(vStr));

    //ends_with
    EXPECT_TRUE(str.ends_with('o'));
    EXPECT_TRUE(str.ends_with("lo"));
    EXPECT_FALSE(str.ends_with(vStr));

    str2.assign(" World");
    //replace
    str.replace(2,3,str2);
    EXPECT_STREQ(str.c_str(),"Ce World");

    str.replace(str.begin(),str.end()-2,str2);
    EXPECT_STREQ(str.c_str()," Worldld");

    str.replace(2,3,str2,2,3);
    EXPECT_STREQ(str.c_str()," Worldld");

    str.clear();
    str.assign("Hello");

    str.replace(2,3,"Good",3);
    EXPECT_STREQ(str.c_str(),"HeGoo");

    str.replace(str.begin(),str.end(),"Testing",4);
    EXPECT_STREQ(str.c_str(),"Test");

    str.replace(3,2,"Yes");
    EXPECT_STREQ(str.c_str(),"TesYes");

    str.replace(str.begin(),str.end(),"Testing");
    EXPECT_STREQ(str.c_str(),"Testing");

    str.replace(2,3,4,'T');
    EXPECT_STREQ(str.c_str(),"TeTTTTng");

    str.replace(str.begin(),str.end(),str2.begin(),str2.end());
    EXPECT_STREQ(str.c_str()," World");

    str.replace(str.begin(),str.end(),{'V','I','P'});
    EXPECT_STREQ(str.c_str(),"VIP");

    vStr = "Test";
    str.replace(2,2,vStr);
    EXPECT_STREQ(str.c_str(),"VITest");

    str.replace(str.begin(),str.end(),vStr);
    EXPECT_STREQ(str.c_str(),"Test");

    str.replace(2,2,vStr,3,1);
    EXPECT_STREQ(str.c_str(),"Tet");

    //substr
    str.assign("Finding the substring in mainstring");
    auto substring = str.substr(12,9);
    EXPECT_STREQ(substring.c_str(),"substring");

    //copy
    char arr[10];
    str.copy(arr,9,12);
    EXPECT_STREQ(arr,"substring");

    //resize
    str.resize(8);
    EXPECT_STREQ(str.c_str(),"Finding ");

    str.resize(10,'b');
    EXPECT_STREQ(str.c_str(),"Finding bb");

    //swap
    str2 = "Swapping";
    str.swap(str2);
    EXPECT_STREQ(str.c_str(),"Swapping");
    EXPECT_STREQ(str2.c_str(),"Finding bb");

}

TEST(Strings, Search)
{
    std::string str("Searching for the substring and then at the at last beststring j");
    std::string substr = "substring";

    //find
    EXPECT_NE(str.find(substr,18),std::string::npos);
    EXPECT_EQ(str.find(substr,20),std::string::npos);

    EXPECT_NE(str.find("for",1,3),std::string::npos);
    EXPECT_NE(str.find("substr",18,6),std::string::npos);
    EXPECT_EQ(str.find("good",1,4),std::string::npos);

    EXPECT_NE(str.find("last",1),std::string::npos);
    EXPECT_EQ(str.find("last",50),std::string::npos);   

    std::string_view view("then");
    EXPECT_NE(str.find(view,1),std::string::npos);

    //Skipping the stringview and then the character constructor for below memeber functions
    //since its pretty same like above. 
    //rfind
    substr = "at";
    EXPECT_NE(str.rfind(substr,50),std::string::npos);
    EXPECT_EQ(str.rfind(substr,20),std::string::npos);

    EXPECT_NE(str.rfind("for",20,3),std::string::npos);
    EXPECT_EQ(str.rfind("for",1,3),std::string::npos);

    //find_first_of
    EXPECT_EQ(str.find_first_of(substr,0),2);
    substr = "k";
    EXPECT_EQ(str.find_first_of(substr,45),std::string::npos);
    
    EXPECT_EQ(str.find_first_of("for",0,2),10);
    EXPECT_EQ(str.find_first_of("at",0,2),2);

    //find_first_not_of
    substr = "Sarc";
    EXPECT_EQ(str.find_first_not_of(substr,0),1);

    EXPECT_EQ(str.find_first_not_of("Searching for the substring and then at the at oops last beststring ",0),63);

    EXPECT_EQ(str.find_first_not_of("Searching for the substring and then at the at last beststring j",0),std::string::npos);

    EXPECT_EQ(str.find_first_not_of("Searching ",0,10),10);

    //find_last_of
    substr = "last";
    EXPECT_EQ(str.find_last_of(substr,64),57);

    EXPECT_EQ(str.find_last_of("ou",64),19);

    EXPECT_EQ(str.find_last_of("kz",64),std::string::npos);

    //find_last_of_not
    str.clear();
    str = "This is a new string";
    substr = "ok string";

    EXPECT_EQ(str.find_last_not_of(substr,str.length()),12);

    EXPECT_EQ(str.find_last_not_of("kz",str.length()),19);

    EXPECT_EQ(str.find_last_not_of("string ",str.length(),7),12);

}

TEST(String, NonMemberFunctions)
{
    std::string str = "Hello ";
    std::string str2 = "World";
    std::string str3 = "Hello ";

    //operator+
    auto concat = str + str2;
    EXPECT_STREQ(concat.c_str(),"Hello World");

    EXPECT_STREQ((str +"Good").c_str(),"Hello Good");

    EXPECT_STREQ((str +"G").c_str(),"Hello G");

    //comparators operators
    EXPECT_TRUE(str == str3);
    EXPECT_TRUE(str < str2);
    EXPECT_TRUE(str2 > str);
    EXPECT_TRUE(str2 != str);

    EXPECT_TRUE(str == "Hello ");
    EXPECT_TRUE(str < "World");
    EXPECT_TRUE(str2 > "Hello ");
    EXPECT_TRUE(str2 != "Hello ");

    //swap
    std::swap(str,str2);
    EXPECT_STREQ(str2.c_str(),"Hello ");
    EXPECT_STREQ(str.c_str(),"World");

    //erase
    std::erase(str, 'd');
    EXPECT_STREQ(str.c_str(),"Worl");
    std::erase(str, 'r');
    EXPECT_STREQ(str.c_str(),"Wol");

    //erase_if
    str = "AbCdEfGhiJKLmnop";
    auto removeSmallCaseLetters = [](char alphabet){ return (alphabet >= 97 && alphabet <= 122);};
    std::erase_if(str,removeSmallCaseLetters);
    EXPECT_STREQ(str.c_str(),"ACEGJKL");
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}