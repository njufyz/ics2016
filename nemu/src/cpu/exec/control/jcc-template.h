#include "cpu/exec/template-start.h"
extern int Len;
#define update_eip()\
    DATA_TYPE_S t = op_src->val;\
    int v = t;\
    uint32_t _new = cpu.eip + v;\
    if(DATA_BYTE==2) \
        _new&=0xffff;\
    print_asm("%s $0x%x",str(instr),_new+Len);

/*------------------------*/
#define instr je

static void do_execute()
{
    update_eip();
    if(cpu.eflags.zf==1) 
        cpu.eip=_new;
}

make_instr_helper(i)
#undef instr



/*------------------------*/

#define instr jbe

static void do_execute(){
    update_eip();
    if(cpu.eflags.cf==1 || cpu.eflags.zf==1)
        cpu.eip=_new;
}

make_instr_helper(i)

#undef instr

/*----------------------*/


#define instr jne

static void do_execute(){
    update_eip();
    if(cpu.eflags.zf==0)
        cpu.eip=_new;
}

make_instr_helper(i)

#undef instr

/*----------------------*/
#define instr jle

static void do_execute(){
    update_eip();
    if(cpu.eflags.zf==1 || cpu.eflags.sf != cpu.eflags.of)
        cpu.eip=_new;
}

make_instr_helper(i)
#undef instr

/*-----------------------*/
#define instr jg

static void do_execute(){
    update_eip();
    if(cpu.eflags.zf==0 && cpu.eflags.sf==cpu.eflags.of)
        cpu.eip=_new;
}

make_instr_helper(i)
#undef instr

/*-----------------------------*/
#define instr jl

static void do_execute(){
    update_eip();
    if(cpu.eflags.sf!=cpu.eflags.of)
        cpu.eip=_new;
}

make_instr_helper(i)

#undef instr

/*------------------------------*/
#define instr jge

static void do_execute(){
    update_eip();
    if(cpu.eflags.sf==cpu.eflags.of)
        cpu.eip=_new;
}

make_instr_helper(i)
#undef instr

/*-----------------------------*/

#define instr ja

static void do_execute(){
    update_eip();
    if(cpu.eflags.cf==0&&cpu.eflags.zf==0)
        cpu.eip=_new;
}
make_instr_helper(i)
#undef instr

/*-----------------------------*/
#define instr js

static void do_execute(){
    update_eip();
    if(cpu.eflags.sf==1)
        cpu.eip=_new;
}
make_instr_helper(i)
#undef instr

/*----------------------------*/
#define instr jns

static void do_execute(){
    update_eip();
    if(cpu.eflags.sf==0)
        cpu.eip=_new;
}
make_instr_helper(i)
#undef instr

/*----------------------------*/
#define instr jb
static void do_execute(){
     update_eip();
     if(cpu.eflags.cf==1)
         cpu.eip=_new;
}
 make_instr_helper(i)
 #undef instr


/*-----------------------------*/

#define instr jae

static void do_execute(){
    update_eip();
    if(cpu.eflags.cf==0)
        cpu.eip=_new;
}
make_instr_helper(i)
#undef instr
/*-----------------------------*/


#include "cpu/exec/template-end.h"










