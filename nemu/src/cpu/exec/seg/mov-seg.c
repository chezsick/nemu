#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

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

