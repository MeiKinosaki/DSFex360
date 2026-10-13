#include "arm9_core.h"
namespace dsfex360 {
Arm9Core::Arm9Core(Arm9State& state):state_(state),instructions_executed_(0),instructions_skipped_(0),last_instruction_(0){}
CoreStepResult Arm9Core::step(const AddressSpace& memory) {
 std::uint32_t pc=0;
 if(!state_.read_register(Arm9State::kPcRegister,&pc))return CoreStepResult::FetchFault;
 if((pc&3u)!=0u)return CoreStepResult::FetchFault;
 std::uint8_t bytes[4]={0,0,0,0};
 if(!memory.read(pc,bytes,4))return CoreStepResult::FetchFault;
 const std::uint32_t ins=static_cast<std::uint32_t>(bytes[0])|
  (static_cast<std::uint32_t>(bytes[1])<<8)|
  (static_cast<std::uint32_t>(bytes[2])<<16)|
  (static_cast<std::uint32_t>(bytes[3])<<24);
 last_instruction_=ins;
 const std::uint32_t condition=ins>>28;
 if(condition!=15u&&!arm_condition_passed(state_,condition)) {
  state_.write_register(Arm9State::kPcRegister,pc+4u);
  ++instructions_skipped_;
  return CoreStepResult::ConditionFailed;
 }
 if((ins&0x0E000000u)==0x0A000000u) {
  const std::int32_t displacement=static_cast<std::int32_t>(ins<<8)>>6;
  if(ins&(1u<<24))state_.write_register(14,pc+4u);
  state_.write_register(Arm9State::kPcRegister,static_cast<std::uint32_t>(pc+8u+displacement)&~3u);
  ++instructions_executed_;
  return CoreStepResult::Executed;
 }
 state_.write_register(Arm9State::kPcRegister,pc+4u);
 const StepResult result=execute_arm_data_processing(state_,ins);
 if(result==StepResult::Executed){++instructions_executed_;return CoreStepResult::Executed;}
 if(result==StepResult::ConditionFailed){++instructions_skipped_;return CoreStepResult::ConditionFailed;}
 state_.write_register(Arm9State::kPcRegister,pc);
 return result==StepResult::Unsupported?CoreStepResult::UnsupportedInstruction:CoreStepResult::MemoryFault;
}
}
