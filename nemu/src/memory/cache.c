#include "common.h"
#include "burst.h"
#include "misc.h"
#include "stdlib.h"
#include "time.h"

#define ADDR_SIZE 32
#define HW_MEM_SIZE (1<<27)

#define BLOCK_WIDTH 6
#define WAY_WIDTH 3
#define GROUP_WIDTH 7
#define TAG_WIDTH ( (ADDR_SIZE) - (BLOCK_WIDTH) - (GROUP_WIDTH) )

#define CACHE_SIZE ( 1 << ((BLOCK_WIDTH) + (WAY_WIDTH) + (GROUP_WIDTH)) )

#define NR_BLOCK ( 1 << (BLOCK_WIDTH) )
#define NR_WAY ( 1 << (WAY_WIDTH) )
#define NR_GROUP ( 1 << (GROUP_WIDTH) )
#define NR_TAG ( 1 << (TAG_WIDTH) )

uint32_t dram_read(hwaddr_t addr, size_t len);
typedef union{
    struct {
        uint32_t block_addr : BLOCK_WIDTH;
        uint32_t index      : GROUP_WIDTH;
        uint32_t tag        : TAG_WIDTH;
    };
    uint32_t addr;
}cache_addr;

typedef struct{
    uint8_t block[NR_BLOCK];
    uint32_t tag;
    bool valid;
} CB;

CB cache[NR_GROUP][NR_WAY];

void init_cache(){
    int i, j;
    for(i = 0; i < NR_GROUP; i ++) {
        for(j = 0; j < NR_WAY; j ++) {
            cache[i][j].valid = false;
        }
    }
}

uint32_t cache_read(hwaddr_t addr, size_t len){
   Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!   ", addr);
    cache_addr temp;
    temp.addr = addr;
    uint32_t block_addr = temp.block_addr;
    uint32_t index = temp.index;
    uint32_t tag = temp.tag;

    int i = 0;
    for(; i < NR_WAY ; i++)
    {
        //hit
        if(cache[index][i].valid == 1 && cache[index][i].tag == tag)
        {
            if(len + block_addr <= NR_BLOCK)
            {
                uint32_t t;
                memcpy(&t,&cache[index][i].block[block_addr],len);
                return t;
            }
            else
            {
                //unaligned
                uint32_t result, t;
                uint32_t l =len - (len + block_addr - NR_BLOCK);
              //  uint32_t l2 = len - l;
                result = cache_read(addr + l, 4);
                result <<= (l * 8);
                memcpy(&t, &cache[index][i].block[block_addr], 4);
               if(l == 1)
                unalign_rw(&result, 1) = t;
               else if(l == 2)
                unalign_rw(&result, 2) = t;
               else if(l == 3)
                unalign_rw(&result, 3) = t;
                return result;
            }
        }
    }
        //miss
        Log("miss");
        bool flag = 0;
        for(i = 0;i < NR_WAY; i++)
            if(cache[index][i].valid == 0 ) 
            {
                flag = 1;
                break;
            }
        if(!flag)
        {
            srand(time(0));
            i = rand() % NR_WAY;
        }

        cache_addr temp2 = temp;
        temp2.block_addr = 0;
        cache[index][i].tag = tag;
        cache[index][i].valid = 1;
        int j = 0;
        for(; j < NR_BLOCK ; j++)
            cache[index][i].block[j] = dram_read(temp2.addr + j, 1);
        return cache_read(addr, len);
    }

