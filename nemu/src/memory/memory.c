#include "common.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);
lnaddr_t seg_translate(swaddr_t, size_t, uint8_t);
hwaddr_t page_translate(lnaddr_t);
int is_mmio(hwaddr_t);
uint32_t mmio_read(hwaddr_t, size_t, int);
void mmio_write(hwaddr_t, size_t, uint32_t, int);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
    int no = is_mmio(addr);
    if(no == -1)
        return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
    else 
        return mmio_read(addr, len, no);
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
    int no = is_mmio(addr);
    if(no == -1)
        cache_write(addr, len, data);
    else mmio_write(addr, len, data, no);
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
    if((addr &0xfff) + len > 0x1000){
        int offset1  = 0x1000 - (addr & 0xfff) ;
        uint32_t low = hwaddr_read( page_translate(addr), offset1);
        int offset2 = len - offset1;
        uint32_t high = hwaddr_read( page_translate(addr + offset1), offset2);
        return (high << offset1) + low;
    }
    hwaddr_t hwaddr = page_translate(addr);
    return hwaddr_read(hwaddr, len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
    if((addr &0xfff) + len > 0x1000)
        assert(0);
    hwaddr_t hwaddr = page_translate(addr);
	hwaddr_write(hwaddr, len, data);
}

uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
    return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
    lnaddr_t lnaddr = seg_translate(addr, len, sreg);
	lnaddr_write(lnaddr, len, data);
}

