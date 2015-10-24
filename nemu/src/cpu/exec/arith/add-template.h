#include "cpu/exec/template-start.h"

#define instr add

static void do_execute() {
	DATA_TYPE result = op_dest->val + op_src->val;
	OPERAND_W(op_dest, result);
	DATA_TYPE p=result;
	uint32_t i;
	p=p&0xff;
	p^=p>>4;
	p^=p>>2;
	p^=p>>1;
	cpu.EFLAGS.SF=MSB(result);
	if (result<op_src->val||result<op_dest->val)
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
	for (i=0;i<DATA_BYTE;i++){
		DATA_TYPE a,b;
		a=result>>(i*4)&0xf;
		b=op_dest->val>>(i*4)&0xf;
		if (a<b) 
			cpu.EFLAGS.AF=1;
		else 
			cpu.EFLAGS.AF=0;

	}
	if ((MSB(op_src->val)==MSB(op_dest->val))&&(MSB(op_src->val)!=MSB(result)))
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

#include "cpu/exec/template-end.h"
