#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute() {
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result);
	DATA_TYPE p=result;
	int i;
	p^=p>>16;
	p^=p>>8;
	p^=p>>4;
	p^=p>>2;
	p^=p>>1;
	if (!!(result>>31))
		cpu.EFLAGS.SF=1;
	else 	
		cpu.EFLAGS.SF=0;
	if (result>op_src2->val)
		cpu.EFLAGS.CF=1;
	else   
		cpu.EFLAGS.CF=0;
	if (result==0) 
		cpu.EFLAGS.ZF=1;
	else 
		cpu.EFLAGS.ZF=0;
	if (p==0)
		cpu.EFLAGS.PF=1;
	else 
		cpu.EFLAGS.PF=0;
	//DATA_TYPE af=0;
	for (i=0;i<4;i++){
		DATA_TYPE a,b;
		a=result>>(i*4)&0xf;
		b=op_dest->val>>(i*4)&0xf;
		if (a>b) 
			cpu.EFLAGS.AF=1;
		else 
			cpu.EFLAGS.AF=0;

	}
	if ((op_src->val>0&&op_dest->val<0&&result>0)||(op_src->val<0&&op_dest->val>0&&result<0))
		cpu.EFLAGS.OF=1;
	else 	
		cpu.EFLAGS.OF=0;
	print_asm_template2();
}

#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
//make_instr_helper(si2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

/*
make_helper(concat(sub_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("sub" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(sub_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("sub" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}*/

#include "cpu/exec/template-end.h"
