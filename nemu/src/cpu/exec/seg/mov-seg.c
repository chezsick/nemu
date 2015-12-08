#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include "../../lib-common/x86-inc/mmu.h"

make_helper(mov_cr2r) {
	ModR_M m;
	m.val=instr_fetch(eip+1, 1);
	switch(m.reg){
		case 0: reg_l(m.R_M)= cpu.cr0.val;break;
		
	}
        print_asm("mov\t %%cr%d, %%%s", m.reg, regsl[m.R_M]);
        return 2;
}
make_helper(mov_r2cr) {
	ModR_M m;
	m.val= instr_fetch(eip+1, 1);
	switch(m.reg){
		case 0:cpu.cr0.val=reg_l(m.R_M);break;
		
	}
	print_asm("mov\t %%%s, %%cr%d", regsl[m.R_M], m.reg);
	return 2;
}

make_helper(mov_rm2s){
	/*
	uint32_t modrm = instr_fetch(eip+1, 1);
	int seg_reg= (modrm & 0x38) >> 3;
	int len =1 ;
	decode_rm_w(eip+1);
	cpu.seg[seg_reg].Selector.val = op_src->val;

	set_segcache(seg_reg);
	switch(seg_reg){
		case 0:print_asm("mov $s, CS", op_src->str);break;
		case 1:print_asm("mov $s, DS", op_src->str);break;
		case 2:print_asm("mov $s, ES", op_src->str);break;
		case 3:print_asm("mov $s, SS", op_src->str);break;
	}
	return 1 + len;
	*/
	int len = decode_rm2r_w(eip+1);
	cpu.sreg[op_dest->reg].Selector.val=op_src->val;
	uint16_t index = cpu.sreg[op_dest->reg].Selector.index;
	lnaddr_t addr = cpu.gdtr.base+index*8;
	uint32_t temp[2];
	temp[0] = lnaddr_read(addr, 4);
	temp[1] = lnaddr_read(addr+4, 4);
	SegDesc *dest = (SegDesc *)temp;
	uint32_t base =0, limit=0;
	base += dest->base_31_24 <<24;
	base += dest->base_23_16 <<16;
	base += dest->base_15_0;
	limit +=dest->limit_19_16 << 16;
	limit +=dest->limit_15_0;
	cpu.sreg[op_dest->reg].base = base;
	cpu.sreg[op_dest->reg].limit=limit;
	print_asm("movw""%%%s, %%%s", op_src->str, op_dest->str);
	//set_segcache(seg_reg);
        /*switch(seg_reg){
                case 0:print_asm("mov $s, CS", op_src->str);break;
                case 1:print_asm("mov $s, DS", op_src->str);break;
                case 2:print_asm("mov $s, ES", op_src->str);break;
                case 3:print_asm("mov $s, SS", op_src->str);break;
        }*/
	return len + 1;
}
