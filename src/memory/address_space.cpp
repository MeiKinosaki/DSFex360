#include "address_space.h"
#include <algorithm>
#include <limits>
namespace dsfex360 {namespace {bool valid(std::uint32_t b,std::size_t s){if(!s||s>std::numeric_limits<std::uint32_t>::max())return false;return std::uint64_t(b)+s<=std::uint64_t(std::numeric_limits<std::uint32_t>::max())+1u;}}
bool AddressSpace::map_region(std::uint32_t b,std::size_t s){if(!valid(b,s))return false;std::uint64_t e=std::uint64_t(b)+s;for(const auto&r:regions_){std::uint64_t re=std::uint64_t(r.base)+r.bytes.size();if(std::uint64_t(b)<re&&std::uint64_t(r.base)<e)return false;}Region r={};r.base=b;r.bytes.resize(s,0);regions_.push_back(r);return true;}
AddressSpace::Region*AddressSpace::find_region(std::uint32_t a,std::size_t s){if(!valid(a,s))return nullptr;std::uint64_t e=std::uint64_t(a)+s;for(auto&r:regions_)if(a>=r.base&&e<=std::uint64_t(r.base)+r.bytes.size())return &r;return nullptr;}
const AddressSpace::Region*AddressSpace::find_region(std::uint32_t a,std::size_t s)const{if(!valid(a,s))return nullptr;std::uint64_t e=std::uint64_t(a)+s;for(const auto&r:regions_)if(a>=r.base&&e<=std::uint64_t(r.base)+r.bytes.size())return &r;return nullptr;}
bool AddressSpace::can_write(std::uint32_t a,std::size_t s)const{return find_region(a,s)!=nullptr;}
bool AddressSpace::write(std::uint32_t a,const std::uint8_t*d,std::size_t s){if(!d)return false;auto*r=find_region(a,s);if(!r)return false;std::copy(d,d+s,r->bytes.begin()+(a-r->base));return true;}
bool AddressSpace::read(std::uint32_t a,std::uint8_t*d,std::size_t s)const{if(!d)return false;auto*r=find_region(a,s);if(!r)return false;std::copy(r->bytes.begin()+(a-r->base),r->bytes.begin()+(a-r->base)+s,d);return true;}}
