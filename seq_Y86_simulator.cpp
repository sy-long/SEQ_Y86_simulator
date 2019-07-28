#include "seq_Y86_simulator.h"

void Stat::set_stat_state(bool _i_adr, bool _d_adr, bool _ins, unsigned char icode){
	if (_i_adr || _d_adr) result = ADR;
    else if(!_ins) result = INS;
    else if(icode==0) result = HLT;
    else result = AOK;
}

void Instruction_memory::set_target_instruction(unsigned long pc){
    if(pc+9>i_mem.size()){
        imem_error=true;
        target_instruction[0]=NOP;
    }
    else{
		vector<unsigned char> temp{i_mem.begin()+pc,i_mem.begin()+pc+10};
        target_instruction=temp;
    }
}
unsigned char Instruction_memory::get_instruction_byte(){
    return static_cast<unsigned char>(target_instruction[0]);
}
vector<unsigned char> Instruction_memory::get_other_byte(){
    return {target_instruction.begin()+1,target_instruction.begin()+10};
}

void Split::set_ic_if(bool imem_error, unsigned char instruction_byte){
    if(imem_error){icode=1;ifun=0;instr_valid=false;}
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

void PC_Add::set_valp(unsigned long pc, unsigned char icode){
	bool need_valc;
	if (icode == static_cast<unsigned char>(3) || icode == static_cast<unsigned char>(4) || icode == static_cast<unsigned char>(5) || icode == static_cast<unsigned char>(7) || icode == static_cast<unsigned char>(8))
		need_valc = true;
	else need_valc = false;
	bool need_r;
	if (icode == static_cast<unsigned char>(0) || icode == static_cast<unsigned char>(1) || icode == static_cast<unsigned char>(7) || icode == static_cast<unsigned char>(8) || icode == static_cast<unsigned char>(9))
		need_r = false;
	else need_r = true;
	valp=pc+1;
    if(need_r) valp++;
    if(need_valc) valp+=8;
}

void PC::set_pc(unsigned char icode, bool cnd, unsigned long valC, unsigned long valP, unsigned long valM) {
	if (icode == 8) pc = valC;
	else if (icode == 7 && cnd) pc = valC;
	else if (icode == 9) pc = valM;
	else pc = valP;
}

void Align::set_r_valc(vector<unsigned char> other_byte, unsigned char icode){
    unsigned char temp_r=static_cast<unsigned char>(other_byte[0]);
    rA=temp_r>>4;
    rB=temp_r&static_cast<unsigned char>(15);
	bool need_r;
	if (icode == static_cast<unsigned char>(0) || icode == static_cast<unsigned char>(1) || icode == static_cast<unsigned char>(7) || icode == static_cast<unsigned char>(8) || icode == static_cast<unsigned char>(9))
		need_r = false;
	else need_r = true;
	if(need_r){
        for(int i=0;i<8;i++)
            c_valc[i]=other_byte[i+1];
    }
    else{
        for(int i=0;i<8;i++)
            c_valc[i]=other_byte[i];
    }
}

void Register_memory::set_value(unsigned char icode, unsigned char rA, unsigned rB, bool cnd) {
	if (icode == static_cast<unsigned char>(2) || icode == static_cast<unsigned char>(4) || icode == static_cast<unsigned char>(6) ||
		icode == static_cast<unsigned char>(10))
		valA = r_mem[rA];
	else if (icode == static_cast<unsigned char>(9) || icode == static_cast<unsigned char>(11))
		valA = r_mem[4];
	else valA = 0;
	if (icode == static_cast<unsigned char>(6) || icode == static_cast<unsigned char>(4) || icode == static_cast<unsigned char>(5))
		valB = r_mem[rB];
	else if (icode == static_cast<unsigned char>(10) || icode == static_cast<unsigned char>(11) || icode == static_cast<unsigned char>(8) || icode == static_cast<unsigned char>(9))
		valB = r_mem[4];
	else valB = 0;
	if (icode == 2 && cnd) dstE = rB;
	else if (icode == 3 || icode == 6) dstE = rB;
	else if (icode == 11 || icode == 10 || icode == 9 || icode == 8) dstE = 4;
	else dstE = 15;
	if (icode == 5 || icode == 11) dstM = rA;
	else dstM = 15;
}

void Register_memory::set_valE_to_rm(unsigned long valE) {
	if (dstE != 15) r_mem[dstE] = valE;
}

void Register_memory::set_valM_to_rm(unsigned long valM) {
	if (dstM != 15) r_mem[dstM] = valM;
}

void ALU::set_valE_and_cnd(unsigned long valA, unsigned long valB, unsigned long valC, unsigned char icode, unsigned char ifun) {
	unsigned char alufun;
	if (icode == static_cast<unsigned char>(6))
		alufun = ifun;
	else alufun = 0;
	long alu_A, alu_B;
	if (icode == static_cast<unsigned char>(6) || icode == static_cast<unsigned char>(2))
		alu_A = static_cast<long>(valA);
	else if (icode == static_cast<unsigned char>(3) || icode == static_cast<unsigned char>(4) || icode == static_cast<unsigned char>(5))
		alu_A = static_cast<long>(valC);
	else if (icode == static_cast<unsigned char>(8) || icode == static_cast<unsigned char>(10))
		alu_A = -8;
	else if (icode == static_cast<unsigned char>(9) || icode == static_cast<unsigned char>(11))
		alu_A = 8;
	else alu_A = 0;
	if (icode == static_cast<unsigned char>(4) || icode == static_cast<unsigned char>(5) || icode == static_cast<unsigned char>(6) ||
		icode == static_cast<unsigned char>(8) || icode == static_cast<unsigned char>(9) || icode == static_cast<unsigned char>(10) ||
		icode == static_cast<unsigned char>(11))
		alu_B = static_cast<long>(valB);
	else if (icode == static_cast<unsigned char>(2) || icode == static_cast<unsigned char>(3))
		alu_B = 0;
	else alu_B = 0;
	switch (alufun) {
	case 0:valE = static_cast<unsigned long>(alu_A + alu_B); break;
	case 1:valE = static_cast<unsigned long>(alu_B - alu_A); break;
	case 2:valE = static_cast<unsigned long>(alu_B & alu_A); break;
	case 3:valE = static_cast<unsigned long>(alu_B ^ alu_A); break;
	}
	if (valE == 0) ZF = true;
	if (static_cast<long>(valE) < 0) SF = true;
	if (alufun == 0) {
		if (alu_A > 0 && alu_B > 0 && static_cast<long>(valE) <= 0)
			OF = true;
		if (alu_A < 0 && alu_B < 0 && static_cast<long>(valE) >= 0)
			OF = true;
	}
}


void Cond::set_cnd(bool ZF, bool SF, bool OF, unsigned char ifun){
	if (ifun == static_cast<unsigned char>(0))
		cnd = true;
    else if(ifun==static_cast<unsigned char>(1)){
        if((SF^OF)|ZF) cnd = true;
        else cnd = false;
    }
	else if(ifun==static_cast<unsigned char>(2)){
        if(SF^OF) cnd = true;
        else cnd = false;
    }
	else if(ifun==static_cast<unsigned char>(3)){
        if(ZF) cnd = true;
        else cnd = false;
    }
	else if(ifun==static_cast<unsigned char>(4)){
        if(~ZF) cnd = true;
        else cnd = false;
    }
	else if(ifun==static_cast<unsigned char>(5)){
        if(~(SF^OF)) cnd = true;
        else cnd = false;
    }
	else if(ifun==static_cast<unsigned char>(6)){
        if((~(SF^OF))&~ZF) cnd = true;
        else cnd = false;
    }
	else cnd = false;
}
void data_memory::set_valM_and_error(unsigned char icode, unsigned long valA, unsigned long valE, unsigned long valP) {
	bool read;
	bool write;
	if (icode == 5 || icode == 11 || icode == 9) read = true;
	else read = false;
	if (icode == 4 || icode == 10 || icode == 8) write = true;
	else write = false;

	unsigned long addr;
	if (icode == 4 || icode == 5 || icode == 10 || icode == 8) addr = valE;
	else if (icode == 11 || icode == 9) addr = valA;

	if (icode == 4 || icode == 10) data = valA;
	else if (icode == 8) data = valP;

	if (write == false && read == false) {
		valM = 0;
		return;
	}
	if (addr+8 >= d_mem.size()) {
		dmem_error = true;
		valM = 0;
		return;
	}
	if (write) {
		for (int i = 0; i < 8; i++)
			d_mem[addr+i] = c_data[i];
		valM = 0;
	}
	else {
		for (int i = 0; i < 8; i++)
			c_valM[i] = d_mem[addr+i];
	}
}


void Y86::run(){
	while (1) {
		i_mem.set_target_instruction(pc.get_pc());
		split.set_ic_if(i_mem.get_imem_error(), i_mem.get_instruction_byte());
		align.set_r_valc(i_mem.get_other_byte(), split.get_icode());
		pc_add.set_valp(pc.get_pc(), split.get_icode());
		r_mem.set_value(split.get_icode(), align.get_rA(), align.get_rB(), cond.get_cnd());
		alu.set_valE_and_cnd(r_mem.get_valA(), r_mem.get_valB(), align.get_valc(), split.get_icode(), split.get_ifun());
		cc.set_sign(alu.get_ZF(), alu.get_SF(), alu.get_OF(), split.get_icode());
		d_mem.set_valM_and_error(split.get_icode(), r_mem.get_valA(), alu.get_valE(), pc_add.get_valp());
		stat.set_stat_state(i_mem.get_imem_error(), d_mem.get_dmem_error(), split.get_instr_valid(), split.get_icode());
		if (stat.get_stat_state() != 1) break;
		pc.set_pc(split.get_icode(), cond.get_cnd(), align.get_valc(), pc_add.get_valp(), d_mem.get_valM());
		cond.set_cnd(cc.get_ZF(), cc.get_SF(), cc.get_OF(), split.get_ifun());
		r_mem.set_valE_to_rm(alu.get_valE());
		r_mem.set_valM_to_rm(d_mem.get_valM());
	}
}
