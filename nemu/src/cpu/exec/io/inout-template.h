#include "cpu/exec/template-start.h"


uint32_t pio_read(ioaddr_t, size_t);
void pio_write(ioaddr_t, size_t, uint32_t);


make_helper(concat(in_,SUFFIX)){
#if DATA_BYTE == 1
    cpu.gpr[0]._8[0] = pio_read(cpu.edx & 0xffff, 1);
#elif DATA_BYTE == 2
    cpu.gpr[0]._16 = pio_read(cpu.edx & 0xffff, 2);
#else
    cpu.eax = pio_read(cpu.edx &0xffff, 4);
#endif
    print_asm("in");
    return 1;
}




make_helper(concat(out_, SUFFIX)){
    ioaddr_t addr = cpu.edx & 0xffff;
    uint32_t data = MEM_R(cpu.esi, R_DS);
    pio_write(addr, DATA_BYTE, data);
    if(cpu.eflags.df == 0)
        cpu.esi += DATA_BYTE;
    else 
        cpu.esi -= DATA_BYTE;

    print_asm("in");
    return 1;
}

#include "cpu/exec/template-end.h"
