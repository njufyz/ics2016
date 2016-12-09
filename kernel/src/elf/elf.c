#include "common.h"
#include "memory.h"
#include <string.h>
#include <elf.h>

#define ELF_OFFSET_IN_DISK 0

#ifdef HAS_DEVICE
void ide_read(uint8_t *, uint32_t, uint32_t);
#else
void ramdisk_read(uint8_t *, uint32_t, uint32_t);
#endif

#define STACK_SIZE (1 << 20)

void create_video_mapping();
void video_mapping_write_test();
void video_mapping_read_test();
void video_mapping_clear();
uint32_t get_ucr3();

uint32_t loader() {
	Elf32_Ehdr *elf;
	Elf32_Phdr *ph = NULL;

	uint8_t buf[4096];
    uint8_t buf_t[ (1<<20) ];
#ifdef HAS_DEVICE
	ide_read(buf, ELF_OFFSET_IN_DISK, 4096);
#else
	ramdisk_read(buf, ELF_OFFSET_IN_DISK, 4096);
#endif

	elf = (void*)buf;

	/* fix the magic number with the correct one */
	const uint32_t elf_magic = 0x464c457f;
	uint32_t *p_magic = (void *)buf;
	nemu_assert(*p_magic == elf_magic);
     ph = (void *)( buf + elf->e_phoff);
   
    /* Load each program segment */
    int i;
	for(i = 0 ; i < elf->e_phnum; i++) {
		/* Scan the program header table, load each segment into memory */
        if(ph->p_type == PT_LOAD) {
			/*  read the content of the segment from the ELF file 
			 * to the memory region [VirtAddr, VirtAddr + FileSiz)
			 */
			 Log("%x",ph->p_vaddr);	
               // uint32_t hwaddr = mm_malloc(ph->p_vaddr, ph->p_memsz);
              //  ide_read((uint8_t *)hwaddr, ph->p_offset, ph->p_filesz);
                /*  zero the memory region 
			 * [VirtAddr + FileSiz, VirtAddr + MemSiz)
			 */
            ide_read(buf_t, ELF_OFFSET_IN_DISK + ph->p_offset, ph->p_filesz);
            uint32_t hwaddr = mm_malloc(ph->p_vaddr, ph->p_memsz);
             memcpy((void *)hwaddr, (void *)(buf_t), ph->p_filesz);
             memset((void*)hwaddr + ph->p_filesz , 0 , ph->p_memsz - ph->p_filesz);  


#ifdef IA32_PAGE
			/* Record the program break for future use. */
			extern uint32_t cur_brk, max_brk;
			uint32_t new_brk = ph->p_vaddr + ph->p_memsz - 1;
			if(cur_brk < new_brk) { max_brk = cur_brk = new_brk; }
#endif
		}
        ph++;
	}
	volatile uint32_t entry = elf->e_entry;

#ifdef IA32_PAGE
	mm_malloc(KOFFSET - STACK_SIZE, STACK_SIZE);

#ifdef HAS_DEVICE
	create_video_mapping();
    video_mapping_clear();
    video_mapping_write_test();
    video_mapping_read_test();
#endif
	write_cr3(get_ucr3());
#endif
	return entry;
}
