#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
WP* new_wp();
void free_wp(int);

/* We use the `readline' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;

	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_p(char *args);

static int cmd_x(char *args);

static int cmd_w(char *args);

static int cmd_d(char *args);

static int cmd_bt();

static int cmd_cache(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
    { "si","Step N instructions exactly.",cmd_si},
    { "info","Display the registers or watchpoints",cmd_info},
    { "p","Display the value of EXPR",cmd_p},
    { "x","Examine memory: x/FMT ADDRESS",cmd_x},
    { "w","When the value of EXPR change, the program will be suspended",cmd_w},
    { "d","Delete the No.N watchpoint",cmd_d},
    { "bt","Print backtrace of all stack frames",cmd_bt},
    { "cache","Print cache block at certain address",cmd_cache}
    /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

static int cmd_si(char *args)
{
    char *arg = strtok(NULL," ");
    int i;

    if(arg == NULL){
        cpu_exec(1);
        return 0;
    }
    else{
        i = atoi(arg);
        if(i<0) 
        {
            puts("Bad Agreement.\n");
            return 0;
        }
        cpu_exec(i);
        return 0;}
}

void display();

static int cmd_info(char *args){
    char *arg = strtok(NULL," ");
    int i;

    if(arg==NULL)
    {
        printf("Bad Argeement.\n");
        return 0;
    }
    if(strcmp(arg,"r")==0){
        for(i=0;i<8;i++)
            printf("%s\t0x%x\t\t%u\n",regsl[i],cpu.gpr[i]._32,cpu.gpr[i]._32);
        for(i=0;i<8;i++)
            printf("%s\t0x%x\t\t%u\n",regsw[i],cpu.gpr[i]._16,cpu.gpr[i]._16);
        for(i=0;i<4;i++)
            printf("%s\t0x%x\t\t%u\n",regsb[i],cpu.gpr[i]._8[0],cpu.gpr[i]._8[0]);
        for(;i<8;i++)
            printf("%s\t0x%x\t\t%2u\n",regsb[i],cpu.gpr[i]._8[1],cpu.gpr[i]._8[1]);

            printf("CF\t0x%x\t\t%2u\n",cpu.eflags.cf,cpu.eflags.cf);
            printf("OF\t0x%x\t\t%2u\n",cpu.eflags.of,cpu.eflags.of);
            printf("SF\t0x%x\t\t%2u\n",cpu.eflags.sf,cpu.eflags.sf);
            printf("ZF\t0x%x\t\t%2u\n",cpu.eflags.zf,cpu.eflags.zf);

    }
    else if(strcmp(arg,"w")==0)
    {
        display();
    }
    else
    {
        puts("Bad Argeement.\n");
        return 0;
    }
    return 0;
}

static int cmd_p(char *args){
   // char* arg= strtok(NULL," ");
    if(args==NULL)
    {
        puts("Bad agreement.");
        return 0;
    }
    bool success = 1;
    unsigned ans = expr(args,&success);
    if(success==1)
    printf("%u\t\t0x%x\n",ans,ans);
    return 0;
}

static int cmd_x(char *args)
{
    char *arg1 = strtok(NULL," ");
    int step = atoi(arg1);
    if(step<1)
    {
        printf("Bad Agreement: %s\n",arg1);
        return 0;
    }

    char *arg2 = strtok(NULL," ");
    int i = 0;
    uint32_t address;
    bool success = 1;
    address = expr(arg2,&success);
    if(success==1)
    {
        printf("\n");;
        for(;i!=step;i++)
        
        {
            printf("0x%x:\t",address+4*i);
            printf("0x%x\n",swaddr_read(address+4*i,4));
        
        }
    }
    return 0;
}

static int cmd_w(char *args)
{
    char* arg=args;
    if(arg==NULL)
    {
        puts("Bad Argeement.");
        return 0;
    }
    bool success = 1;
    uint32_t k =expr(arg,&success);
    if(success==0) return 0;
    WP* wp=new_wp();
    strcpy(wp->expr,arg);
    if(success==1)
    {
        wp->val=k;
        printf("watchpoint %d: expr:%s  value:  %u\n",wp->NO,wp->expr,wp->val);
    }
    else 
        puts("Bad EXPR!");

    return 0;
}

extern WP* head;

static int cmd_d(char *args)
{
    char* arg = strtok(NULL," ");
    if(arg==NULL)
    {
        puts("Bad Agreement.");
        return 0;
    }
    int NO = atoi(arg);
    free_wp(NO);
    return 0;
}

char* get_func(uint32_t addr, uint32_t *readdr);
static int cmd_bt()
{
 uint32_t addr = cpu.eip;
 uint32_t ebp = cpu.ebp;
 uint32_t argv[4]={0,0,0,0};
 char* name = NULL;
 uint32_t readdr = -1;
// int count = 0;
 do{
     name=NULL;
     readdr = -1;
     name = get_func(addr,&readdr);
    argv[0] = swaddr_read(ebp+8,4);
    argv[1] = swaddr_read(ebp+12,4);
    argv[2] = swaddr_read(ebp+16,4);
    argv[3] = swaddr_read(ebp+20,4);
    if(!ebp) name = "_start";
    printf(" %s()  at: 0x%x  argv1: 0x%x, argv2: 0x%x, argv3: 0x%x, argv4: 0x%x\n",name,readdr,argv[0],argv[1],argv[2],argv[3]);
   if(ebp==0) break;
   addr= swaddr_read(ebp + 4,4);
    ebp = swaddr_read(ebp,4);
}while(1);
return 0;
}

void cache_monitor(hwaddr_t addr);
static int cmd_cache(char* args){
    char* arg = strtok(NULL," ");
    if(arg == NULL) {
        puts("Bad Agreement!");
        return 0;
    }
    bool success;
    uint32_t addr = expr(arg,&success);
    if(success==0) {
        puts("Bad Agreement!");
        return 0;
    }
    cache_monitor(addr);
    return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
