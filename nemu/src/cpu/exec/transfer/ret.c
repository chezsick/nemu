#include "cpu/exec/helper.h"
/*
#define DATA_BYTE 1
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ret-template.h"
#undef DATA_BYTE
make_helper_v(ret_i_w)
*/
/* for instruction encoding overloading */

//#define instr ret
make_helper(ret){
	uint32_t imm16=instr_fetch(cpu.eip+1,2);
	if (ops_decoded.is_data_size_16){
		//cpu.eip&=0xffffff00;
		cpu.eip=swaddr_read(cpu.esp,2);
		cpu.esp+=2;
		cpu.eip&=0x0000ffff;
	}
	else{
		//printf("%x\n",cpu.eip);
		cpu.eip=swaddr_read(reg_l(R_ESP),4);
		cpu.esp+=4;
		//printf("%x\n",cpu.eip);
	}
	if (ops_decoded.opcode==0xc2){
		//printf("%x\n",cpu.eip);
		//uint32_t imm16=instr_fetch(cpu.eip+1,2);
		cpu.esp+=imm16;
		//cpu.eip+=2;
		print_asm("ret $0x%x", imm16);
		//return 3;
	}
	else{
		print_asm("ret");
	}
	//printf("%d\n",cpu.eip);
	return 1;
}
/*
make_helper_v(sub_i2rm)
make_helper_v(sub_si2rm)
make_helper_v(sub_r2rm)
make_helper_v(sub_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
*/
