#include "common.h"
#include "cpu/reg.h"
uint32_t hwaddr_read(uint32_t,size_t);

hwaddr_t page_translate(lnaddr_t addr){
    if(cpu.cr0.paging == 0) return addr;
    
union Hwaddr{
        uint32_t addr;
        struct{
            uint32_t offset : 12;
            uint32_t page:    10;
            uint32_t dir:     10;
        };
}h;
    h.addr = addr;
    
    uint32_t base = cpu.cr3.page_directory_base;
    uint32_t pagedir = hwaddr_read((base << 12) + h.dir * 4, 4);
    PDE pde;
    pde.val = pagedir;

    //assert present
    assert(pde.present == 1);

    base = pde.page_frame;
    uint32_t pagetab = hwaddr_read((base << 12) + h.page * 4, 4);
    PTE pte;
    pte.val = pagetab;

    //assert present
    assert(pte.present == 1);

    return pte.page_frame + h.offset;
}
