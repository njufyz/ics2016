#include "cpu/exec/helper.h"

void raise_intr(uint8_t);
make_helper(int_imm8){
    uint8_t no = instr_fetch(cpu.eip + 1, 1);

    cpu.eip += 2;
    raise_intr(no);
    print_asm("int %x", no);
    return 2;
}

