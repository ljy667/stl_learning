
#include "my_list.hpp"
#include "vector.hpp"

void TestList(){
    MyList<int> myList;

    for (int i = 0 ; i < 5; i++ ){
        myList.insert(myList.end() ,i);
    }

    myList.show();

    auto it = myList.end();
    --it ;
    myList.erase(it);

    myList.show();


    myList.erase(myList.begin() , --myList.end());
    myList.show() ;

    myList.assign(10, 60);
    myList.show() ;
    
}

void TestVector(){
    cxx::vector<std::string>  myVector  {} ;
    myVector.show() ;

    cxx::vector<std::string> vec2(5 , "0");
    vec2.show();

    cxx::vector<std::string> vec3(std::move(vec2));
    vec2.show();
    vec3.show();

    myVector.swap(vec3);
    vec3.show();
    myVector.show();


    cxx::vector<std::string> vec5(7 , "9");
    cxx::vector<std::string> vec;

    vec = vec5;
    vec.show();

    vec = std::move(vec5);
    vec.show();
    vec5.show();

}

int main(){
    TestList();
    TestVector();
    return  0 ;
    
}