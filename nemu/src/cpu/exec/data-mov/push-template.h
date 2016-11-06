#include "cpu/exec/template-start.h"

#define instr push
static void do_execute()
{
    if(ops_decoded.is_operand_size_16 && instr_fetch(cpu.eip+1,1)==0x6a)
        {
            int8_t t = op_src->val;
            op_src->val = t;
        }
    else if(ops_decoded.is_operand_size_16 == 0 && instr_fetch(cpu.eip,1)==0x6a)
        {
            int8_t t = op_src->val;
            op_src->val = t;
        }

    if(DATA_BYTE==2)
    {   
        cpu.esp-=2;
        swaddr_write(cpu.esp,2,op_src->val);
    }
    else
    {
        cpu.esp-=4;
        swaddr_write(cpu.esp,4,op_src->val);
    }
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
