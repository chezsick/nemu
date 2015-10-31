#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute() {
	DATA_TYPE result;
	if (op_src->size==1&&(op_dest->size==2||op_dest->size==4)){
		result = op_dest->val - ((DATA_TYPE_S)(op_src->val)+cpu.EFLAGS.CF);
		//printf("signextended:");
	}
	else{
		result = op_dest->val-(op_src->val+cpu.EFLAGS.CF);
	}
	//printf("%d-%d=%d\n",op_dest->val,op_src->val,result);
	OPERAND_W(op_dest, result);
	
	cpu.EFLAGS.SF=MSB(result);
	cpu.EFLAGS.CF=(result>op_dest->val)?1:0;
	cpu.EFLAGS.ZF=(result==0)?1:0;

	DATA_TYPE p=result;
	p=p&0xff;
	p^=p>>4;
	p^=p>>2;
	p^=p>>1;
	cpu.EFLAGS.PF=(p==0)?1:0;

	uint32_t i;
	for (i=0;i<DATA_BYTE;i++){
		DATA_TYPE a,b;
		a=result>>(i*4)&0xf;
		b=op_dest->val>>(i*4)&0xf;
		cpu.EFLAGS.AF=(a>b)?1:0;

	}

	if (MSB(op_src->val)!=MSB(op_dest->val)&&MSB(result)==MSB(op_src->val))
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
