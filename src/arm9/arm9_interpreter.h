#ifndef DSFEX360_ARM9_INTERPRETER_H
#define DSFEX360_ARM9_INTERPRETER_H
#include "arm9_state.h"
#include <cstdint>
namespace dsfex360 {
enum class StepResult { Executed,ConditionFailed,Unsupported,InvalidRegister };
bool arm_condition_passed(const Arm9State& state,std::uint32_t condition);
StepResult execute_arm_data_processing(Arm9State& state,std::uint32_t instruction);
}
#endif
