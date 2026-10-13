#include "nds_header.h"
#include <cctype>
namespace dsfex360 { namespace {
const std::size_t kHeaderMinimumSize=0x160;
std::uint32_t read_u32_le(const std::vector<std::uint8_t>& d,std::size_t o) {
 return static_cast<std::uint32_t>(d[o])|(static_cast<std::uint32_t>(d[o+1])<<8)|(static_cast<std::uint32_t>(d[o+2])<<16)|(static_cast<std::uint32_t>(d[o+3])<<24);
}
std::string read_fixed_string(const std::vector<std::uint8_t>& d,std::size_t o,std::size_t n) {
 std::string s; s.reserve(n); for(std::size_t i=0;i<n;++i){unsigned char c=d[o+i];if(c==0)break;s.push_back(static_cast<char>(c));}
 while(!s.empty()&&std::isspace(static_cast<unsigned char>(s.back())))s.pop_back(); return s;
}
bool range_fits(std::uint32_t o,std::uint32_t n,std::size_t total) {
 if(n==0)return false; const std::uint64_t end=static_cast<std::uint64_t>(o)+n; return o<total&&end<=total;
}
HeaderParseResult failure(HeaderError e){HeaderParseResult r={};r.error=e;return r;}
}
HeaderParseResult parse_nds_header(const std::vector<std::uint8_t>& rom) {
 if(rom.size()<kHeaderMinimumSize)return failure(HeaderError::HeaderTooSmall);
 HeaderParseResult r={};r.error=HeaderError::None;
 r.header.title=read_fixed_string(rom,0x00,12);r.header.game_code=read_fixed_string(rom,0x0C,4);r.header.maker_code=read_fixed_string(rom,0x10,2);
 r.header.unit_code=rom[0x12];r.header.device_capacity=rom[0x14];
 r.header.arm9_rom_offset=read_u32_le(rom,0x20);r.header.arm9_entry_address=read_u32_le(rom,0x24);r.header.arm9_load_address=read_u32_le(rom,0x28);r.header.arm9_size=read_u32_le(rom,0x2C);
 r.header.arm7_rom_offset=read_u32_le(rom,0x30);r.header.arm7_entry_address=read_u32_le(rom,0x34);r.header.arm7_load_address=read_u32_le(rom,0x38);r.header.arm7_size=read_u32_le(rom,0x3C);
 if(r.header.title.empty())return failure(HeaderError::EmptyTitle);
 if(r.header.game_code.empty())return failure(HeaderError::EmptyGameCode);
 if(!range_fits(r.header.arm9_rom_offset,r.header.arm9_size,rom.size()))return failure(HeaderError::InvalidArm9Range);
 if(!range_fits(r.header.arm7_rom_offset,r.header.arm7_size,rom.size()))return failure(HeaderError::InvalidArm7Range);
 return r;
}
const char* header_error_message(HeaderError e) {
 switch(e){case HeaderError::None:return "no error";case HeaderError::HeaderTooSmall:return "ROM is smaller than the 0x160-byte DS header";case HeaderError::EmptyTitle:return "header title is empty";case HeaderError::EmptyGameCode:return "header game code is empty";case HeaderError::InvalidArm9Range:return "ARM9 ROM range is outside the ROM buffer";case HeaderError::InvalidArm7Range:return "ARM7 ROM range is outside the ROM buffer";}return "unknown header error";
}
}
