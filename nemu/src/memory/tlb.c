#include <time.h>
#include <stdlib.h>
#define ADDR_SIZE 32
#include "nemu.h"
#include "cpu.h"
#include "mmu.h"

#define BLOCK_WIDTH 6
#define NR_BLOCK 64
#define TAG 20
#define OFFSET 12

typedef union{
    struct{
        uint32_t offset : OFFSET;
        uint32_t tag    : TAG;
    };
    uint32_t addr;
}tlb_addr;

typedef struct{
    PTE pte;
    uint32_t tag;
    bool  valid;
}TLB;

static TLB tlb[NR_BLOCK];

void init_tlb(){
    int i;
    for(i = 0;i < NR_BLOCK; i++)
        tlb[i].valid = false;
}

PTE pte_fetch(lnaddr_t addr);

hwaddr_t tlb_read(lnaddr_t addr){
    tlb_addr temp;
    temp.addr = addr;
    uint32_t offset = temp.offset;
    uint32_t tag = temp.tag;

    int i = 0;
    for(; i < NR_BLOCK; i++ )
    {
        if(tlb[i].valid == 1 && tlb[i].tag == tag)
        {
            return (tlb[i].pte.page_frame << 12) + offset;
        }
    }

    bool flag = 0;
    for(i = 0; i < NR_BLOCK; i++)
        if(tlb[i].valid == 0)
        {
            flag = 1;
            break;
        }
    if(!flag)
    {
        srand(time(0));
        i = rand() % NR_BLOCK;
    }

    tlb[i].valid = 1;
    tlb[i].tag = tag;
    tlb[i].pte = pte_fetch(addr);
    
    return tlb_read(addr);
}

