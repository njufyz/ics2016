#include "common.h"
#include "cpu/reg.h"
void load_segcache(uint8_t sreg){
    if(cpu.segcache[sreg].valid==1) return;
    else{
        cpu.segcache[sreg].valid = 1;
        uint64_t m;
        memcpy(&m, (void *)(cpu.gdtr.base + sreg * 8), 8);
        union{
            uint64_t temp1;
            SegDesc temp2;
        }t;
        t.temp1 = m;
        cpu.segcache[sreg].limit = t.temp2.limit_15_0 + (t.temp2.limit_19_16 <<15);
        cpu.segcache[sreg].base = t.temp2.base_15_0 + (t.temp2.base_23_16 << 15) +( t.temp2.base_31_24 <<23);
    }
}
        

lnaddr_t seg_tranlate(swaddr_t addr ,size_t len, uint8_t sreg){
    if(cpu.cr0.protect_enable == 0) return addr;
    else if(cpu.segcache[sreg].valid == 0) {
    load_segcache(sreg);
    return seg_tranlate(addr, len, sreg);}
    else return cpu.segcache[sreg].base + addr;
}







