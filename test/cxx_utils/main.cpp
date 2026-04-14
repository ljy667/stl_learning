
#include "my_list.hpp"

void TestList(){
    MyList<int> MyList;

    for (int i = 0 ; i < 5; i++ ){
        MyList.insert(MyList.end() ,i);
    }

    MyList.show();
}

int main(){
    TestList();
    return  0 ;
    
}