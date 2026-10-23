#ifndef DSFEX360_NDS_HEADER_H
#define DSFEX360_NDS_HEADER_H
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
namespace dsfex360 {
struct NdsHeader {
 std::string title,game_code,maker_code;
 std::uint8_t unit_code,device_capacity;
 std::uint32_t arm9_rom_offset,arm9_entry_address,arm9_load_address,arm9_size;
 std::uint32_t arm7_rom_offset,arm7_entry_address,arm7_load_address,arm7_size;
};
enum class HeaderError { None,HeaderTooSmall,EmptyTitle,EmptyGameCode,InvalidArm9Range,InvalidArm7Range };
struct HeaderParseResult { HeaderError error; NdsHeader header; bool ok() const { return error==HeaderError::None; } };
HeaderParseResult parse_nds_header(const std::vector<std::uint8_t>& rom);
const char* header_error_message(HeaderError error);
}
#endif
