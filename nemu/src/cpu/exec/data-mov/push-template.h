#include "cpu/exec/template-start.h"

#define instr push
uint32_t eip;
static void do_execute()
{
    if(DATA_BYTE==2)
    {   
        if(instr_fetch(eip,1)==0x6a)
        {
            int8_t t = op_src->val;
            op_src->val = t;
        }
        cpu.esp-=DATA_BYTE;
        swaddr_write(cpu.esp,2,op_src->val);
    }
    else
    {
        if(instr_fetch(eip,1)==0x6a)
        {
            int8_t t = op_src->val;
            op_src->val = t;
        }
        cpu.esp-=4;
        swaddr_write(cpu.esp,4,op_src->val);
    }
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
