#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);

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

static int cmd_bt(char *args);

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
    { "bt","Print backtrace of all stack frames",cmd_bt}
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

static int cmd_info(char *args){
    char *arg = strtok(NULL," ");
    int i;

    if(strcmp(arg,"r")==0){
        for(i=0;i<8;i++)
            printf("%s\t%I32u\n ",regsl[i],cpu.gpr[i]._32);
        for(i=0;i<8;i++)
            printf("%s\t%I16u\n ",regsw[i],cpu.gpr[i]._16);
        for(i=0;i<4;i++)
            printf("%s\t%I8u\n ",regsb[i],cpu.gpr[i]._8[0]);
        for(;i<8;i++)
            printf("%s\t%I8u\n ",regsb[i],cpu.gpr[i]._8[1]);
    }
    else if(strcmp(arg,"w")==0)
    {
        /*TODO*/
    }
    else
    {
        puts("Bad Argeement.\n");
        return 0;
    }
    return 0;
}

static int cmd_p(char *args){
    char* arg= strtok(NULL," ");
    bool success = 1;
    unsigned ans = expr(arg,&success);
    if(success==1)
    printf("%u\n",ans);
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
    /*char *stop;
    unsigned  address = strtol(arg2,&stop,16);
    int i = 0;
    if(0)
    {
        for(;i!=step;i++)
        {
            printf("%x\n ",swaddr_read(address,4));
            address+=4;
        }
            
    }
    else{
        bool success=1;
        expr(arg2,&success);
    }*/
    int i = 0;
    uint32_t address;
    bool success = 1;
    address = expr(arg2,&success);
    if(success==1)
    {
        puts(" ");
        for(;i!=step;i++)
        {
            printf("0x%x\n\t",swaddr_read(address+4*i,4));
        }
    }
    puts(" ");
    return 0;
}

static int cmd_w(char *args)
{
    return -1;
}

static int cmd_d(char *args)
{
    return -1;
}

static int cmd_bt(char *args)
{
    return -1;
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
