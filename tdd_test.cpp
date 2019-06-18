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
    pc.set_pc(1000);
    expect_actual(1000,pc.get_pc());
}
void test_stat(){
    Stat stat;
    expect_actual(Stat::AOK,stat.get_stat_state());
    stat.set_hlt(true);
    expect_actual(Stat::HLT,stat.get_stat_state());
    stat.set_ins(false);
    expect_actual(Stat::INS,stat.get_stat_state());
    stat.set_adr(true);
    expect_actual(Stat::ADR,stat.get_stat_state());
}
void test_instruction_memory(){
    Instruction_memory i_mem;
    vector<char> test{-80,2,3,4,5,6,7,8,9,10,11,12};
    i_mem.set_i_mem(test);
    i_mem.set_target_instruction(0);
    expect_actual(176,i_mem.get_instruction_byte());
    vector<char> t_other=i_mem.get_other_byte();
    for(size_t i=0;i<9;i++){
        expect_actual(test[i+1],t_other[i]);
    }
    expect_actual(false,i_mem.get_imem_error());

    i_mem.set_target_instruction(20);
    expect_actual(16,i_mem.get_instruction_byte());
    expect_actual(true,i_mem.get_imem_error());
}
void test_split(){
    Split split;
    split.set_imem_error_state(false);
    split.set_ic_if(static_cast<unsigned char>(97));
    expect_actual(6,split.get_icode());
    expect_actual(1,split.get_ifun());
    expect_actual(true,split.get_instr_valid());

    split.set_imem_error_state(false);
    split.set_ic_if(static_cast<unsigned char>(177));
    expect_actual(1,split.get_icode());
    expect_actual(0,split.get_ifun());
    expect_actual(false,split.get_instr_valid());

    split.set_imem_error_state(true);
    split.set_ic_if(static_cast<unsigned char>(97));
    expect_actual(6,split.get_icode());
    expect_actual(1,split.get_ifun());
    expect_actual(true,split.get_instr_valid());
}
void test_pc_add(){
    PC_Add pc_add;
    pc_add.set_need_r(1);
    pc_add.set_need_valc(1);
    pc_add.set_valp(0);
    expect_actual(1,pc_add.get_valp());
    pc_add.set_need_r(2);
    pc_add.set_valp(0);
    expect_actual(2,pc_add.get_valp());
    pc_add.set_need_valc(3);
    pc_add.set_valp(0);
    expect_actual(10,pc_add.get_valp());
}
void test_align(){
    Align align;
    align.set_need_r(true);
    vector<char> test{-29,1,0,0,0,0,0,0,0};
    align.set_r_valc(test);
    expect_actual(14,align.get_rA());
    expect_actual(3,align.get_rB());
    expect_actual(1,align.get_valc());
}
void test_register_memory(){
    Register_memory r_mem;
    r_mem.set_icode(6);
    r_mem.set_rA(2);
    r_mem.set_rB(3);
    expect_actual(0,r_mem.get_valA());
    expect_actual(0,r_mem.get_valB());
}
void test(){
    test_pc();
    test_stat();
    test_instruction_memory();
    test_split();
    test_pc_add();
    test_align();
    test_register_memory();
}

int main(){
    test();
    cout<<pass<<"/"<<count<<endl;
}
