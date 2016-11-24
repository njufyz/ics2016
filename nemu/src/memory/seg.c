#include "common.h"
#include "cpu/reg.h"
uint32_t lnaddr_read(swaddr_t addr, size_t len);

void load_segcache(uint8_t sreg){
    if(cpu.segcache[sreg].valid==1) return;
    else{
        Log("sreg:%x",sreg);
        cpu.segcache[sreg].valid = 1;
        uint8_t m[8];
        int i;
        for(i = 0; i< 8; i++)
        {
         m[i] = lnaddr_read(cpu.gdtr.base + cpu.segreg[sreg].index * 8 + i, 1);
        printf("%x index:%d",m[i], cpu.segreg[sreg].index);
        }
            
        SegDesc *temp2 = (SegDesc *)m;
        /* assert check */
       assert(temp2->present == 1);
        assert(temp2->limit_15_0 + (temp2->limit_19_16 <<16) >= cpu.segreg[sreg].index * 8) ;
        /* load seg */
        cpu.segcache[sreg].limit = temp2->limit_15_0 + (temp2->limit_19_16 <<16);
        cpu.segcache[sreg].base = temp2->base_15_0 + (temp2->base_23_16 << 16) +( temp2->base_31_24 <<24);
        cpu.segcache[sreg].rpl = temp2->privilege_level;
    }
}
        

lnaddr_t seg_translate(swaddr_t addr ,size_t len, uint8_t sreg){
    if(cpu.cr0.protect_enable == 0) return addr;
    else if(cpu.segcache[sreg].valid == 0) {
    load_segcache(sreg);
    return cpu.segcache[sreg].base + addr;
    }
    else{
        return cpu.segcache[sreg].base + addr;
    }
}

void init_CS(){
    cpu.segcache[R_CS].base = 0;
    cpu.segcache[R_CS].valid = 1;
    cpu.segcache[R_CS].limit = 0xffffffff;
}





