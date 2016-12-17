#include "nemu.h"
#include "cpu.h"
#include "mmu.h"

static uint32_t mem_cr3 = 0;

uint32_t hwaddr_read(uint32_t,size_t);

static union Hwaddr{
        uint32_t addr;
        struct{
            uint32_t offset : 12;
            uint32_t page:    10;
            uint32_t dir:     10;
        };
}h, t;

hwaddr_t tlb_read(lnaddr_t addr);
void init_tlb();

PTE pte_fetch(lnaddr_t addr){
    h.addr = addr;
    
    uint32_t base = cpu.cr3.page_directory_base;
    uint32_t pagedir = hwaddr_read((base << 12) + h.dir * 4, 4);
    PDE pde;
    pde.val = pagedir;

    //assert present
    Assert(pde.present == 1, "PDE present invalid! addr: 0x%x dir: 0x%x val: 0x%x", addr, h.dir, pde.val);

    base = pde.page_frame;
    uint32_t pagetab = hwaddr_read((base << 12) + h.page * 4, 4);
    PTE pte;
    pte.val = pagetab;

    //assert present
//    Assert(pte.present == 1, "PTE present invalid! addr: 0x%x frame: 0x%x val: %x", addr, pde.page_frame, pte.val);

    return pte;
}

hwaddr_t page_translate(lnaddr_t addr){
    if(cpu.cr0.protect_enable == 0 || cpu.cr0.paging == 0) 
        return addr;
    if(mem_cr3 != cpu.cr3.val)
    {
        init_tlb();
        mem_cr3 = cpu.cr3.val;
    }
    return tlb_read(addr);
}

void page_monitor(uint32_t addr){
    t.addr = addr;
    
    uint32_t base = cpu.cr3.page_directory_base;
    printf("page_dirrctory_base: %x\n", base);

    uint32_t pagedir = hwaddr_read((base << 12) + h.dir * 4, 4);
    PDE pde;
    pde.val = pagedir;

    Assert(pde.present == 1, "PDE present invalid! addr: 0x%x dir: 0x%x", addr, h.dir);
    printf("dir_entry: %x\n",pde.page_frame);
    
    base = pde.page_frame;
    uint32_t pagetab = hwaddr_read((base << 12) + h.page * 4, 4);
    PTE pte;
    pte.val = pagetab;

    Assert(pte.present == 1, "PTE present invalid! addr: 0x%x frame: 0x%x", addr, pde.page_frame);
    printf("page_frame: %x\n",pte.page_frame);
    printf("hwaddr: %x\n",(pte.page_frame << 12) + t.offset);
}
