#include "cpu/exec/helper.h"

#define instr ret
make_helper(ret_i){

    uint32_t offset = instr_fetch(cpu.eip+1,2);
    cpu.eip = swaddr_read(cpu.gpr[4]._32,4);
    cpu.gpr[4]._32 += 4;
    cpu.eip -= 7;
    cpu.eip += offset;
    print_asm_template1();
    return 3;
}
