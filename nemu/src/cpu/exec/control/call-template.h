#include "cpu/exec/template-start.h"

#define instr call

extern int Len;
static void do_execute()
{
    cpu.esp-=4;
    swaddr_write(cpu.esp,4,cpu.eip+Len, R_SS);
    if(instr_fetch(cpu.eip,1)==0xe8)
    {
        cpu.eip+=op_src->val;
        print_asm("call $0x%x", cpu.eip + Len);
    }
    else
    {
        cpu.eip=op_src->val - Len;
        print_asm("call $0x%x", cpu.eip + Len);
    }
    //print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
