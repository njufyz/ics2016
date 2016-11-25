#include "cpu/exec/template-start.h"

#define instr jmp
extern int Len;
static void do_execute(){
    if(instr_fetch(cpu.eip,1)==0xeb||instr_fetch(cpu.eip,1)==0xe9)
    {
      DATA_TYPE_S temp = op_src->val;
      int32_t tp = temp;
      cpu.eip +=tp;
      if (DATA_BYTE == 2 )
          cpu.eip &= 0xffff;
    print_asm("jmp $0x%x",cpu.eip+Len);;
    }
    else if(instr_fetch(cpu.eip, 1) == 0xea)
    {
        cpu.segreg[R_CS].val = swaddr_read(op_src->addr, 2, R_CS);
        cpu.eip = swaddr_read(op_src->addr + 2, 4, R_CS);
        print_asm("ljmp");
    }
    else{
        if (DATA_BYTE == 2) 
            op_src->val &= 0xffff;
        cpu.eip = op_src->val - Len;
    print_asm("jmp $0x%x",(op_src->val));
    }


}

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
