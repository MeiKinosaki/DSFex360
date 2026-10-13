#include "address_space.h"
#include <algorithm>
#include <limits>
namespace dsfex360 {
namespace {
bool valid_range(std::uint32_t base,std::size_t size) {
 if(size==0||size>std::numeric_limits<std::uint32_t>::max())return false;
 const std::uint64_t end=static_cast<std::uint64_t>(base)+size;
 return end<=static_cast<std::uint64_t>(std::numeric_limits<std::uint32_t>::max())+1u;
}
}
bool AddressSpace::map_region(std::uint32_t base,std::size_t size) {
 if(!valid_range(base,size))return false;
 const std::uint64_t end=static_cast<std::uint64_t>(base)+size;
 for(const Region& r:regions_) {
  const std::uint64_t other=static_cast<std::uint64_t>(r.base)+r.bytes.size();
  if(static_cast<std::uint64_t>(base)<other&&static_cast<std::uint64_t>(r.base)<end)return false;
 }
 Region r={}; r.base=base; r.bytes.resize(size,0); regions_.push_back(r); return true;
}
AddressSpace::Region* AddressSpace::find_region(std::uint32_t a,std::size_t n) {
 if(!valid_range(a,n))return nullptr; const std::uint64_t end=static_cast<std::uint64_t>(a)+n;
 for(Region& r:regions_) { const std::uint64_t re=static_cast<std::uint64_t>(r.base)+r.bytes.size(); if(a>=r.base&&end<=re)return &r; }
 return nullptr;
}
const AddressSpace::Region* AddressSpace::find_region(std::uint32_t a,std::size_t n) const {
 if(!valid_range(a,n))return nullptr; const std::uint64_t end=static_cast<std::uint64_t>(a)+n;
 for(const Region& r:regions_) { const std::uint64_t re=static_cast<std::uint64_t>(r.base)+r.bytes.size(); if(a>=r.base&&end<=re)return &r; }
 return nullptr;
}
bool AddressSpace::can_write(std::uint32_t a,std::size_t n) const { return find_region(a,n)!=nullptr; }
bool AddressSpace::write(std::uint32_t a,const std::uint8_t* data,std::size_t n) {
 if(data==nullptr)return false; Region* r=find_region(a,n); if(!r)return false;
 const std::size_t off=static_cast<std::size_t>(a-r->base); std::copy(data,data+n,r->bytes.begin()+off); return true;
}
bool AddressSpace::read(std::uint32_t a,std::uint8_t* out,std::size_t n) const {
 if(out==nullptr)return false; const Region* r=find_region(a,n); if(!r)return false;
 const std::size_t off=static_cast<std::size_t>(a-r->base); std::copy(r->bytes.begin()+off,r->bytes.begin()+off+n,out); return true;
}
}
