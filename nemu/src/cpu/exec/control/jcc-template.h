#include "cpu/exec/template-start.h"
#define update_eip()\
    int32_t v = op_src->val;\
    v<<= (32 - 8 * DATA_BYTE);\
    v>>= (32 - 8 * DATA_BYTE);\
    uint32_t new_ = cpu.eip + v;\
    if(DATA_BYTE==2) \
        new_&=0xffff;\
    print_asm_template1();

/*------------------------*/
#define instr je

static void do_execute()
{
    update_eip();
    if(cpu.eflags.zf==1) 
        cpu.eip=new_;
}

make_instr_helper(i)
#undef instr



/*------------------------*/

#define instr jbe

static void do_execute(){
    update_eip();
    if(cpu.eflags.cf==1 || cpu.eflags.zf==1)
        cpu.eip=new_;
}

make_instr_helper(i)

#undef instr

/*----------------------*/


#define instr jne

static void do_execute(){
    update_eip();
    if(cpu.eflags.zf==0)
        cpu.eip=new_;
}

make_instr_helper(i)

#undef instr

/*----------------------*/
#define instr jle

static void do_execute(){
    update_eip();
    if(cpu.eflags.zf==1 || cpu.eflags.sf != cpu.eflags.of)
        cpu.eip=new_;
}

make_instr_helper(i)
#undef instr

/*-----------------------*/
#define instr jg

static void do_execute(){
    update_eip();
    if(cpu.eflags.zf==0 && cpu.eflags.sf==cpu.eflags.of)
        cpu.eip=new_;
}

make_instr_helper(i)
#undef instr

/*-----------------------------*/
#define instr jl

static void do_execute(){
    update_eip();
    if(cpu.eflags.sf!=cpu.eflags.of)
        cpu.eip=new_;
}

make_instr_helper(i)

#undef instr

/*------------------------------*/
#define instr jge

static void do_execute(){
    update_eip();
    if(cpu.eflags.sf==cpu.eflags.of)
        cpu.eip=new_;
}

make_instr_helper(i)
#undef instr

/*-----------------------------*/

#define instr ja

static void do_execute(){
    update_eip();
    if(cpu.eflags.cf==0&&cpu.eflags.zf==0)
        cpu.eip=new_;
}
make_instr_helper(i)
#undef instr

#include "cpu/exec/template-end.h"










