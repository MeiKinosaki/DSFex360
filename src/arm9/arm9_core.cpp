#include "arm9_core.h"
namespace dsfex360 {
Arm9Core::Arm9Core(Arm9State& state):state_(state),instructions_executed_(0),instructions_skipped_(0),last_instruction_(0){}
CoreStepResult Arm9Core::step(AddressSpace& memory) {
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
 // Block data transfer: LDM/STM with a non-empty register list (R0-R14).
 if((ins&0x0E000000u)==0x08000000u) {
  const bool pre=(ins&(1u<<24))!=0, up=(ins&(1u<<23))!=0;
  const bool writeback=(ins&(1u<<21))!=0, load=(ins&(1u<<20))!=0;
  const unsigned rn=(ins>>16)&15u;
  const std::uint32_t list=ins&0x7FFFu; // PC in list is intentionally not implemented yet.
  unsigned count=0;for(unsigned r=0;r<15;++r)if(list&(1u<<r))++count;
  if(count==0)return CoreStepResult::UnsupportedInstruction;
  std::uint32_t base=0;if(!state_.read_register(rn,&base))return CoreStepResult::MemoryFault;
  const std::uint32_t delta=static_cast<std::uint32_t>(count*4u);
  const std::uint32_t first=up?(pre?base+4u:base):(pre?base-delta:base-delta+4u);
  std::uint32_t loaded[15]={0};unsigned index=0;
  for(unsigned r=0;r<15;++r)if(list&(1u<<r)){
   const std::uint32_t address=first+index*4u;
   if(load){std::uint8_t b[4]={0};if(!memory.read(address,b,4))return CoreStepResult::MemoryFault;loaded[r]=static_cast<std::uint32_t>(b[0])|(static_cast<std::uint32_t>(b[1])<<8)|(static_cast<std::uint32_t>(b[2])<<16)|(static_cast<std::uint32_t>(b[3])<<24);}
   else {std::uint32_t value=0;if(!state_.read_register(r,&value))return CoreStepResult::MemoryFault;const std::uint8_t b[4]={static_cast<std::uint8_t>(value),static_cast<std::uint8_t>(value>>8),static_cast<std::uint8_t>(value>>16),static_cast<std::uint8_t>(value>>24)};if(!memory.write(address,b,4))return CoreStepResult::MemoryFault;}
   ++index;
  }
  if(load){for(unsigned r=0;r<15;++r)if(list&(1u<<r))state_.write_register(r,loaded[r]);}
  if(writeback)state_.write_register(rn,up?base+delta:base-delta);
  state_.write_register(Arm9State::kPcRegister,pc+4u);
  ++instructions_executed_;
  return CoreStepResult::Executed;
 }
 // Single data transfer: immediate-offset LDR/STR (word or byte), pre/post-index.
 if((ins&0x0C000000u)==0x04000000u && (ins&(1u<<25))==0u) {
  const bool pre=(ins&(1u<<24))!=0, up=(ins&(1u<<23))!=0;
  const bool byte=(ins&(1u<<22))!=0, writeback=(ins&(1u<<21))!=0;
  const bool load=(ins&(1u<<20))!=0;
  const unsigned rn=(ins>>16)&15u, rd=(ins>>12)&15u;
  std::uint32_t base=0;
  if(!state_.read_register(rn,&base))return CoreStepResult::MemoryFault;
  const std::uint32_t offset=ins&0xFFFu;
  const std::uint32_t adjusted=up?base+offset:base-offset;
  const std::uint32_t address=pre?adjusted:base;
  if(!byte && (address&3u)!=0u)return CoreStepResult::MemoryFault;
  if(load) {
   std::uint32_t value=0;
   if(byte) { std::uint8_t b=0;if(!memory.read(address,&b,1))return CoreStepResult::MemoryFault;value=b; }
   else { std::uint8_t b[4]={0,0,0,0};if(!memory.read(address,b,4))return CoreStepResult::MemoryFault;value=static_cast<std::uint32_t>(b[0])|(static_cast<std::uint32_t>(b[1])<<8)|(static_cast<std::uint32_t>(b[2])<<16)|(static_cast<std::uint32_t>(b[3])<<24); }
   if(!state_.write_register(rd,rd==15u?(value&~3u):value))return CoreStepResult::MemoryFault;
  } else {
   std::uint32_t value=0;if(!state_.read_register(rd,&value))return CoreStepResult::MemoryFault;
   if(byte) { const std::uint8_t b=static_cast<std::uint8_t>(value);if(!memory.write(address,&b,1))return CoreStepResult::MemoryFault; }
   else { const std::uint8_t b[4]={static_cast<std::uint8_t>(value),static_cast<std::uint8_t>(value>>8),static_cast<std::uint8_t>(value>>16),static_cast<std::uint8_t>(value>>24)};if(!const_cast<AddressSpace&>(memory).write(address,b,4))return CoreStepResult::MemoryFault; }
  }
  if(!pre||writeback) { if(!state_.write_register(rn,adjusted))return CoreStepResult::MemoryFault; }
  if(!(load&&rd==15u))state_.write_register(Arm9State::kPcRegister,pc+4u);
  ++instructions_executed_;
  return CoreStepResult::Executed;
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
