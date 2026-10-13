#include "../src/arm9/arm9_interpreter.h"
#include <cassert>
int main(){
 dsfex360::Arm9State s;assert(s.write_register(0,7));assert(s.write_register(1,5));
 std::uint32_t value=0;bool carry=false;
 assert(dsfex360::execute_arm_data_processing_immediate(0xE3A02005u,&s));assert(s.read_register(2,&value)&&value==5);
 assert(dsfex360::execute_arm_data_processing_immediate(0xE2803003u,&s));assert(s.read_register(3,&value)&&value==10);
 s.set_flag(dsfex360::Arm9State::Zero,false);assert(dsfex360::execute_arm_data_processing_immediate(0x03A04001u,&s));assert(s.read_register(4,&value)&&value==0);
 assert(s.write_register(0,0x80000001u));assert(dsfex360::decode_arm_register_shifted_operand(0xE1A00080u,&s,&value,&carry));assert(value==2&&carry);
 assert(s.write_register(0,7));assert(s.write_register(1,5));
 assert(dsfex360::execute_arm_data_processing_register(0xE0803001u,&s));assert(s.read_register(3,&value)&&value==12);
 assert(dsfex360::execute_arm_data_processing_register(0xE1A04080u,&s));assert(s.read_register(4,&value)&&value==14);
 assert(dsfex360::execute_arm_data_processing_register(0xE0505001u,&s));assert(s.read_register(5,&value)&&value==2);assert(s.flag(dsfex360::Arm9State::Carry));
 std::uint32_t before=0;assert(s.read_register(6,&before));assert(dsfex360::execute_arm_data_processing_register(0x00806001u,&s));assert(s.read_register(6,&value)&&value==before);
 assert(dsfex360::execute_arm_data_processing_register(0xE1500001u,&s));assert(!s.flag(dsfex360::Arm9State::Zero));
 return 0;
}
