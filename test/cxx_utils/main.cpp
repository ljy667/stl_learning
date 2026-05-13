
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
}

int main(){
    TestList();
    TestVector();
    return  0 ;
    
}