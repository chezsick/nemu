#include "nemu.h"
#include "../../../lib-common/x86-inc/mmu.h"

lnaddr_t seg_translate(swaddr_t addr, size_t len, uint8_t sr_no){
	
	if (cpu.cr0.protect_enable==0) return addr;
	if (!cpu.sreg[sr_no].valid) return cpu.sreg[sr_no].base;
	SegDesc sd;
	int i;
	for (i=0; i< sizeof(SegDesc); i++){
		*((uint8_t *)(&sd)+i)= lnaddr_read(cpu.gdtr.base+cpu.sreg[sr_no].Selector.index*sizeof(SegDesc)+i, 1);
		
	}
	cpu.sreg[sr_no].base= (sd.base_31_24<<24)|(sd.base_23_16<<16)|sd.base_15_0;
	cpu.sreg[sr_no].limit=(sd.limit_19_16<<16)|sd.limit_15_0;
	cpu.sreg[sr_no].valid=true;
	return cpu.sreg[sr_no].base + addr;
	/*
	
	uint16_t select = cpu.sreg[sr_no].Selector.index;
	uint32_t buf[2];
	lnaddr_t temp = cpu.gdtr.base + select*8;
	printf("")
	buf[0]=lnaddr_read(temp, 4);
	buf[1]=lnaddr_read(temp+4, 4);
	SegDesc *des =(SegDesc *)buf;
	lnaddr_t ln_addr;
	uint32_t offset = 0;
	//offset = (des->base_31_24<<24) | (des->base_23_16 << 16) | des->base_15_0;
	ln_addr=addr + offset;
	return ln_addr;
	*/

}
