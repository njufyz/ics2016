#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
    Log("%x",op_src->val);
    uint32_t addr = instr_fetch(cpu.eip + 3, 4);
    cpu.gdtr.limit = lnaddr_read(addr, 2);
    cpu.gdtr.base = lnaddr_read(addr + 2 , 4);
    print_asm("base: 0x%x limit: %x",cpu.gdtr.base,cpu.gdtr.limit);
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
