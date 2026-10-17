#ifndef DSFEX360_ARM9_CPU_H
#define DSFEX360_ARM9_CPU_H
#include "arm9_interpreter.h"
#include "../memory/address_space.h"
#include <cstdint>
namespace dsfex360 {
enum class CpuStepResult { Executed,ConditionSkipped,UnsupportedInstruction,FetchFault,InvalidState };
class Arm9Cpu {
public:
 explicit Arm9Cpu(Arm9State& state);
 CpuStepResult step(const AddressSpace& memory);
 std::uint64_t instructions_executed() const { return instructions_executed_; }
 void reset_counter() { instructions_executed_=0; }
private:
 Arm9State& state_;
 std::uint64_t instructions_executed_;
};
}
#endif
