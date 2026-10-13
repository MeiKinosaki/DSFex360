#include "../src/arm9/arm9_core.h"
#include <cstdlib>
#include <iostream>
using namespace dsfex360;
static void check(bool b,const char* n){if(!b){std::cerr<<"[FAIL] "<<n<<'\n';std::exit(1);}std::cout<<"[PASS] "<<n<<'\n';}
static void put32(AddressSpace& m,std::uint32_t a,std::uint32_t v){std::uint8_t b[4]={static_cast<std::uint8_t>(v),static_cast<std::uint8_t>(v>>8),static_cast<std::uint8_t>(v>>16),static_cast<std::uint8_t>(v>>24)};if(!m.write(a,b,4))std::exit(2);}
static std::uint32_t get(Arm9State& s,unsigned r){std::uint32_t v=0;s.read_register(r,&v);return v;}
int main(){
 AddressSpace mem;check(mem.map_region(0x02000000u,0x100u),"map code RAM");
 put32(mem,0x02000000u,0xE3A00007u);
 put32(mem,0x02000004u,0xE2801002u);
 put32(mem,0x02000008u,0xEA000000u);
 put32(mem,0x0200000Cu,0xE3A02001u);
 put32(mem,0x02000010u,0xE3A02009u);
 Arm9State state;state.reset(0x02000000u);Arm9Core core(state);
 check(core.step(mem)==CoreStepResult::Executed,"fetch and execute MOV");
 check(get(state,0)==7&&get(state,15)==0x02000004u,"MOV result and PC advance");
 check(core.step(mem)==CoreStepResult::Executed,"fetch and execute ADD");
 check(get(state,1)==9&&get(state,15)==0x02000008u,"ADD result and PC advance");
 check(core.step(mem)==CoreStepResult::Executed,"branch executes");
 check(get(state,15)==0x02000010u,"branch target follows ARM PC+8 rule");
 check(core.step(mem)==CoreStepResult::Executed,"execution resumes at branch target");
 check(get(state,2)==9,"branch skipped intervening instruction");
 check(core.instructions_executed()==4,"instruction counter tracks executed instructions");
 state.reset(0x02000003u);check(core.step(mem)==CoreStepResult::FetchFault,"unaligned fetch rejected");
 state.reset(0x03000000u);check(core.step(mem)==CoreStepResult::FetchFault,"unmapped fetch rejected");
 std::cout<<"ARM9 core tests completed\n";return 0;
}
