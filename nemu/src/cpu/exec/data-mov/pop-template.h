#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    
   // OPERAND_W(op_dest,op_src->val);
   // swaddr_write(reg_l(op_src->val),4,swaddr_read(reg_l(4),4));
   // cpu.gpr[4]._32+=4;
   // print_asm_template1();
   
    MEM_W(op_src->val,cpu.gpr[4]._32);
    cpu.gpr[4]._32+=DATA_BYTE;
    printf("rp\n");

}

make_instr_helper(r)

#include "cpu/exec/template-end.h"
