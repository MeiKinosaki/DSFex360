#ifndef DSFEX360_NDS_LOADER_H
#define DSFEX360_NDS_LOADER_H
#include "nds_header.h"
#include "../memory/address_space.h"
#include <cstdint>
#include <vector>
namespace dsfex360 {
enum class LoaderError{None,InvalidHeader,Arm9CopyRangeInvalid,Arm7CopyRangeInvalid,Arm9DestinationInvalid,Arm7DestinationInvalid,MemoryWriteFailed};
struct LoadedSection{std::uint32_t entry_address,load_address;std::vector<std::uint8_t>bytes;};
struct NdsLoadResult{LoaderError error;NdsHeader header;LoadedSection arm9,arm7;bool ok()const{return error==LoaderError::None;}};
NdsLoadResult load_nds_sections(const std::vector<std::uint8_t>&rom);
LoaderError load_nds_into_memory(const NdsLoadResult&loaded,AddressSpace&memory);
const char* loader_error_message(LoaderError error);
}
#endif
