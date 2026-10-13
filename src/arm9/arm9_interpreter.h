#ifndef DSFEX360_ARM9_INTERPRETER_H
#define DSFEX360_ARM9_INTERPRETER_H
#include <cstdint>
#include "arm9_state.h"
namespace dsfex360 {
bool arm_condition_passed(unsigned condition,const Arm9State* state);
bool decode_arm_register_shifted_operand(std::uint32_t instruction,const Arm9State* state,std::uint32_t* value,bool* carry_out);
bool execute_arm_data_processing_immediate(std::uint32_t instruction,Arm9State* state);
}
#endif
