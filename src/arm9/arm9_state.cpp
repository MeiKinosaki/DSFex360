#include "arm9_state.h"
namespace dsfex360 {
Arm9State::Arm9State() : registers_(), cpsr_(0u) { reset(); }
void Arm9State::reset(std::uint32_t entry_point) {
    registers_.fill(0u); registers_[kPcRegister]=entry_point;
    // Scaffold default only; the real DS boot sequence must establish state.
    cpsr_=0x000000D3u;
}
bool Arm9State::read_register(std::size_t index,std::uint32_t* value) const {
    if(index>=kRegisterCount||value==0)return false; *value=registers_[index]; return true;
}
bool Arm9State::write_register(std::size_t index,std::uint32_t value) {
    if(index>=kRegisterCount)return false; registers_[index]=value; return true;
}
std::uint32_t Arm9State::cpsr() const { return cpsr_; }
void Arm9State::set_cpsr(std::uint32_t value) { cpsr_=value; }
bool Arm9State::flag(CpsrFlag bit) const { return (cpsr_&static_cast<std::uint32_t>(bit))!=0u; }
void Arm9State::set_flag(CpsrFlag bit,bool enabled) {
    const std::uint32_t mask=static_cast<std::uint32_t>(bit);
    cpsr_=enabled?(cpsr_|mask):(cpsr_&~mask);
}
}
