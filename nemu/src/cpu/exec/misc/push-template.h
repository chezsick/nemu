#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	cpu.esp-=4;

	print_asm_template2();
}
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
