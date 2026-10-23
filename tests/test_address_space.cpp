#include "../src/memory/address_space.h"
#include <cstdint>
#include <iostream>
int main(){dsfex360::AddressSpace m;int n=0;
if(m.map_region(0x02000000u,0x1000u))++n;else return 1;
if(!m.map_region(0x02000800u,0x1000u))++n;else return 2;
if(!m.map_region(0xfffffff0u,0x40u))++n;else return 3;
const std::uint8_t src[]={0x11,0x22,0x33,0x44};if(m.write(0x02000010u,src,4))++n;else return 4;
std::uint8_t dst[4]={};if(m.read(0x02000010u,dst,4)&&dst[0]==0x11&&dst[3]==0x44)++n;else return 5;
if(!m.read(0x02000fffu,dst,4))++n;else return 6;
if(!m.write(0x03000000u,src,4))++n;else return 7;
if(!m.write(0x02000000u,nullptr,1))++n;else return 8;
if(!m.read(0x02000000u,nullptr,1))++n;else return 9;
if(!m.map_region(0x03000000u,0))++n;else return 10;
std::cout<<"[PASS] AddressSpace tests: "<<n<<"/10\n";return n==10?0:11;}
