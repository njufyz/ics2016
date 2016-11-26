#include "common.h"
#include "cpu/reg.h"
uint32_t hwaddr_read(uint32_t,size_t);

hwaddr_t page_translate(lnaddr_t addr){
    if(cpu.cr0.paging == 0 || cpu.cr0.protect_enable == 0) return addr;
    
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

    puts("1111");
    //assert present
    Assert(pde.present == 1, "PDE present invalid! addr: 0x%x dir: 0x%x", addr, h.dir);

    base = pde.page_frame;
    uint32_t pagetab = hwaddr_read((base << 12) + h.page * 4, 4);
    PTE pte;
    pte.val = pagetab;

    //assert present
    Assert(pte.present == 1, "PTE present invalid! addr: 0x%x frame: 0x%x", addr, pde.page_frame);

    return pte.page_frame + h.offset;
}
