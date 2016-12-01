#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
    uint32_t addr = instr_fetch(cpu.eip + 2, 4);
    Log("%x",addr);
    cpu.gdtr.limit = lnaddr_read(op_src->addr, 2);
    cpu.gdtr.base = lnaddr_read(op_src->addr , 4);
    print_asm("base: 0x%x limit: %x",cpu.gdtr.base,cpu.gdtr.limit);
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
