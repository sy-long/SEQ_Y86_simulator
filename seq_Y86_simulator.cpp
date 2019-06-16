#include "seq_Y86_simulator.h"

Stat::error Stat::get_stat_state(){
    if(adr) return ADR;
    else if(!ins) return INS;
    else if(hlt) return HLT;
    else return AOK;
}

void Instruction_memory::set_target_instruction(unsigned long pc){
    if(pc+10>=i_mem.size()){
        imem_error=true;
        target_instruction[0]=NOP;
    }
    else{
        vector<char> temp{i_mem.begin()+pc,i_mem.begin()+pc+10};
        target_instruction=temp;
    }
}
unsigned char Instruction_memory::get_instruction_byte(){
    return static_cast<unsigned char>(target_instruction[0]);
}
vector<char> Instruction_memory::get_other_byte(){
    return {target_instruction.begin()+1,target_instruction.begin()+10};
}

void Split::set_ic_if(unsigned char instruction_byte){
    if(imem_error){icode=1;ifun=0;instr_valid=true;}
    switch (instruction_byte) {
    case HALT:{icode=0;ifun=0;}break;
    case NOP:{icode=1;ifun=0;}break;
    case RRMOVQ:{icode=2;ifun=0;}break;
    case CMOVLE:{icode=2;ifun=1;}break;
    case CMOVL:{icode=2;ifun=2;}break;
    case CMOVE:{icode=2;ifun=3;}break;
    case CMOVNE:{icode=2;ifun=4;}break;
    case CMOVGE:{icode=2;ifun=5;}break;
    case CMOVG:{icode=2;ifun=6;}break;
    case IRMOVQ:{icode=3;ifun=0;}break;
    case RMMOVQ:{icode=4;ifun=0;}break;
    case MRMOVQ:{icode=5;ifun=0;}break;
    case ADDQ:{icode=6;ifun=0;}break;
    case SUBQ:{icode=6;ifun=1;}break;
    case ANDQ:{icode=6;ifun=2;}break;
    case XORQ:{icode=6;ifun=3;}break;
    case JMP:{icode=7;ifun=0;}break;
    case JLE:{icode=7;ifun=1;}break;
    case JL:{icode=7;ifun=2;}break;
    case JE:{icode=7;ifun=3;}break;
    case JNE:{icode=7;ifun=4;}break;
    case JGE:{icode=7;ifun=5;}break;
    case JG:{icode=7;ifun=6;}break;
    case CALL:{icode=8;ifun=0;}break;
    case RET:{icode=9;ifun=0;}break;
    case PUSHQ:{icode=10;ifun=0;}break;
    case POPQ:{icode=11;ifun=0;}break;
    default:{icode=1;ifun=0;instr_valid=false;}break;
    }
}

void PC_Add::set_need_valc(unsigned char icode){
    if(icode==IRMOVQ||icode==RMMOVQ||icode==MRMOVQ||icode==CALL||
       icode==JMP||icode==JLE||icode==JL||icode==JE||icode==JNE||
       icode==JGE||icode==JG)
       need_valc=true;
    else need_valc=false;
}
void PC_Add::set_need_r(unsigned char icode){
    if(icode==HALT||icode==NOP||icode==JMP||icode==JLE||icode==JL||
       icode==JE||icode==JNE||icode==JGE||icode==JG||icode==CALL||icode==RET)
        need_r=false;
    else need_r=true;
}
void PC_Add::set_valp(unsigned long pc){
    valp=pc+1;
    if(need_r) valp++;
    if(need_valc) valp+=8;
}

void Y86::run(){
    i_mem.set_target_instruction(pc.get_pc());
    stat.set_adr(i_mem.get_imem_error());
    split.set_imem_error_state(i_mem.get_imem_error());
    split.set_ic_if(i_mem.get_instruction_byte());
    stat.set_ins(split.get_instr_valid());
}
