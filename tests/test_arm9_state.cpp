#include "../src/arm9/arm9_state.h"
#include <cassert>
int main(){dsfex360::Arm9State s;std::uint32_t v=99;assert(s.read_register(15,&v)&&v==0);s.reset(0x02000000);assert(s.read_register(15,&v)&&v==0x02000000);assert(!s.read_register(16,&v));s.set_flag(dsfex360::Arm9State::Zero,true);assert(s.flag(dsfex360::Arm9State::Zero));s.set_flag(dsfex360::Arm9State::Zero,false);assert(!s.flag(dsfex360::Arm9State::Zero));return 0;}
