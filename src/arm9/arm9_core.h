#ifndef DSFEX360_ARM9_CORE_H
#define DSFEX360_ARM9_CORE_H
#include "arm9_interpreter.h"
#include "../memory/address_space.h"
#include <cstdint>
namespace dsfex360 {
enum class CoreStepResult { Executed,ConditionFailed,UnsupportedInstruction,FetchFault,MemoryFault };
class Arm9Core {
public:
 explicit Arm9Core(Arm9State& state);
 CoreStepResult step(AddressSpace& memory);
 std::uint64_t instructions_executed() const { return instructions_executed_; }
 std::uint64_t instructions_skipped() const { return instructions_skipped_; }
 std::uint32_t last_instruction() const { return last_instruction_; }
private:
 Arm9State& state_;
 std::uint64_t instructions_executed_;
 std::uint64_t instructions_skipped_;
 std::uint32_t last_instruction_;
};
}
#endif
