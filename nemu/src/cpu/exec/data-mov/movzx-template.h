#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){
    uint32_t result = op_src->val;
    long long i =1;
    result&=((i<<op_src->size * 8)-1);
   /* uint32_t result = temp; */ 
    if(cpu.eip==0x100a1c) 
        Log("result=%x",result);
    write_operand_l(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
