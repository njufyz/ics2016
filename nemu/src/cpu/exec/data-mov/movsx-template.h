#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
   // DATA_TYPE_S temp = op_src->val;
  //  uint32_t result = temp;
   // uint32_t result = op_src->val;
   // if(MSB(result))
  //    result = (0xffffffff<<(8*DATA_BYTE)) | op_src->val;
    uint32_t result = (DATA_TYPE_S) op_src->val;
    write_operand_l(op_dest,result);
    print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
            



    
