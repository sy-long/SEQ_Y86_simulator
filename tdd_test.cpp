#include <iostream>
#include "seq_Y86_simulator.h"
using std::cout;
using std::endl;
static int count=0;
static int pass=0;

#define expect_actual( expect, actual) do{ count++;\
if (expect == actual) pass++;\
else cout<<__FILE__<<"  "<<__LINE__<<"  "<<"expect: "<<expect<<"  actual:  "<< actual<< endl;\
}while(0)\

void test_pc(){
    PC pc;
    expect_actual(0,pc.get_pc());
	pc.set_pc(8, false, 125, 9, 0);
	expect_actual(125, pc.get_pc());
	pc.set_pc(7, true, 125, 9, 0);
	expect_actual(125, pc.get_pc());
	pc.set_pc(9, false, 0, 1, 128);
	expect_actual(128, pc.get_pc());
	pc.set_pc(6, false, 0, 2, 0);
	expect_actual(2, pc.get_pc());
}

void test_stat() {
	Stat stat;
	expect_actual(1, stat.get_stat_state());
	stat.set_stat_state(true, false, true, false);
	expect_actual(2, stat.get_stat_state());
	stat.set_stat_state(false, true, true, false);
	expect_actual(2, stat.get_stat_state());
	stat.set_stat_state(false, false, false, false);
	expect_actual(3, stat.get_stat_state());
	stat.set_stat_state(false, false, true, true);
	expect_actual(4, stat.get_stat_state());
}

void test_instruction_memory() {
	Instruction_memory i_mem;
	vector<unsigned char> user_code = { 96,19,0,0,0,0,0,0,0,0 };
	i_mem.set_i_mem(user_code);
	i_mem.set_target_instruction(0);
	expect_actual(96, i_mem.get_instruction_byte());
	vector<unsigned char> other = i_mem.get_other_byte();
	expect_actual(19, other[0]);
	for (int i = 1; i < 9; i++) {
		expect_actual(0, other[i]);
	}
	expect_actual(false, i_mem.get_imem_error());

	user_code = { 96,19,0,0,0,0 };
	i_mem.set_i_mem(user_code);
	i_mem.set_target_instruction(0);
	expect_actual(16, i_mem.get_instruction_byte());
	other = i_mem.get_other_byte();
	expect_actual(19, other[0]);
	for (int i = 1; i < 9; i++) {
		expect_actual(0, other[i]);
	}
	expect_actual(true, i_mem.get_imem_error());
}

void test_split() {
	Split split;
	split.set_ic_if(false, 113);
	expect_actual(7, split.get_icode());
	expect_actual(1, split.get_ifun());
	expect_actual(true, split.get_instr_valid());
	split.set_ic_if(false, 177);
	expect_actual(1, split.get_icode());
	expect_actual(0, split.get_ifun());
	expect_actual(false, split.get_instr_valid());
}

void test_pc_add() {
	PC_Add pc_add;
	pc_add.set_valp(2, 6);
	expect_actual(4, pc_add.get_valp());
	pc_add.set_valp(4, 3);
	expect_actual(14, pc_add.get_valp());
}

void test_align() {
	Align align;
	vector<unsigned char> other = { 19,0,0,0,0,0,0,0,0 };
	align.set_r_valc(other, 6);
	expect_actual(1, align.get_rA());
	expect_actual(3, align.get_rB());
	other = { 19,2,0,0,0,0,0,0,0 };
	align.set_r_valc(other, 4);
	expect_actual(1, align.get_rA());
	expect_actual(3, align.get_rB());
	expect_actual(2, align.get_valc());
}

void test_register_memory() {
	Register_memory r_mem;
	r_mem.set_value(6, 1, 3, false);
	expect_actual(0, r_mem.get_valA());
	expect_actual(0, r_mem.get_valB());
}

void test_alu() {
	ALU alu;
	alu.set_valE_and_cnd(1, 3, 5, 6, 0);
	expect_actual(false, alu.get_ZF());
	expect_actual(false, alu.get_SF());
	expect_actual(false, alu.get_OF());
	expect_actual(4, alu.get_valE());
	alu.set_valE_and_cnd(1, 1, 5, 6, 1);
	expect_actual(true, alu.get_ZF());
	expect_actual(false, alu.get_SF());
	expect_actual(false, alu.get_OF());
	expect_actual(0, alu.get_valE());
}

void test_cc() {
	CC cc;
	cc.set_sign(false, true, false, 4);
	expect_actual(false, cc.get_SF());
	cc.set_sign(false, true, false, 6);
	expect_actual(true, cc.get_SF());
}

void test_cond() {
	Cond cond;
	cond.set_cnd(false, false, false, 0);
	expect_actual(true, cond.get_cnd());
	cond.set_cnd(true, false, false, 3);
	expect_actual(true, cond.get_cnd());
}

void test(){
    test_pc();
    test_stat();
    test_instruction_memory();
    test_split();
    test_pc_add();
    test_align();
    test_register_memory();
    test_alu();
    test_cc();
    test_cond();
}

int main(){
    test();
    cout<<pass<<"/"<<count<<endl;
}
