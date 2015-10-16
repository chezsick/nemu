#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ret-template.h"
#undef DATA_BYTE
make_helper_v(ret_i)
/* for instruction encoding overloading */
#define instr ret
make_helper(ret){
	if (ops_decoded.is_data_size_16){
		cpu.eip&=0xffffff00;
		cpu.eip|=swaddr_read(reg_l(R_ESP),2);
		reg_l(R_ESP)+=2;
		cpu.eip&=0x0000ffff;
	}
	else{
		printf("%x\n",cpu.eip);
		cpu.eip=swaddr_read(reg_l(R_ESP),4);
		reg_l(R_ESP)+=4;
		//printf("%x\n",cpu.eip);
	}
	print_asm("ret");
	//printf("%d\n",cpu.eip);
	return 5;
}
/*
make_helper_v(sub_i2rm)
make_helper_v(sub_si2rm)
make_helper_v(sub_r2rm)
make_helper_v(sub_rm2r)
make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
*/
