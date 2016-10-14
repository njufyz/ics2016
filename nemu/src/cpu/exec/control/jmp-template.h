#include "cpu/exec/template-start.h"

#define instr jmp
extern int Len;
static void do_execute(){
    if(op_src->type==OP_TYPE_IMM)
    {
      Log("%x",op_src->val);
      cpu.eip +=op_src->val;
      Log("cpu.eipN=%x %x",cpu.eip+Len,cpu.eip);
      if (DATA_BYTE == 2 )
          cpu.eip &= 0xffff;
    }
    else{
        if (DATA_BYTE == 2) 
            op_src->val &= 0xffff;
        cpu.eip = op_src->val - Len;
    }


    print_asm_template1();
}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
