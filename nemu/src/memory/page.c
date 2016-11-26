#include "common.h"
#include "cpu/reg.h"

hwaddr_t page_translate(lnaddr_t addr){
    if(cpu.cr0.paging == 0) return addr;
    else return 0;
}
