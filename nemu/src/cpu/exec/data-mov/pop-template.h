#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
    
    OPERAND_W(op_src,MEM_R(cpu.esp, R_SS));
    cpu.esp += DATA_BYTE;
    print_asm_template1();   
}

make_instr_helper(r)

#if DATA_BYTE == 4
make_helper(popad){
    cpu.edi = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;

    cpu.esi = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;

    cpu.ebp = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;

    cpu.esp += 4;

    cpu.ebx = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;

    cpu.edx = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;
    
    cpu.ecx = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;

    cpu.eax = MEM_R(cpu.esp, R_SS);
    cpu.esp += 4;

    print_asm("popad");
    return 1;
}
#endif

#include "cpu/exec/template-end.h"
