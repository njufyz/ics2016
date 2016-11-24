#include "common.h"
#include "cpu/reg.h"
uint32_t lnaddr_read(swaddr_t addr, size_t len);

void load_segcache(uint8_t sreg){
    if(cpu.segcache[sreg].valid==1) return;
    else{
        cpu.segcache[sreg].valid = 1;
        uint8_t m[8];
        int i;
        for(i = 0; i< 8; i++)
        {
         m[i] = lnaddr_read(cpu.gdtr.base + cpu.segreg[sreg].index * 8 + i, 1);
        }
        union{
            uint64_t temp1;
            SegDesc temp2;
        }t;
        memcpy(&t.temp1, m, 8);
        /* assert check */
        assert(t.temp2.present == 1);
        assert(t.temp2.limit_15_0 + (t.temp2.limit_19_16 <<16) >= cpu.segreg[sreg].index * 8 );
        /* load seg */
        cpu.segcache[sreg].limit = t.temp2.limit_15_0 + (t.temp2.limit_19_16 <<16);
        cpu.segcache[sreg].base = t.temp2.base_15_0 + (t.temp2.base_23_16 << 16) +( t.temp2.base_31_24 <<24);
        cpu.segcache[sreg].rpl = t.temp2.privilege_level;
    }
}
        

lnaddr_t seg_translate(swaddr_t addr ,size_t len, uint8_t sreg){
    if(cpu.cr0.protect_enable == 0) return addr;
    else if(cpu.segcache[sreg].valid == 0) {
    load_segcache(sreg);
    return seg_translate(addr, len, sreg);
    }
    else{
        return cpu.segcache[sreg].base + addr;
    }
}

void init_CS(){
    cpu.segcache[R_CS].base = 0;
    cpu.segcache[R_CS].limit = 0xffffffff;
}





