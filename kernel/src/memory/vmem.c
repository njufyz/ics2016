#include "common.h"
#include "memory.h"
#include <string.h>

#define VMEM_ADDR 0xa0000
#define SCR_SIZE (320 * 200)

/* Use the function to get the start address of user page directory. */
PDE* get_updir();
static PTE uptable[NR_PTE];

void create_video_mapping() {
	/* TODO: create an identical mapping from virtual memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) to physical memory area 
	 * [0xa0000, 0xa0000 + SCR_SIZE) for user program. You may define
	 * some page tables to create this mapping.
    */
    PDE * pdir = get_updir();
    PTE*  ptable = uptable + (VMEM_ADDR >> 12);
    pdir->val = make_pde(va_to_pa(uptable + (VMEM_ADDR >> 12)));
    Log("1 %x",pdir->val);
    uint32_t pfram_addr;
    for(pfram_addr = VMEM_ADDR; pfram_addr <= SCR_SIZE + VMEM_ADDR ; pfram_addr += PAGE_SIZE, ptable ++){
        ptable -> val = make_pte(pfram_addr);
        Log("%x", ptable->val);
    }
}

void video_mapping_write_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		buf[i] = i;
	}
}

void video_mapping_read_test() {
	int i;
	uint32_t *buf = (void *)VMEM_ADDR;
	for(i = 0; i < SCR_SIZE / 4; i ++) {
		assert(buf[i] == i);
	}
}

void video_mapping_clear() {
	memset((void *)VMEM_ADDR, 0, SCR_SIZE);
}

