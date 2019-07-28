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
	void set_pc(unsigned char icode, bool cnd, unsigned long valC, unsigned long valP, unsigned long valM);
};

class Instruction_memory{
private:
    vector<unsigned char> i_mem;
    vector<unsigned char> target_instruction;
    bool imem_error;
public:
    Instruction_memory():i_mem(I_MEM_SIZE),target_instruction(10),imem_error(false){}
    void set_i_mem(vector<unsigned char> user_code){i_mem=user_code;}
    void set_target_instruction(unsigned long pc);
    unsigned char get_instruction_byte();
    vector<unsigned char> get_other_byte();
    bool get_imem_error(){return imem_error;}
};


class Stat{
private:
	enum error { AOK = 1, ADR, INS, HLT };
	error result;
public:
	Stat() :result(AOK){}
	void set_stat_state(bool _i_adr, bool _d_adr, bool _ins, unsigned char icode);
	error get_stat_state() { return result; }
};

class Split{
private:
    unsigned char icode;
    unsigned char ifun;
    bool instr_valid;
public:
    Split():instr_valid(true){}
	void set_ic_if(bool imem_error, unsigned char instruction_byte);
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
    void set_valp(unsigned long pc, unsigned char icode);
    unsigned long get_valp(){return valp;}
};

class Align{
private:
    unsigned char rA;
    unsigned char rB;
    union{
        unsigned long valc;
        char c_valc[8];
    };
public:
	void set_r_valc(vector<unsigned char> other_byte, unsigned char icode);
    unsigned char get_rA(){return rA;}
    unsigned char get_rB(){return rB;}
    unsigned long get_valc(){return valc;}
};

class Register_memory{
private:
    unsigned long r_mem[15];
	unsigned long valA;
	unsigned long valB;
	unsigned char dstE;
	unsigned char dstM;
public:
    Register_memory(){for(int i=0;i<15;i++) r_mem[i]=0;}
	void set_value(unsigned char icode, unsigned char rA, unsigned rB, bool cnd);
	void set_valE_to_rm(unsigned long valE);
	void set_valM_to_rm(unsigned long valM);
	unsigned long get_valA() { return valA; }
	unsigned long get_valB() { return valB; }
	unsigned long get_rax() { return r_mem[0]; }
};
class CC{
    bool ZF;
    bool SF;
    bool OF;
public:
    CC():ZF(false),SF(false),OF(false){}
    void set_sign(bool _ZF,bool _SF,bool _OF, unsigned char icode){
		if (icode == static_cast<unsigned char>(6)){
            ZF=_ZF;
            SF=_SF;
            OF=_OF;
        }
    }
    bool get_ZF(){return ZF;}
    bool get_SF(){return SF;}
    bool get_OF(){return OF;}
};

class ALU{
private:
	unsigned long valE;
    bool ZF;
    bool SF;
    bool OF;
public:
    ALU():ZF(false),SF(false),OF(false){}

	void set_valE_and_cnd(unsigned long valA, unsigned long valB, unsigned long valC, unsigned char icode, unsigned char ifun);

    bool get_ZF(){return ZF;}
    bool get_SF(){return SF;}
    bool get_OF(){return OF;}
	unsigned long get_valE() { return valE; }
};
class Cond{
private:
    bool ZF;
    bool SF;
    bool OF;
	bool cnd;
public:
	Cond() :ZF(false), SF(false), OF(false), cnd(false){}
	void set_cnd(bool ZF, bool SF, bool OF, unsigned char ifun);
	bool get_cnd() { return cnd; }
};

class data_memory {
private:
	vector<unsigned char> d_mem;
	union {
		unsigned long data;
		unsigned char c_data[8];
	};
	union {
		unsigned long valM;
		unsigned char c_valM[8];
	};
	bool dmem_error;
public:
	data_memory() :d_mem(200),dmem_error(false){}
	bool get_dmem_error() { return dmem_error; }
	void set_valM_and_error(unsigned char icode, unsigned long valA, unsigned long valE, unsigned long valP);
	unsigned long get_valM() { return valM; }
};

class Y86{
private:
    Stat stat;
    Instruction_memory i_mem;
    PC pc;
    Split split;
    PC_Add pc_add;
    Align align;
    Register_memory r_mem;
    ALU alu;
    CC cc;
    Cond cond;
	data_memory d_mem;
public:
    void set_i_mem(vector<unsigned char> user_code){i_mem.set_i_mem(user_code);}
    void run();
	unsigned long get_result() { return r_mem.get_rax(); }
};

#endif // SEQ_Y86_SIMULATOR_H

