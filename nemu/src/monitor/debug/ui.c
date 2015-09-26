#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint32_t);
extern CPU_state cpu;
extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];
/* We use the ``readline'' library to provide more flexibility to read from stdin. */
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

static int cmd_si(char *args) {
	if (args==NULL)
		cpu_exec(1);
	else	{
		cpu_exec(atoi(args));
		//printf("%s\t%d\n",args,atoi(args));
	}
	return 0;
}

static int cmd_info(char *args) {
	if (strcmp(args,"r")==0){
		printf("eax\t\t%p\t0x%x\t\t%d\n",&cpu.eax,cpu.eax,cpu.eax);
		printf("ecx\t\t%p\t0x%x\t\t%d\n",&cpu.ecx,cpu.ecx,cpu.ecx);
		printf("edx\t\t%p\t0x%x\t\t%d\n",&cpu.edx,cpu.edx,cpu.edx);
		printf("ebx\t\t%p\t0x%x\t\t%d\n",&cpu.ebx,cpu.ebx,cpu.ebx);
		printf("esp\t\t%p\t0x%x\t\t%d\n",&cpu.esp,cpu.esp,cpu.esp);
		printf("ebp\t\t%p\t0x%x\t\t%d\n",&cpu.ebp,cpu.ebp,cpu.ebp);
		printf("esi\t\t%p\t0x%x\t\t%d\n",&cpu.esi,cpu.esi,cpu.esi);
		printf("edi\t\t%p\t0x%x\t\t%d\n",&cpu.edi,cpu.edi,cpu.edi);
		printf("eip\t\t%p\t0x%x\t\t%d\n",&cpu.eip,cpu.eip,cpu.eip);
		int i;
		for (i=0;i<8;i++)
			printf("%s\t\t%p\t0x%x\t%d\n",regsw[i],&cpu.gpr[i]._16,cpu.gpr[i]._16,cpu.gpr[i]._16);
		for (i=0;i<4;i++){
			printf("%s\t\t%p\t0x%x\t%d\n",regsb[i],&cpu.gpr[i]._8[0],cpu.gpr[i]._8[0],cpu.gpr[i]._8[0]);
			printf("%s\t\t%p\t0x%x\t%d\n",regsb[i+4],&cpu.gpr[i]._8[1],cpu.gpr[i]._8[1],cpu.gpr[i]._8[1]);
		}
	}else
	if (strcmp(args,"w")==0){
		display_wp();
	}
	return 0;
}

static int cmd_x(char *args)
{
	char *num=strtok(args," ");
	int n=atoi(num);
	char *addr=num+strlen(num)+1;
	//printf("****%s\n",addr);
	int addr_int;
	sscanf(addr,"%x",&addr_int);
	while (n>0) {
		printf("%d\n",swaddr_read(addr_int,4));
		addr_int+=4;
		//printf("****%d\n",addr_int);
		n--;
	}

	return 0;
}
static int cmd_p(char *args)
{
	bool success=true;
	//*success=true;
	uint32_t output=expr(args,&success);
	if (success)
		printf("%d\n",output);
	else    
		printf("please try again:\n");
	return 0;
}
static int cmd_w(char *args)
{
	/*init_wp_list();
	bool success=true;
	uint32_t output=expr(args,&success);
	if (success)
		new_wp(args,output);
	else
	      	printf("please try again:\n"); 
	*/
	new_wp(args);
	return 0;		
}

static int cmd_d(char *args)
{
	int no=atoi(args);
	free_wp(no);
	return 0;
}
static int cmd_help(char *args);

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
	{ "si", "Step In", cmd_si},
	{ "info", "Print register state", cmd_info},
	{ "x", "Examine memory", cmd_x},
	{ "p", "Print the value of expression EXP",cmd_p},
	{ "w", "Set a watchpoint for an expression.",cmd_w},
	{ "d", "Delete a watchpoint.",cmd_d}
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

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		init_wp_list();

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
