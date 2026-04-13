#include <iostream>
#include <pnc_map.h>

void mapTest()
{
    std::cout<<"this is pnc_map test" <<std::endl;
    PNC_Map mymap;
    mymap.mapInfo();
}

int main()
{
    mapTest();
    return 0;
}