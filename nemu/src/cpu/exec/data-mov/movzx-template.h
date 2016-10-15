#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){
    uint32_t result;
    if(instr_fetch(cpu.eip+1,1)==0xb6)
    {
         result =op_src->val & 0xff;   //bw
    }
    else
        result = op_src->val & 0xffff;
//    long long i =1;
 //   result&=((i<<op_src->size * 8)-1);
   /* uint32_t result = temp; */ 
   OPERAND_W(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
