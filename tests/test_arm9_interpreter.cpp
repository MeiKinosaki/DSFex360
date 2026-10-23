#include "../src/arm9/arm9_interpreter.h"
#include <cstdlib>
#include <iostream>
using namespace dsfex360;
static void check(bool b,const char* n){if(!b){std::cerr<<"[FAIL] "<<n<<'\n';std::exit(1);}std::cout<<"[PASS] "<<n<<'\n';}
static std::uint32_t dp(unsigned cond,unsigned opcode,bool s,unsigned rn,unsigned rd,std::uint32_t op2,bool imm=true){return (cond<<28)|(imm?1u<<25:0)|(opcode<<21)|(s?1u<<20:0)|(rn<<16)|(rd<<12)|op2;}
int main(){Arm9State c;c.reset();c.write_register(0,5);c.write_register(1,7);
check(execute_arm_data_processing(c,dp(14,4,false,0,2,1))==StepResult::Executed,"ADD immediate executes");
std::uint32_t v=0; c.read_register(2,&v);check(v==6,"ADD result correct");
check(execute_arm_data_processing(c,dp(14,2,true,1,3,1))==StepResult::Executed,"SUBS executes");
c.read_register(3,&v);check(v==6,"SUB result correct");check(!c.flag(Arm9State::Zero),"SUBS updates Z");
check(execute_arm_data_processing(c,dp(0,13,false,0,4,0))==StepResult::ConditionFailed,"EQ condition skips when Z clear");
c.set_flag(Arm9State::Zero,true);check(execute_arm_data_processing(c,dp(0,13,false,0,4,9))==StepResult::Executed,"EQ condition executes when Z set");
c.read_register(4,&v);check(v==9,"MOV immediate result correct");
c.write_register(0,5);check(execute_arm_data_processing(c,dp(14,10,true,0,0,5))==StepResult::Executed,"CMP executes");
check(c.flag(Arm9State::Zero),"CMP sets zero on equality");
c.write_register(5,3);const std::uint32_t add_shift=(14u<<28)|(1u<<21)|(5u<<16)|(6u<<12)|(1u<<25)|0u;
check(execute_arm_data_processing(c,dp(14,0,false,5,6,(1u<<7)|(0u<<5)|5u,false))==StepResult::Executed,"AND register with LSL operand executes");
c.read_register(6,&v);check(v==2,"AND shifted result correct");
c.write_register(0,2);c.write_register(1,9);
check(execute_arm_data_processing(c,dp(14,3,true,0,7,5))==StepResult::Executed,"RSBS executes");
c.read_register(7,&v);check(v==3u,"RSB computes operand2 minus operand1");
check(c.flag(Arm9State::Carry),"RSB sets no-borrow carry correctly");
c.write_register(0,0x0Fu);c.write_register(1,0x03u);
check(execute_arm_data_processing(c,dp(14,8,true,0,0,0x10))==StepResult::Executed,"TST executes");
check(c.flag(Arm9State::Zero),"TST updates flags without writing result");
check(execute_arm_data_processing(c,dp(14,9,true,0,0,1))==StepResult::Executed,"TEQ executes");
check(!c.flag(Arm9State::Zero),"TEQ updates flags from XOR result");
c.write_register(0,0xFFu);
check(execute_arm_data_processing(c,dp(14,14,false,0,8,0x0Fu))==StepResult::Executed,"BIC executes");
c.read_register(8,&v);check(v==0xF0u,"BIC clears selected bits");
check(execute_arm_data_processing(c,dp(14,15,false,0,9,0x0Fu))==StepResult::Executed,"MVN executes");
c.read_register(9,&v);check(v==0xFFFFFFF0u,"MVN bitwise complements operand");
c.write_register(0,0xFFFFFFFFu);c.set_flag(Arm9State::Carry,true);
check(execute_arm_data_processing(c,dp(14,5,true,0,10,0))==StepResult::Executed,"ADCS executes");
c.read_register(10,&v);check(v==0u&&c.flag(Arm9State::Carry)&&c.flag(Arm9State::Zero),"ADC includes carry-in and updates flags");
c.write_register(0,0x7FFFFFFFu);
check(execute_arm_data_processing(c,dp(14,11,true,0,0,1))==StepResult::Executed,"CMN executes");
check(c.flag(Arm9State::Negative)&&c.flag(Arm9State::Overflow),"CMN updates negative and overflow flags without storing result");
c.write_register(0,10);c.write_register(1,3);c.set_flag(Arm9State::Carry,true);
check(execute_arm_data_processing(c,dp(14,6,true,0,11,1))==StepResult::Executed,"SBCS executes");
c.read_register(11,&v);check(v==7u&&c.flag(Arm9State::Carry),"SBC subtracts with carry");
c.set_flag(Arm9State::Carry,false);
check(execute_arm_data_processing(c,dp(14,7,true,0,12,1))==StepResult::Executed,"RSCS executes");
c.read_register(12,&v);check(v==0xFFFFFFF7u,"RSC subtracts operand1 and borrow from operand2");
check(execute_arm_data_processing(c,0xE0E00000u)==StepResult::Unsupported,"unsupported opcode rejected");
check(!arm_condition_passed(c,15),"NV condition rejected");
std::cout<<"ARM9 interpreter smoke checks completed\n";return 0;}
