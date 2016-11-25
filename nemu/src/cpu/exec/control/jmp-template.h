#include "cpu/exec/template-start.h"

#define instr jmp
extern int Len;
void load_segcache(uint8_t);

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
    else{
        if (DATA_BYTE == 2) 
            op_src->val &= 0xffff;
        cpu.eip = op_src->val - Len;
    print_asm("jmp $0x%x",(op_src->val));
    }


}

#if DATA_BYTE == 4
make_helper(ljmp){
        cpu.segreg[R_CS].val = instr_fetch(cpu.eip + 5,  2);
        load_segcache(R_CS);
        cpu.eip = instr_fetch(cpu.eip + 1, 4) - Len;
        print_asm("ljmp");
        return Len;
}
#endif

make_instr_helper(i)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(rm)
#endif

#include "cpu/exec/template-end.h"
