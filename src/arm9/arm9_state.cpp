#include "arm9_state.h"
namespace dsfex360 {
Arm9State::Arm9State():registers_(),cpsr_(0u){reset();}
void Arm9State::reset(std::uint32_t entry_point){registers_.fill(0u);registers_[kPcRegister]=entry_point;cpsr_=0xD3u;}
bool Arm9State::read_register(std::size_t i,std::uint32_t*v)const{if(i>=kRegisterCount||v==0)return false;*v=registers_[i];return true;}
bool Arm9State::write_register(std::size_t i,std::uint32_t v){if(i>=kRegisterCount)return false;registers_[i]=v;return true;}
std::uint32_t Arm9State::cpsr()const{return cpsr_;} void Arm9State::set_cpsr(std::uint32_t v){cpsr_=v;}
bool Arm9State::flag(CpsrFlag b)const{return(cpsr_&static_cast<std::uint32_t>(b))!=0u;}
void Arm9State::set_flag(CpsrFlag b,bool on){std::uint32_t m=static_cast<std::uint32_t>(b);cpsr_=on?(cpsr_|m):(cpsr_&~m);}
}
