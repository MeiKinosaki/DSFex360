#include "../src/memory/address_space.h"
#include <cassert>
int main(){dsfex360::AddressSpace m;assert(m.map_region(0x02000000,64));assert(!m.map_region(0x02000020,64));std::uint8_t in[3]={1,2,3},out[3]={0,0,0};assert(m.write(0x02000004,in,3));assert(m.read(0x02000004,out,3));assert(out[0]==1&&out[1]==2&&out[2]==3);assert(!m.read(0x03000000,out,3));return 0;}
