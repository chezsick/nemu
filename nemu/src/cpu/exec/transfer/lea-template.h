#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"
#define instr lea

static void do_execute() {
	if (DATA_BYTE==2){
		OPERAND_W(op_dest,(short int)(op_src->addr&0xffff));
	}
	else {
		OPERAND_W(op_dest, op_src->addr);
	}
	print_asm_template2();
}

make_instr_helper(rm2r)
/*

make_helper(concat(lea_rm2r_,SUFFIX)) {
	ModR_M m;
	m.val = instr_fetch(eip+1,1);
	swaddr_t addr;
	int len = read_ModR_M(eip+1, &addr, &addr);
	REG(m.reg) = addr;
	print_asm("lea"str(SUFFIX)"   %s,%%%s",ModR_M_asm,REG_NAME(m.reg));
	return 1+len;
}
*/
#include "cpu/exec/template-end.h"
