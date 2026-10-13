#include "../src/cartridge/nds_header.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
namespace {
void put32(std::vector<std::uint8_t>& d,std::size_t o,std::uint32_t v){for(unsigned i=0;i<4;++i)d[o+i]=static_cast<std::uint8_t>(v>>(8*i));}
void put(std::vector<std::uint8_t>& d,std::size_t o,const std::string& s){for(std::size_t i=0;i<s.size();++i)d[o+i]=static_cast<std::uint8_t>(s[i]);}
std::vector<std::uint8_t> valid(){std::vector<std::uint8_t>d(0x200,0);put(d,0,"DSFEX TEST");put(d,0x0C,"TSTP");put(d,0x10,"01");put32(d,0x20,0x160);put32(d,0x24,0x02000000);put32(d,0x28,0x02000000);put32(d,0x2C,0x20);put32(d,0x30,0x180);put32(d,0x34,0x03800000);put32(d,0x38,0x03800000);put32(d,0x3C,0x10);return d;}
void check(bool b,const char* s){if(!b){std::cerr<<"[FAIL] "<<s<<'\n';std::exit(1);}std::cout<<"[PASS] "<<s<<'\n';}
}
int main(){auto r=valid();auto p=dsfex360::parse_nds_header(r);check(p.ok(),"valid synthetic header");check(p.header.title=="DSFEX TEST","title parsed");check(p.header.game_code=="TSTP","game code parsed");check(p.header.arm9_rom_offset==0x160&&p.header.arm9_size==0x20,"ARM9 fields parsed");check(p.header.arm7_rom_offset==0x180&&p.header.arm7_size==0x10,"ARM7 fields parsed");
std::vector<std::uint8_t> shortrom(0x100,0);check(dsfex360::parse_nds_header(shortrom).error==dsfex360::HeaderError::HeaderTooSmall,"short ROM rejected");
r=valid();put32(r,0x2C,0xFFFF);check(dsfex360::parse_nds_header(r).error==dsfex360::HeaderError::InvalidArm9Range,"bad ARM9 range rejected");
r=valid();put32(r,0x3C,0xFFFF);check(dsfex360::parse_nds_header(r).error==dsfex360::HeaderError::InvalidArm7Range,"bad ARM7 range rejected");
r=valid();r[0x0C]=0;check(dsfex360::parse_nds_header(r).error==dsfex360::HeaderError::EmptyGameCode,"empty game code rejected");}
