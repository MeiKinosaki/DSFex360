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
 // LDR/STR word immediate and unsigned byte transfer smoke tests.
 put32(mem,0x02000020u,0xE5810004u); // STR r0,[r1,#4]
 put32(mem,0x02000024u,0xE5913004u); // LDR r3,[r1,#4]
 put32(mem,0x02000028u,0xE5C10008u); // STRB r0,[r1,#8]
 put32(mem,0x0200002Cu,0xE5D14008u); // LDRB r4,[r1,#8]
 state.reset(0x02000020u);state.write_register(0,0x123456ABu);state.write_register(1,0x02000040u);
 check(core.step(mem)==CoreStepResult::Executed,"STR word stores register");
 std::uint8_t stored[4]={};check(mem.read(0x02000044u,stored,4)&&stored[0]==0xAB&&stored[3]==0x12,"STR word uses little endian");
 check(core.step(mem)==CoreStepResult::Executed,"LDR word loads register");check(get(state,3)==0x123456ABu,"LDR word value correct");
 check(core.step(mem)==CoreStepResult::Executed,"STRB stores low byte");check(mem.read(0x02000048u,stored,1)&&stored[0]==0xAB,"STRB stores low byte only");
 check(core.step(mem)==CoreStepResult::Executed,"LDRB loads unsigned byte");check(get(state,4)==0xABu,"LDRB zero extends byte");
 // LDMIA/STMIA: two-register lists, ascending memory addresses.
 put32(mem,0x02000050u,0xE8A10005u); // STMIA r1!,{r1,r2}
 put32(mem,0x02000054u,0xE8B10018u); // LDMIA r1!,{r3,r4}
 state.reset(0x02000050u);state.write_register(1,0x02000060u);state.write_register(2,0xCAFEBABEu);
 check(core.step(mem)==CoreStepResult::Executed,"STMIA executes");
 check(get(state,1)==0x02000068u,"STMIA writeback advances base");
 std::uint8_t pair[8]={};check(mem.read(0x02000060u,pair,8)&&pair[0]==0xAB&&pair[4]==0xBE,"STMIA writes register list in ascending order");
 state.write_register(1,0x02000060u);check(core.step(mem)==CoreStepResult::Executed,"LDMIA executes");
 check(get(state,3)==0x123456ABu&&get(state,4)==0xCAFEBABEu,"LDMIA restores listed registers");
 check(get(state,1)==0x02000068u,"LDMIA writeback advances base");
 // Conditional execution: EQ should skip MOV when Z is clear.
 put32(mem,0x02000070u,0x03A05001u);state.reset(0x02000070u);state.set_flag(Arm9State::Zero,false);
 check(core.step(mem)==CoreStepResult::ConditionFailed,"failed EQ condition skips instruction");
 check(get(state,5)==0u&&get(state,15)==0x02000074u,"conditional skip leaves destination unchanged");
 // Post-index transfer updates base after using its old value.
 put32(mem,0x02000074u,0xE4916004u);state.reset(0x02000074u);state.write_register(1,0x02000040u);
 put32(mem,0x02000040u,0x76543210u);
 check(core.step(mem)==CoreStepResult::Executed,"post-index LDR executes");
 check(get(state,6)==0x76543210u&&get(state,1)==0x02000044u,"post-index LDR loads old base then updates base");
 // Reject an unsupported instruction without silently advancing PC.
 put32(mem,0x02000078u,0xE6000010u);state.reset(0x02000078u);
 check(core.step(mem)==CoreStepResult::UnsupportedInstruction,"unsupported instruction reported");
 check(get(state,15)==0x02000078u,"unsupported instruction preserves PC for diagnosis");
 // Block store preflight: invalid second word must not partially modify first word.
 put32(mem,0x02000080u,0xE8820003u); // STMIA r2,{r0,r1}
 state.reset(0x02000080u);state.write_register(0,0xAAAAAAAAu);state.write_register(1,0xBBBBBBBBu);state.write_register(2,0x020000FCu);
 std::uint8_t before[4]={};check(mem.read(0x020000FCu,before,4),"read final mapped word before failed transfer");
 check(core.step(mem)==CoreStepResult::MemoryFault,"block store rejects unmapped second word");
 std::uint8_t after[4]={};check(mem.read(0x020000FCu,after,4)&&after[0]==before[0]&&after[1]==before[1]&&after[2]==before[2]&&after[3]==before[3],"failed block store leaves memory unchanged");
 std::cout<<"ARM9 core tests completed\n";return 0;
}
