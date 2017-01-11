#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
 //   uint32_t addr = instr_fetch(cpu.eip + 3, 4);
    uint32_t addr = op_src->addr;
    cpu.gdtr.limit = lnaddr_read(addr, 2);
    cpu.gdtr.base = lnaddr_read(addr + 2 , 4);
    print_asm("gdtr base: 0x%x limit: %x",cpu.gdtr.base,cpu.gdtr.limit);
}

make_instr_helper(rm);

#undef instr

/*-----------------------*/

#define instr lidt

static void do_execute(){
    uint32_t addr = op_src->addr;
    cpu.idtr.limit = lnaddr_read(addr, 2);
    cpu.idtr.base = lnaddr_read(addr + 2 , 4);
    print_asm("idtr base: 0x%x limit: %x",cpu.idtr.base,cpu.idtr.limit);
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"
