#include "common.h"
#include "cpu/reg.h"
#define IA32_SEG
#define IA32_PAGE
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

uint32_t cache_read(hwaddr_t, size_t);
void cache_write(hwaddr_t, size_t, uint32_t);
//#define IA32_SEG
/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	//return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	return cache_read(addr, len) & (~0u >> ((4 - len) << 3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	cache_write(addr, len, data);
	//dram_write(addr, len, data);
}

lnaddr_t page_translate(lnaddr_t);
uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
#ifdef IA32_PAGE
 	/*if ((addr^(addr+len-1))&(~0xfff)){
		assert(0);
	}
 	else*/{
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr, len);
	
	}
#else
	return hwaddr_read(addr, len);
#endif
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
#ifdef IA32_PAGE
 	if ((addr^(addr+len-1))&(~0xfff)){
		assert(0);
	}
 	else{
		hwaddr_t hwaddr = page_translate(addr);
		hwaddr_write(hwaddr, len, data);
	}
#else
	hwaddr_write(addr, len, data);
#endif
}

lnaddr_t seg_translate(swaddr_t, size_t, uint8_t);
uint32_t swaddr_read(swaddr_t addr, size_t len, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=addr;
//#ifdef IA32_SEG
	lnaddr= seg_translate(addr, len, sreg);
	//assert(0);
//#endif
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data, uint8_t sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr=addr;
//#ifdef IA32_SEG
	lnaddr= seg_translate(addr, len, sreg);
//#endif
	lnaddr_write(lnaddr, len, data);
}

