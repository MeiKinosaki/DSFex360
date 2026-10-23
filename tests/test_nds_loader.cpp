#include "../src/cartridge/nds_loader.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
namespace {
void put32(std::vector<std::uint8_t>& d,std::size_t o,std::uint32_t v){for(unsigned i=0;i<4;++i)d[o+i]=static_cast<std::uint8_t>(v>>(i*8));}
void put(std::vector<std::uint8_t>& d,std::size_t o,const std::string& s){for(std::size_t i=0;i<s.size();++i)d[o+i]=static_cast<std::uint8_t>(s[i]);}
std::vector<std::uint8_t> rom(){std::vector<std::uint8_t>d(0x200,0);put(d,0,"DSFEX TEST");put(d,0x0c,"TSTP");put(d,0x10,"01");put32(d,0x20,0x160);put32(d,0x24,0x02000000);put32(d,0x28,0x02000000);put32(d,0x2c,4);put32(d,0x30,0x180);put32(d,0x34,0x03800000);put32(d,0x38,0x03800000);put32(d,0x3c,4);d[0x160]=0x11;d[0x161]=0x22;d[0x162]=0x33;d[0x163]=0x44;d[0x180]=0xAA;d[0x181]=0xBB;d[0x182]=0xCC;d[0x183]=0xDD;return d;}
void check(bool b,const char* s){if(!b){std::cerr<<"[FAIL] "<<s<<'\n';std::exit(1);}std::cout<<"[PASS] "<<s<<'\n';}
}
int main(){const auto data=rom();const auto loaded=dsfex360::load_nds_sections(data);check(loaded.ok(),"valid ROM sections extracted");check(loaded.arm9.bytes.size()==4&&loaded.arm7.bytes.size()==4,"section lengths retained");dsfex360::AddressSpace memory;check(memory.map_region(0x02000000,0x1000),"map ARM9 RAM");check(memory.map_region(0x03800000,0x1000),"map ARM7 RAM");check(dsfex360::load_nds_into_memory(loaded,memory)==dsfex360::LoaderError::None,"sections copied into mapped memory");std::uint8_t a[4]={},b[4]={};check(memory.read(0x02000000,a,4)&&a[0]==0x11&&a[3]==0x44,"ARM9 payload matches ROM");check(memory.read(0x03800000,b,4)&&b[0]==0xAA&&b[3]==0xDD,"ARM7 payload matches ROM");dsfex360::AddressSpace unmapped;check(dsfex360::load_nds_into_memory(loaded,unmapped)==dsfex360::LoaderError::Arm9DestinationInvalid,"unmapped destination rejected");auto invalid=data;put32(invalid,0x2c,0xFFFFFFFFu);check(!dsfex360::load_nds_sections(invalid).ok(),"invalid ARM9 range rejected");}
