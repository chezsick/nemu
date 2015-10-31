#include "cpu/exec/template-start.h"

#define instr movzb

static void do_execute() {
	printf("before:%x\n",op_src->val);
	DATA_TYPE result=0xff&op_src->val;
	//int shift=op_dest->type-op_src->type;
	//result=(op_src<<shift)>>shift;
	//printf("dest:%x\n",op_dest->val);
	printf("res=%x\n",result);
	OPERAND_W(op_dest, result);
	//printf("??%x\n",op_dest->val);
	//printf("addr:%x***%c\n",op_src->addr,result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr movzw
static void do_execute() {
	DATA_TYPE result= 0xffff&op_src->val;
	OPERAND_W(op_dest,result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr
#include "cpu/exec/template-end.h"
