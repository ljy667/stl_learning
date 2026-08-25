
#include  "dataSource.h"

int main()
{
    auto a = reaction::var(1) ;
    auto b = reaction::var(3.14) ;
    auto ddds = reaction::calc([](int aa , double bb){return aa + bb ;} , a ,b ,ddds)


}
