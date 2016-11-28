#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "cpu.h"
#include "mmu.h"
enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum { R_ES, R_CS, R_SS, R_DS};
/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */


typedef struct {
    union{
        struct{
            union {
		        uint32_t _32;
		        uint16_t _16;
		        uint8_t _8[2];
	            } gpr[8];
              };

	/* Do NOT change the order of the GPRs' definitions. */
	struct{
    uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
            };
    };

	swaddr_t eip;

    struct Eflags
    {
        unsigned nouse1:8;
        unsigned nouse2:8;
        unsigned nouse3:4;
        unsigned of:1;
        unsigned df:1;
        unsigned If:1;
        unsigned tf:1;
        unsigned sf:1;
        unsigned zf:1;
        unsigned no1:1;
        unsigned af:1;
        unsigned no2:1;
        unsigned pf:1;
        unsigned no3:1;
        unsigned cf:1;
    }eflags;
   
    CR0 cr0;
    CR3 cr3;

    struct GDTR{
        uint32_t base;
        uint16_t limit;
    }gdtr;


 union Selector{
    struct{
        uint16_t rpl :2;
        uint16_t ti  :1;
        uint16_t index:13;
       };
       uint16_t val;
}segreg[4];

    struct Selector_Cache{
        bool valid;
        uint32_t rpl :2;
        uint32_t limit;
        uint32_t base;
    }segcache[4];

} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
extern const char* segregs[];
#endif
