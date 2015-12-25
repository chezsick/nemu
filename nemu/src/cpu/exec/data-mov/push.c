#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
make_helper_v(push_i)
make_helper_v(push_r)
make_helper_v(push_rm)

void push_l(uint32_t x){
	cpu.esp-=4;
	swaddr_write(cpu.esp, 4, x, R_SS);
}

make_helper(pusha){
	swaddr_t temp = cpu.esp;
	int i;
	for (i = R_EAX; i <= R_EDI; i++){
		//cpu.esp-=4;
		if (i == R_ESP) push_l(temp);
		else push_l(reg_l(i));
		//if (i== R_EBX)
		Log("pusha:%d, addr:%x",i , cpu.esp);
	}
	print_asm("pusha");
	return 1;
}
/*
make_helper_v(sub_si2rm)
make_helper_v(sub_r2rm)
make_helper_v(sub_rm2r)

make_helper_v(mov_a2moffs)
make_helper_v(mov_moffs2a)
*/
