
#include "my_list.hpp"

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

int main(){
    TestList();
    return  0 ;
    
}