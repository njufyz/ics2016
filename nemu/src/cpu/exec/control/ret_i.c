#include "cpu/exec/helper.h"

#define instr ret
make_helper(ret_i){
    cpu.eip = swaddr_read(cpu.gpr[4]._32,4);
    cpu.gpr[4]._32 += 4;
    uint32_t offset = instr_fetch(cpu.eip+1,2);
    Log("%x",cpu.eip);
    cpu.eip = cpu.eip + offset -1;
    print_asm_template1();
    return 3;
}
