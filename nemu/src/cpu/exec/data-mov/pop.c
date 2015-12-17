#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */
//make_helper_v(push_i)
make_helper_v(pop_r)
make_helper_v(pop_rm)

uint32_t pop_l(){
	uint32_t re=swaddr_read(cpu.esp, 4, R_SS);
	cpu.esp+=4;
	return re;
}
make_helper(popa){
	int i;
	for (i = R_EAX; i <= R_EDI; i++){
		if (i == R_ESP) pop_l();
		else reg_l(i)= pop_l();
	}
	print_asm("popa"str(SUFFIX));
	return 1;
}
