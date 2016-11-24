#include "cpu/exec/template-start.h"

#define instr mov
void load_segcache(uint8_t sreg);
static void do_execute() {
    //CR0 mov
    if(instr_fetch(cpu.eip,2) == 0x200f){
    OPERAND_W(op_dest, cpu.cr0.val);
    print_asm("movl %%cr0,%%%s",REG_NAME((op_dest->reg)));

    return;
    }
    else if(instr_fetch(cpu.eip,2) == 0x220f){
        int k = op_dest->reg;
        cpu.cr0.val = REG(op_dest->reg);
        
        print_asm("movl %%%s,%%cr0",REG_NAME(k));
        return;
    }

    //seg mov
    else if(instr_fetch(cpu.eip, 1) == 0x8c){
        OPERAND_W(op_dest, cpu.segreg[op_src->reg].val);
        print_asm("movw %%%s, %%%s",REG_NAMES(op_src->reg), REG_NAME(op_dest->reg));
        return;
    }
    else if(instr_fetch(cpu.eip, 1) == 0x8e){
        cpu.segreg[op_dest->reg].val = REG(op_src->reg);
        load_segcache(op_src->reg);             //load segcache
        print_asm("movw %%%s, %%%s",REG_NAME(op_src->reg), REG_NAMES(op_dest->reg));
        return;
    }

    OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
make_instr_helper(rm)

make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX), R_DS);

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr, R_DS);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"
