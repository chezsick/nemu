#include "nemu.h"
#include "../../../lib-common/x86-inc/mmu.h"

#define NR_TLB 8
typedef struct{
	bool valid;
	uint32_t tag;
	PTE pte;
} TLB[NR_TLB];

TLB tlb;

typedef union{
	struct{
		uint32_t offset :12;
		uint32_t page	:10;
		uint32_t dir	:10;
	};
	struct{
		uint32_t	:12;
		uint32_t tag	:20;
	};
	uint32_t val;
}ln_addr;

void init_TLB(){
	int i;
	for (i=0; i<NR_TLB; i++){
		tlb[i].valid = 0;	
	}
}

hwaddr_t page_translate(lnaddr_t addr){
	if ((cpu.cr0.protect_enable==0) || (cpu.cr0.paging==0)) return addr;
	ln_addr lnaddr;
	lnaddr.val= addr;
	int i;
	for (i=0; i< NR_TLB; i++){
		if (tlb[i].valid&&(tlb[i].tag== lnaddr.tag)) break;
		if (!tlb[i].valid) break;
	}
	if (i==NR_TLB) i=addr % NR_TLB;
	if (tlb[i].valid==0){
		hwaddr_t pde_addr =(cpu.cr3.page_directory_base<<12)+lnaddr.dir*sizeof(PDE);
		PDE pde;
		pde.val=hwaddr_read(pde_addr, 4);
		hwaddr_t pte_addr = (pde.page_frame<< 12)+lnaddr.page*sizeof(PTE);
		PTE pte;
		pte.val= hwaddr_read(pte_addr, 4);
		tlb[i].tag= lnaddr.tag;
		tlb[i].valid = true;
		tlb[i].pte=pte;
	}
	return (tlb[i].pte.page_frame<< 12)+ lnaddr.offset;

}
