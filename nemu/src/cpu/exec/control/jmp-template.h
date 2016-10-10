#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
    if(op_src->type==OP_TYPE_IMM)
    {
      cpu.eip +=op_src->val;
      if (DATA_BYTE == 2 )
          cpu.eip &= 0xffff;
    }
    else{
       if (DATA_BYTE == 2)
       {
           cpu.eip = (op_src->val & 0xffff);
           cpu.eip -=2;
       }
       else 
           cpu.eip = op_src->val;
       if(op_src->type == OP_TYPE_MEM)
           cpu.eip -= DATA_BYTE + 3;
       else cpu.eip -= 2;
    }


    print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
