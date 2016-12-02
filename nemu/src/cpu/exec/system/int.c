#include "cpu/exec/helper.h"

void raise_intr(uint8_t);
make_helper(int_imm8){
    uint8_t no = instr_fetch(cpu.eip + 1, 1);
   
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eflags.val, R_SS);  //push eflags
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.segreg[R_CS].val, R_SS);  //push CS
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eip, R_SS);  //push eip

    Log("111");
    raise_intr(no);

    print_asm("int %x", no);
    return 2;
}

