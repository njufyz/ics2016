#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    
    OPERAND_W(op_src,swaddr_read(cpu.gpr[4]._32,4));
    cpu.gpr[4]._32+=DATA_BYTE;
    print_asm_template1();   
}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
