#ifndef DSFEX360_ARM9_STATE_H
#define DSFEX360_ARM9_STATE_H
#include <array>
#include <cstddef>
#include <cstdint>
namespace dsfex360 {
class Arm9State {
public:
 static const std::size_t kRegisterCount=16, kPcRegister=15;
 enum CpsrFlag:std::uint32_t {Negative=1u<<31,Zero=1u<<30,Carry=1u<<29,Overflow=1u<<28};
 Arm9State(); void reset(std::uint32_t entry_point=0u);
 bool read_register(std::size_t index,std::uint32_t* value)const;
 bool write_register(std::size_t index,std::uint32_t value);
 std::uint32_t cpsr()const; void set_cpsr(std::uint32_t value);
 bool flag(CpsrFlag bit)const; void set_flag(CpsrFlag bit,bool enabled);
private: std::array<std::uint32_t,kRegisterCount> registers_; std::uint32_t cpsr_;
};
}
#endif
