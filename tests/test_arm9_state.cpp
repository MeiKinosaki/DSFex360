#include "../src/arm9/arm9_state.h"
#include <cstdlib>
#include <iostream>
using dsfex360::Arm9State;
static int checks=0;
static void expect(bool ok,const char* name){++checks;if(!ok){std::cerr<<"[FAIL] "<<name<<'\n';std::exit(1);}std::cout<<"[PASS] "<<name<<'\n';}
int main(){Arm9State c;std::uint32_t v=0xFFFFFFFFu;
expect(c.read_register(0,&v)&&v==0,"registers reset");expect(c.read_register(15,&v)&&v==0,"PC reset");
expect((c.cpsr()&0x1Fu)==0x13u,"supervisor mode");expect((c.cpsr()&0xC0u)==0xC0u,"IRQ/FIQ masked");
expect(c.write_register(3,0x12345678u),"write register");expect(c.read_register(3,&v)&&v==0x12345678u,"read register");
expect(c.write_register(15,0x02000000u),"write PC");expect(c.read_register(15,&v)&&v==0x02000000u,"read PC");
expect(!c.read_register(16,&v),"reject bad register");expect(!c.read_register(0,nullptr),"reject null read");
expect(!c.write_register(16,1),"reject bad write");c.set_flag(Arm9State::Zero,true);expect(c.flag(Arm9State::Zero),"set Z");
c.set_flag(Arm9State::Negative,true);expect(c.flag(Arm9State::Negative),"set N");c.set_flag(Arm9State::Zero,false);expect(!c.flag(Arm9State::Zero),"clear Z");
c.set_cpsr(0xA000001Fu);expect(c.cpsr()==0xA000001Fu,"read/write CPSR");c.reset(0x02004000u);
expect(c.read_register(15,&v)&&v==0x02004000u,"custom reset PC");expect(!c.flag(Arm9State::Negative)&&!c.flag(Arm9State::Zero),"reset clears flags");
std::cout<<"ARM9 state checks passed: "<<checks<<'\n';return 0;}
