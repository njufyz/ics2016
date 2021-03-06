#include "cpu/exec/template-start.h"

#define instr mov
void load_segcache(uint8_t sreg);
static void do_execute() {
    //seg mov
     if(instr_fetch(cpu.eip, 1) == 0x8c){
        OPERAND_W(op_dest, cpu.segreg[op_src->reg].val);
        print_asm("movw %%%s, %%%s",REG_NAMES(op_src->reg), REG_NAME(op_dest->reg));
        return;
    }
    else if(instr_fetch(cpu.eip, 1) == 0x8e){
        cpu.segreg[op_dest->reg].val = REG(op_src->reg);
        load_segcache(op_dest->reg);             //load segcache
        print_asm("movw %%%s, %%%s  %s:0x%x",REG_NAME(op_src->reg), REG_NAMES(op_dest->reg), REG_NAMES(op_dest->reg), cpu.segcache[op_dest->reg].base);
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

#define GetReg (instr_fetch(cpu.eip + 2, 1))
make_helper(concat(mov_r2cr_,SUFFIX)){
    uint32_t reg = GetReg ;
    if(((reg >> 3) & 0x7) == 0) {                     //cr0
        cpu.cr0.val = REG( reg & 0x7 );
        print_asm("movl %%%s, %%cr0", REG_NAME( reg & 0x7));
    }
    else{                                          //cr3
        cpu.cr3.val = REG( reg & 0x7 );
        print_asm("movl %%%s, %%cr3", REG_NAME( reg & 0x7));
    }
    return 2;
}

make_helper(concat(mov_cr2r_,SUFFIX)){
    uint32_t reg = GetReg;
    if(((reg >> 3) & 0x7) == 0) {                     //cr0
        REG( reg & 0x7 ) = cpu.cr0.val;
        print_asm("movl  %%cr0, %%%s", REG_NAME( reg & 0x7));
    }
    else{                                          //cr3
        REG( reg & 0x7 ) =  cpu.cr3.val;
        print_asm("movl  %%cr3, %%%s", REG_NAME( reg & 0x7));
    }
    return 2;
}

#include "cpu/exec/template-end.h"
