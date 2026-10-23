#include "nds_loader.h"
namespace dsfex360 { namespace {
NdsLoadResult failure(LoaderError e){NdsLoadResult r={};r.error=e;return r;}
bool range_fits(std::uint32_t o,std::uint32_t n,std::size_t total){const std::uint64_t end=static_cast<std::uint64_t>(o)+n;return n!=0&&o<total&&end<=total;}
LoadedSection copy_section(const std::vector<std::uint8_t>& rom,std::uint32_t o,std::uint32_t n,std::uint32_t entry,std::uint32_t load){
 LoadedSection s={};s.entry_address=entry;s.load_address=load;s.bytes.assign(rom.begin()+o,rom.begin()+o+n);return s;
}
}
NdsLoadResult load_nds_sections(const std::vector<std::uint8_t>& rom) {
 const HeaderParseResult p=parse_nds_header(rom);if(!p.ok())return failure(LoaderError::InvalidHeader);
 const NdsHeader& h=p.header;
 if(!range_fits(h.arm9_rom_offset,h.arm9_size,rom.size()))return failure(LoaderError::Arm9CopyRangeInvalid);
 if(!range_fits(h.arm7_rom_offset,h.arm7_size,rom.size()))return failure(LoaderError::Arm7CopyRangeInvalid);
 NdsLoadResult r={};r.error=LoaderError::None;r.header=h;
 r.arm9=copy_section(rom,h.arm9_rom_offset,h.arm9_size,h.arm9_entry_address,h.arm9_load_address);
 r.arm7=copy_section(rom,h.arm7_rom_offset,h.arm7_size,h.arm7_entry_address,h.arm7_load_address);return r;
}
LoaderError load_nds_into_memory(const NdsLoadResult& l,AddressSpace& m) {
 if(!l.ok())return LoaderError::InvalidHeader;
 if(!m.can_write(l.arm9.load_address,l.arm9.bytes.size()))return LoaderError::Arm9DestinationInvalid;
 if(!m.can_write(l.arm7.load_address,l.arm7.bytes.size()))return LoaderError::Arm7DestinationInvalid;
 if(!m.write(l.arm9.load_address,l.arm9.bytes.data(),l.arm9.bytes.size()))return LoaderError::MemoryWriteFailed;
 if(!m.write(l.arm7.load_address,l.arm7.bytes.data(),l.arm7.bytes.size()))return LoaderError::MemoryWriteFailed;
 return LoaderError::None;
}
const char* loader_error_message(LoaderError e) {
 switch(e){case LoaderError::None:return "no error";case LoaderError::InvalidHeader:return "invalid Nintendo DS ROM header";case LoaderError::Arm9CopyRangeInvalid:return "ARM9 payload range cannot be copied";case LoaderError::Arm7CopyRangeInvalid:return "ARM7 payload range cannot be copied";case LoaderError::Arm9DestinationInvalid:return "ARM9 destination is not mapped";case LoaderError::Arm7DestinationInvalid:return "ARM7 destination is not mapped";case LoaderError::MemoryWriteFailed:return "failed to write CPU payload to memory";}return "unknown loader error";
}
}
