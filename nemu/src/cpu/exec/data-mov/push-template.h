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
        cpu.esp-=DATA_BYTE;
        swaddr_write(cpu.esp,2,op_src->val, R_SS);
    }
    else
    {
        cpu.esp-=4;
        swaddr_write(cpu.esp,4,op_src->val, R_SS);
    }
    print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#if DATA_BYTE == 4
make_helper(pushad){
    uint32_t temp = cpu.esp;

    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eax, R_SS);
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.ecx, R_SS);
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.edx, R_SS);
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.ebx, R_SS);

    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, temp, R_SS);

    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.ebp, R_SS);

    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.esi, R_SS);

    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.edi, R_SS);

    print_asm("pushad");
    return 1;
}
#endif
#include "cpu/exec/template-end.h"
