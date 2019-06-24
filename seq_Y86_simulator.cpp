#include "seq_Y86_simulator.h"

Stat::error Stat::get_stat_state(){
    if(adr) return ADR;
    else if(!ins) return INS;
    else if(hlt) return HLT;
    else return AOK;
}

void Instruction_memory::set_target_instruction(unsigned long pc){
    if(pc+9>i_mem.size()){
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
    if(icode==static_cast<unsigned char>(3)||icode==static_cast<unsigned char>(4)||icode==static_cast<unsigned char>(5)||icode==static_cast<unsigned char>(7)||icode==static_cast<unsigned char>(8))
       need_valc=true;
    else need_valc=false;
}
void PC_Add::set_need_r(unsigned char icode){
    if(icode==static_cast<unsigned char>(0)||icode==static_cast<unsigned char>(1)||icode==static_cast<unsigned char>(7)||icode==static_cast<unsigned char>(8)||icode==static_cast<unsigned char>(9))
        need_r=false;
    else need_r=true;
}
void PC_Add::set_valp(unsigned long pc){
    valp=pc+1;
    if(need_r) valp++;
    if(need_valc) valp+=8;
}

void Align::set_r_valc(vector<char> other_byte){
    unsigned char temp_r=static_cast<unsigned char>(other_byte[0]);
    rA=temp_r>>4;
    rB=temp_r&static_cast<unsigned char>(15);
    if(need_r){
        for(int i=0;i<8;i++)
            c_valc[i]=other_byte[i+1];
    }
    else{
        for(int i=0;i<8;i++)
            c_valc[i]=other_byte[i];
    }
}

unsigned long Register_memory::get_valA(){
    if(icode==static_cast<unsigned char>(2)||icode==static_cast<unsigned char>(4)||icode==static_cast<unsigned char>(6)||
       icode==static_cast<unsigned char>(10))
        return r_mem[rA];
    else if(icode==static_cast<unsigned char>(9)||icode==static_cast<unsigned char>(11))
        return r_mem[4];
    else return 0;
}

unsigned long Register_memory::get_valB(){
    if(icode==static_cast<unsigned char>(6)||icode==static_cast<unsigned char>(4)||icode==static_cast<unsigned char>(5))
        return r_mem[rB];
    else if(icode==static_cast<unsigned char>(10)||icode==static_cast<unsigned char>(11)||icode==static_cast<unsigned char>(8)||icode==static_cast<unsigned char>(9))
        return r_mem[4];
    else return 0;
}
void ALU::set_alufun(){
    if(icode==static_cast<unsigned char>(6))
        alufun=ifun;
    else alufun=0;
}
unsigned long ALU::get_valE(){
    long alu_A,alu_B;
    unsigned long valE;
    if(icode==static_cast<unsigned char>(6)||icode==static_cast<unsigned char>(2))
        alu_A=static_cast<long>(valA);
    else if(icode==static_cast<unsigned char>(3)||icode==static_cast<unsigned char>(4)||icode==static_cast<unsigned char>(5))
        alu_A=static_cast<long>(valC);
    else if(icode==static_cast<unsigned char>(8)||icode==static_cast<unsigned char>(10))
        alu_A=-8;
    else if(icode==static_cast<unsigned char>(9)||icode==static_cast<unsigned char>(11))
        alu_A=8;
    else alu_A=0;
    if(icode==static_cast<unsigned char>(4)||icode==static_cast<unsigned char>(5)||icode==static_cast<unsigned char>(6)||
       icode==static_cast<unsigned char>(8)||icode==static_cast<unsigned char>(9)||icode==static_cast<unsigned char>(10)||
       icode==static_cast<unsigned char>(11))
        alu_B=static_cast<long>(valB);
    else if(icode==static_cast<unsigned char>(2)||icode==static_cast<unsigned char>(3))
        alu_B=0;
    else alu_B=0;
    switch (alufun) {
    case 0:valE=static_cast<unsigned long>(alu_A+alu_B);break;
    case 1:valE=static_cast<unsigned long>(alu_B-alu_A);break;
    case 2:valE=static_cast<unsigned long>(alu_B&alu_A);break;
    case 3:valE=static_cast<unsigned long>(alu_B^alu_A);break;
    }
    if(valE==0) ZF=true;
    if(static_cast<long>(valE)<0) SF=true;
    if(alu_A>0&&alu_B>0&&static_cast<long>(valE)<=0)
        OF=true;
    if(alu_A<0&&alu_B<0&&static_cast<long>(valE)>=0)
        OF=true;
    return valE;
}

bool Cond::get_cnd(){
    if(ifun==static_cast<unsigned char>(0))
        return true;
    if(ifun==static_cast<unsigned char>(1)){
        if((SF^OF)|ZF) return true;
        else return false;
    }
    if(ifun==static_cast<unsigned char>(2)){
        if(SF^OF) return true;
        else return false;
    }
    if(ifun==static_cast<unsigned char>(3)){
        if(ZF) return true;
        else return false;
    }
    if(ifun==static_cast<unsigned char>(4)){
        if(~ZF) return true;
        else return false;
    }
    if(ifun==static_cast<unsigned char>(5)){
        if(~(SF^OF)) return true;
        else return false;
    }
    if(ifun==static_cast<unsigned char>(6)){
        if((~(SF^OF))&~ZF) return true;
        else return false;
    }
    return false;
}

void data_memory::set_r_or_w(unsigned char icode) {
	if (icode == 5 || icode == 11 || icode == 9) read = true;
	else read = false;
	if (icode == 4 || icode == 10 || icode == 8) write = true;
	else write = false;
}

void data_memory::set_addr(unsigned char icode, unsigned long valA, unsigned long valE) {
	if (icode == 4 || icode == 5 || icode == 10 || icode == 8) addr = valE;
	else if (icode == 11 || icode == 9) addr = valA;
}
void data_memory::set_data(unsigned char icode, unsigned long valA, unsigned long valP) {
	if (icode == 4 || icode == 10) data = valA;
	else if (icode == 8) data = valP;
}
unsigned long data_memory::get_valM() {
	if (write == false && read == false)
		return 0;
	if (addr+8 >= d_mem.size()) {
		dmem_error = true;
		return 0;
	}
	if (write) {
		for (int i = 0; i < 8; i++)
			d_mem[addr+i] = c_data[i];
		return 0;
	}
	else {
		for (int i = 0; i < 8; i++)
			c_valM[i] = d_mem[addr+i];
		return valM;
	}
}

void PC::set_pc(unsigned char icode, bool cnd, unsigned long valC, unsigned long valP, unsigned long valM) {
	if (icode == 8) pc = valC;
	else if (icode == 7 && cnd) pc = valC;
	else if (icode == 9) pc = valM;
	else pc = valP;
}

void Register_memory::set_rE(bool cnd) {
	if (icode == 2 && cnd) rE = rB;
	else if (icode == 3 || icode == 6) rE = rB;
	else if (icode == 11 || icode == 10 || icode == 9 || icode == 8) rE = 4;
	else rE = 15;
}

void Register_memory::set_rM() {
	if (icode == 5 || icode == 11) rM = rA;
	else rM = 15;
}

void Register_memory::set_valE_to_m(unsigned long valE) {
	if (rE != 15) r_mem[rE] = valE;
}

void Register_memory::set_valM_to_m(unsigned long valM) {
	if (rM != 15) r_mem[rM] = valM;
}

void Y86::run(){
    i_mem.set_target_instruction(pc.get_pc());
    stat.set_adr(i_mem.get_imem_error());
    split.set_imem_error_state(i_mem.get_imem_error());
    split.set_ic_if(i_mem.get_instruction_byte());
    stat.set_ins(split.get_instr_valid());
    pc_add.set_need_r(split.get_icode());
    pc_add.set_need_valc(split.get_icode());
    pc_add.set_valp(pc.get_pc());
    align.set_need_r(pc_add.get_need_r());
    align.set_r_valc(i_mem.get_other_byte());
    r_mem.set_rA(align.get_rA());
    r_mem.set_rB(align.get_rB());
    r_mem.set_icode(split.get_icode());
    alu.set_icode(split.get_icode());
    alu.set_ifun(split.get_ifun());
    alu.set_valA(r_mem.get_valA());
    alu.set_valB(r_mem.get_valB());
    alu.set_valC(align.get_valc());
    alu.set_alufun();
    cc.set_set_cc(split.get_icode());
	d_mem.set_r_or_w(split.get_icode);
	d_mem.set_addr(split.get_icode, r_mem.get_valA, alu.get_valE);
	d_mem.set_data(split.get_icode, r_mem.get_valA, pc_add.get_valp);
	pc.set_pc(split.get_icode, cond.get_cnd, align.get_valc, pc_add.get_valp, d_mem.get_valM);
	cc.set_sign(alu.get_ZF(), alu.get_SF(), alu.get_OF());
	cond.set_sign(cc.get_ZF(), cc.get_SF(), cc.get_OF());
	cond.set_ifun(split.get_ifun());
	//todo
}
