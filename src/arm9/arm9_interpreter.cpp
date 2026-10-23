#include "arm9_interpreter.h"
namespace dsfex360 { namespace {
bool flag(const Arm9State& s,Arm9State::CpsrFlag f){return s.flag(f);}
std::uint32_t ror(std::uint32_t x,unsigned n){n&=31u;return n==0?x:(x>>n)|(x<<(32u-n));}
bool operand2(Arm9State& s,std::uint32_t ins,std::uint32_t* value,bool* carry) {
 const bool immediate=(ins&(1u<<25))!=0;
 if(immediate){const unsigned rot=((ins>>8)&15u)*2u;const std::uint32_t imm=ins&255u;*value=ror(imm,rot);*carry=rot?((*value>>31)&1u)!=0:flag(s,Arm9State::Carry);return true;}
 const unsigned rm=ins&15u;std::uint32_t v=0;if(!s.read_register(rm,&v))return false;
 const bool by_reg=(ins&(1u<<4))!=0;unsigned type=(ins>>5)&3u,amount=0;
 if(by_reg){if(ins&(1u<<7))return false;std::uint32_t rs=0;if(!s.read_register((ins>>8)&15u,&rs))return false;amount=rs&255u;}
 else amount=(ins>>7)&31u;
 if(by_reg&&amount==0){*value=v;*carry=flag(s,Arm9State::Carry);return true;}
 if(type==0){if(amount==0){*value=v;*carry=flag(s,Arm9State::Carry);}else if(amount<32){*value=v<<amount;*carry=((v>>(32u-amount))&1u)!=0;}else if(amount==32){*value=0;*carry=(v&1u)!=0;}else{*value=0;*carry=false;}}
 else if(type==1){if(!by_reg&&amount==0)amount=32;if(amount==0){*value=v;*carry=flag(s,Arm9State::Carry);}else if(amount<32){*value=v>>amount;*carry=((v>>(amount-1u))&1u)!=0;}else if(amount==32){*value=0;*carry=((v>>31)&1u)!=0;}else{*value=0;*carry=false;}}
 else if(type==2){if(!by_reg&&amount==0)amount=32;if(amount==0){*value=v;*carry=flag(s,Arm9State::Carry);}else if(amount<32){*value=static_cast<std::uint32_t>(static_cast<std::int32_t>(v)>>amount);*carry=((v>>(amount-1u))&1u)!=0;}else{*value=(v&0x80000000u)?0xFFFFFFFFu:0u;*carry=(v>>31)!=0;}}
 else {if(!by_reg&&amount==0){*value=(flag(s,Arm9State::Carry)?0x80000000u:0u)|(v>>1);*carry=(v&1u)!=0;}else{amount%=32u;if(amount==0){*value=v;*carry=(v>>31)!=0;}else{*value=ror(v,amount);*carry=(*value>>31)!=0;}}}
 return true;
}
void set_nz(Arm9State& s,std::uint32_t v){s.set_flag(Arm9State::Negative,(v>>31)!=0);s.set_flag(Arm9State::Zero,v==0);}
void set_add_flags(Arm9State& s,std::uint32_t a,std::uint32_t b,std::uint32_t result){
 const std::uint64_t wide=static_cast<std::uint64_t>(a)+b+0u;
 s.set_flag(Arm9State::Carry,wide>0xFFFFFFFFull);
 s.set_flag(Arm9State::Overflow,((~(a^b)&(a^result))>>31)!=0);
}
void set_sub_flags(Arm9State& s,std::uint32_t a,std::uint32_t b,std::uint32_t result){
 s.set_flag(Arm9State::Carry,a>=b);
 s.set_flag(Arm9State::Overflow,(((a^b)&(a^result))>>31)!=0);
}
}
bool arm_condition_passed(const Arm9State& s,std::uint32_t c){
 const bool n=flag(s,Arm9State::Negative),z=flag(s,Arm9State::Zero),cy=flag(s,Arm9State::Carry),v=flag(s,Arm9State::Overflow);
 switch(c&15u){case 0:return z;case 1:return !z;case 2:return cy;case 3:return !cy;case 4:return n;case 5:return !n;case 6:return v;case 7:return !v;case 8:return cy&&!z;case 9:return !cy||z;case 10:return n==v;case 11:return n!=v;case 12:return !z&&(n==v);case 13:return z||(n!=v);case 14:return true;default:return false;}
}
StepResult execute_arm_data_processing(Arm9State& s,std::uint32_t ins){
 if((ins>>28)!=15u&&!arm_condition_passed(s,ins>>28))return StepResult::ConditionFailed;
 if((ins&0x0C000000u)!=0)return StepResult::Unsupported;
 const unsigned opcode=(ins>>21)&15u;const bool set_flags=(ins&(1u<<20))!=0;
 const unsigned rn=(ins>>16)&15u,rd=(ins>>12)&15u;
 std::uint32_t a=0,b=0,oldpc=0;bool shcarry=false;
 if(opcode!=13u&&opcode!=15u&&!s.read_register(rn,&a))return StepResult::InvalidRegister;
 if(!operand2(s,ins,&b,&shcarry))return StepResult::Unsupported;
 std::uint32_t result=0;bool writes=true,logical=false,arithmetic=false;
 switch(opcode){
 case 0:result=a&b;logical=true;break; // AND
 case 1:result=a^b;logical=true;break; // EOR
 case 2:result=a-b;arithmetic=true;break; // SUB
 case 3:result=b-a;arithmetic=true;break; // RSB
 case 4:result=a+b;arithmetic=true;break; // ADD
 case 8:result=a&b;logical=true;writes=false;break; // TST
 case 9:result=a^b;logical=true;writes=false;break; // TEQ
 case 10:result=a-b;arithmetic=true;writes=false;break; // CMP
 case 12:result=a|b;logical=true;break; // ORR
 case 13:result=b;logical=true;break; // MOV
 default:return StepResult::Unsupported;
 }
 if(writes&&!s.write_register(rd,result))return StepResult::InvalidRegister;
 if(set_flags||!writes){set_nz(s,result);if(arithmetic){if(opcode==2||opcode==10)set_sub_flags(s,a,b,result);else if(opcode==3)set_sub_flags(s,b,a,result);else set_add_flags(s,a,b,result);}else s.set_flag(Arm9State::Carry,shcarry);}
 if(writes&&rd==15u){if(s.read_register(15,&oldpc))s.write_register(15,oldpc&~3u);}
 return StepResult::Executed;
}
}
