#ifndef SEQ_Y86_SIMULATOR_H
#define SEQ_Y86_SIMULATOR_H

#include <vector>
using std::vector;

#define PC_SIZE 0
#define I_MEM_SIZE 400

#define HALT 0 //00000000
#define NOP 16 //00010000
#define RRMOVQ 32 //00100000
#define CMOVLE 33 //00100001
#define CMOVL 34 //00100010
#define CMOVE 35 //00100011
#define CMOVNE 36//00100100
#define CMOVGE 37 //00100101
#define CMOVG 38 //00100110
#define IRMOVQ 48 //00110000
#define RMMOVQ 64 //01000000
#define MRMOVQ 80 //01010000
#define ADDQ 96 //01100000
#define SUBQ 97 //01100001
#define ANDQ 98 //01100010
#define XORQ 99 //01100011
#define JMP 112 //01110000
#define JLE 113 //01110001
#define JL 114 //01110010
#define JE 115 //01110011
#define JNE 116 //01110100
#define JGE 117 //01110101
#define JG 118 //01110110
#define CALL 128 //10000000
#define RET 144 //10010000
#define PUSHQ 160 //10100000
#define POPQ 176 //10110000

class PC{
private:
    unsigned long pc;
public:
    PC():pc(PC_SIZE){}
    unsigned long get_pc(){return pc;}
    void set_pc(unsigned long _pc){pc=_pc;}
};

class Instruction_memory{
private:
    vector<char> i_mem;
    vector<char> target_instruction;
    bool imem_error;
public:
    Instruction_memory():i_mem(I_MEM_SIZE),target_instruction(10),imem_error(false){}
    void set_i_mem(vector<char> user_code){i_mem=user_code;}
    void set_target_instruction(unsigned long pc);
    unsigned char get_instruction_byte();
    vector<char> get_other_byte();
    bool get_imem_error(){return imem_error;}
};

class Stat{
private:
    bool adr;
    bool ins;
    bool hlt;
public:
    Stat():adr(false),ins(true),hlt(false){}
    enum error{AOK=1,ADR,INS,HLT};
    void set_adr(bool _adr){if(_adr==true) adr=true;}
    void set_ins(bool _ins){if(_ins==false) ins=false;}
    void set_hlt(bool _hlt){if(_hlt==true) hlt=true;}
    error get_stat_state();
};

class Split{
private:
    unsigned char icode;
    unsigned char ifun;
    bool instr_valid;
    bool imem_error;
public:
    Split():instr_valid(true),imem_error(false){}
    void set_ic_if(unsigned char instruction_byte);
    void set_imem_error_state(bool _imem_error){imem_error=_imem_error;}
    unsigned char get_icode(){return icode;}
    unsigned char get_ifun(){return ifun;}
    bool get_instr_valid(){return instr_valid;}
};

class PC_Add{
private:
    unsigned long valp;
    bool need_valc;
    bool need_r;
public:
    void set_valp(unsigned long pc);
    unsigned long get_valp(){return valp;}
    void set_need_valc(unsigned char icode);
    void set_need_r(unsigned char icode);
};

class Y86{
private:
    Stat stat;
    Instruction_memory i_mem;
    PC pc;
    Split split;
public:
    void run();
};

#endif // SEQ_Y86_SIMULATOR_H
