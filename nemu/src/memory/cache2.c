#include "common.h"
#include "burst.h"
#include "misc.h"
#include "stdlib.h"
#include "time.h"

#define ADDR_SIZE 32
#define HW_MEM_SIZE (1<<27)

#define BLOCK_WIDTH 6
#define WAY_WIDTH 4
#define GROUP_WIDTH 10
#define TAG_WIDTH ( (ADDR_SIZE) - (BLOCK_WIDTH) - (GROUP_WIDTH))

#define cache2_SIZE ( 1 << ((BLOCK_WIDTH) + (WAY_WIDTH) + (GROUP_WIDTH)) )

#define NR_BLOCK ( 1 << (BLOCK_WIDTH) )
#define NR_WAY ( 1 << (WAY_WIDTH) )
#define NR_GROUP ( 1 << (GROUP_WIDTH) )
#define NR_TAG ( 1 << (TAG_WIDTH) )

uint32_t dram_read(hwaddr_t addr, size_t len);
uint32_t dram_write(hwaddr_t addr, size_t len, uint32_t data);
typedef union{
    struct {
        uint32_t block_addr : BLOCK_WIDTH;
        uint32_t group      : GROUP_WIDTH;
        uint32_t tag        : TAG_WIDTH;
    };
    uint32_t addr;
}cache2_addr;

typedef struct{
    uint8_t block[NR_BLOCK];
    uint32_t tag;
    bool valid;
    bool dirty;
} CB2;

CB2 cache2[NR_GROUP][NR_WAY];

void init_cache2(){
    int i, j;
    for(i = 0; i < NR_GROUP; i ++) {
        for(j = 0; j < NR_WAY; j ++) {
            cache2[i][j].valid = false;
        }
    }
}

uint32_t cache2_read(hwaddr_t addr, size_t len){
  Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!   ", addr);
    cache2_addr temp;
    temp.addr = addr;
    uint32_t block_addr = temp.block_addr;
    uint32_t group = temp.group;
    uint32_t tag = temp.tag;

    int i = 0;
    for(; i < NR_WAY ; i++)
    {
        //hit
        if(cache2[group][i].valid == 1 && cache2[group][i].tag == tag)
        {
            if(len + block_addr <= NR_BLOCK)
            {
                uint32_t t;
                memcpy(&t,&cache2[group][i].block[block_addr],len);
                return t;
            }
            else
            {
             //   Log("unaligned");
                uint32_t result, unaligned;
                int l = ( NR_BLOCK - block_addr);
                result = cache2_read(addr + l, 4);
                result <<= (l * 8);
                memcpy(&unaligned, &cache2[group][i].block[block_addr], 4);
               if(l == 1)
                unalign_rw(&result, 1) = unaligned;
               else if(l == 2)
                unalign_rw(&result, 2) = unaligned;
               else if(l == 3)
                unalign_rw(&result, 3) = unaligned;
                return result;
            }
    }
}
        //miss
 //       Log("miss");
        bool flag = 0;
        for(i = 0;i < NR_WAY; i++)
            if(cache2[group][i].valid == 0 ) 
            {
                flag = 1;
                break;
            }
        if(!flag)
        {
            srand(time(0));
            i = rand() % NR_WAY;
        }

        cache2_addr temp2 = temp;
        temp2.block_addr = 0;

        //write back
        if(cache2[group][i].dirty){
            int address = 0;
            for(;address < NR_BLOCK ; address++)
                dram_write(temp2.addr + address, 1, cache2[group][i].block[address]);
        } 
        
        cache2[group][i].tag = tag;
        cache2[group][i].valid = 1;
        cache2[group][i].dirty = 0;
        int j = 0;
        for(; j < NR_BLOCK ; j++){
           cache2[group][i].block[j] = dram_read(temp2.addr + j, 1) ;
        }
        return cache2_read(addr, len);
    }


void cache2_write(hwaddr_t addr ,size_t len, uint32_t data){
    cache2_addr temp;
    temp.addr = addr;
    uint32_t block_addr = temp.block_addr;
    uint32_t group = temp.group;
    uint32_t tag = temp.tag;
    int i = 0;
    for(;i<NR_WAY;i++)
    {
        if(cache2[group][i].valid == 1 && cache2[group][i].tag == tag)
        {
            if(block_addr + len <= NR_BLOCK)
            {
                memcpy(&cache2[group][i].block[block_addr],&data,len);
                cache2[group][i].dirty = 1;
                return;
            }
            else
            {
               int l = NR_BLOCK - block_addr;
               int j = 0;
               for(; j < l ; j++)
              {
                  uint32_t t = data >> (8 * j);
                 cache2[group][i].block[block_addr + j] = unalign_rw( &t, 1) ;
              } 
               cache2[group][i].dirty = 1;
               cache2_write(addr + l, len - l, data >> ( 8 * j) );
               return;
            }

        }
    }
        bool flag = 0;
        for(i = 0;i < NR_WAY; i++)
            if(cache2[group][i].valid == 0 ) 
            {
                flag = 1;
                break;
            }
        if(!flag)
        {
            srand(time(0));
            i = rand() % NR_WAY;
        }

        cache2_addr temp2 = temp;
        temp2.block_addr = 0;

        //write back
        if(cache2[group][i].dirty){
            int address = 0;
            for(;address < NR_BLOCK ; address++)
                dram_write(temp2.addr + address, 1, cache2[group][i].block[address]);
        }
        cache2[group][i].tag = tag;
        cache2[group][i].valid = 1;
        cache2[group][i].dirty = 0;
        int j = 0;
        for(; j < NR_BLOCK ; j++){
           cache2[group][i].block[j] = dram_read(temp2.addr + j, 1) ;
        }
        cache2_write(addr,len,data);
 }


void cache2_monitor(hwaddr_t addr){
    Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!     ", addr);
    cache2_addr temp;
    temp.addr = addr;
    uint32_t block_addr = temp.block_addr;
    uint32_t group = temp.group;
    uint32_t tag = temp.tag;
    
    printf("L2    Block_addr: %x  Group: %x   Tag: %x\n",block_addr,group,tag);
    int i = 0;
    for(; i < NR_WAY ; i++)
    {
        //hit
        if(cache2[group][i].valid == 1 && cache2[group][i].tag == tag)
        {
            puts("Hit in L2!");
            int i = 0;
            for(;i + block_addr <= NR_BLOCK && i < 4; i++)
                printf("Address: 0x%x    Value: 0x%02x\n" ,addr + i, cache2_read(addr +i, 1 )  & 0xff);
            return;
        }
    }
    puts("Miss in L2!");
}
























































