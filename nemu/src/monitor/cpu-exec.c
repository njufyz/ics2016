#include "monitor/monitor.h"
#include "cpu/helper.h"
#include "monitor/watchpoint.h"
#include <setjmp.h>
#include "mmu.h"
#define HAS_DEVICE
/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INSTR_TO_PRINT 10


uint32_t i8259_query_intr();
void i8259_ack_intr();
void raise_intr(uint8_t);


int nemu_state = STOP;
int exec(swaddr_t);

char assembly[80];
char asm_buf[128];

/* Used with exception handling. */
jmp_buf jbuf;

void print_bin_instr(swaddr_t eip, int len) {
	int i;
	int l = sprintf(asm_buf, "%8x:   ", eip);
	for(i = 0; i < len; i ++) {
		l += sprintf(asm_buf + l, "%02x ", instr_fetch(eip + i, 1));
	}
	sprintf(asm_buf + l, "%*.s", 50 - (12 + 3 * len), "");
}

/* This function will be called when an `int3' instruction is being executed. */
void do_int3() {
	printf("\nHit breakpoint at eip = 0x%08x\n", cpu.eip);
	nemu_state = STOP;
}

/* Simulate how the CPU works. */
void cpu_exec(volatile uint32_t n) {
	if(nemu_state == END) {
		printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
		return;
	}
	nemu_state = RUNNING;

#ifdef DEBUG
	volatile uint32_t n_temp = n;
#endif

	setjmp(jbuf);

	for(; n > 0; n --) {
#ifdef DEBUG
		swaddr_t eip_temp = cpu.eip;
		if((n & 0xffff) == 0) {
			/* Output some dots while executing the program. */
			fputc('.', stderr);
		}
#endif

		/* Execute one instruction, including instruction fetch,
		 * instruction decode, and the actual execution. */
		int instr_len = exec(cpu.eip);

		cpu.eip += instr_len;

#ifdef DEBUG
		print_bin_instr(eip_temp, instr_len);
		strcat(asm_buf, assembly);
		Log_write("%s\n", asm_buf);
		if(n_temp < MAX_INSTR_TO_PRINT) {
			printf("%s\n", asm_buf);
		}
#endif

        check_watchpoint();

		if(nemu_state != RUNNING) { return; }
#ifdef HAS_DEVICE
		extern void device_update();
		device_update();
        if(cpu.intr & cpu.eflags.If) {
            uint32_t intr_no = i8259_query_intr();
            i8259_ack_intr();
            raise_intr(intr_no);
        }
#endif
	}

	if(nemu_state == RUNNING) { nemu_state = STOP; }
}

/*  Used for interrupt or expection */
void load_segcache(uint8_t);
void  raise_intr(uint8_t no){
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eflags.val, R_SS);  //push eflags
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.segreg[R_CS].val, R_SS);  //push CS
    
    cpu.esp -= 4;
    swaddr_write(cpu.esp, 4, cpu.eip , R_SS);  //push eip

    uint8_t tmp[8];
    int i= 0;
    for(;i < 8;i++)
        tmp[i] = lnaddr_read(cpu.idtr.base + no * 8 + i, 1);
    GateDesc* gate = (GateDesc*) tmp;

    cpu.segreg[R_CS].val = gate->segment;
    load_segcache(R_CS);
    cpu.eip = (gate->offset_31_16 << 16) + gate->offset_15_0;
    longjmp(jbuf, 1);
}

