#include "cpu/exec/template-start.h"

#define instr push

static void do_execute() {
	uint32_t n=4;
	if (ops_decoded.is_data_size_16) n=2; 
	cpu.esp-=n;
	//printf("push:%x(%d)\n",op_src->val,n);
	//MEM_W(cpu.esp,op_src->val);
	swaddr_write(cpu.esp,n,op_src->val, R_SS);
	//printf("+%x\n",MEM_R(cpu.esp));
	print_asm_template1();
}
make_instr_helper(i)
make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"
