#include "cpu/exec/template-start.h"

#define instr setne

static void do_execute(){
    if(cpu.eflags.zf==0) 
        OPERAND_W(op_src,1);
    else OPERAND_W(op_src,0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

/*--------------------------*/
#define instr setge

static void do_execute(){
    if(cpu.eflags.sf==cpu.eflags.of) 
        OPERAND_W(op_src,1);
    else OPERAND_W(op_src,0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr

/*--------------------------*/
#define instr sete

static void do_execute(){
    if(cpu.eflags.zf == 1) 
        OPERAND_W(op_src,1);
    else OPERAND_W(op_src,0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr


#define instr seta

static void do_execute(){
    if(cpu.eflags.zf == 0 && cpu.eflags.cf == 0) 
        OPERAND_W(op_src,1);
    else OPERAND_W(op_src,0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr
/*--------------------------*/
#define instr setle

static void do_execute(){
    if(cpu.eflags.zf == 1 && cpu.eflags.sf != cpu.eflags.of) 
        OPERAND_W(op_src,1);
    else OPERAND_W(op_src,0);
    print_asm_template1();
}

make_instr_helper(rm)

#undef instr
#include "cpu/exec/template-end.h"
