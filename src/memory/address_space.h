#ifndef DSFEX360_ADDRESS_SPACE_H
#define DSFEX360_ADDRESS_SPACE_H
#include <cstddef>
#include <cstdint>
#include <vector>
namespace dsfex360 {class AddressSpace {public:bool map_region(std::uint32_t base,std::size_t size);bool can_write(std::uint32_t address,std::size_t size)const;bool write(std::uint32_t address,const std::uint8_t*data,std::size_t size);bool read(std::uint32_t address,std::uint8_t*out,std::size_t size)const;std::size_t region_count()const{return regions_.size();}private:struct Region{std::uint32_t base;std::vector<std::uint8_t>bytes;};Region* find_region(std::uint32_t,std::size_t);const Region* find_region(std::uint32_t,std::size_t)const;std::vector<Region>regions_;};}
#endif
