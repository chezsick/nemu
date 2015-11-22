#include "common.h"
#include "burst.h"
#include "misc.h"

//64b 4M 64kline 
#define TAG_WIDTH 9	//27-12-6
#define BLOCK_WIDTH 6	//64B
#define SLOT_WIDTH 16	//64k line
#define INDEX_WIDTH 12	//4k set
#define WAY 16

#define BLOCK_SIZE (1<<BLOCK_WIDTH)
#define NR_TAG (1<<TAG_WIDTH)
#define NR_SLOT (1<<SLOT_WIDTH)			
#define NR_INDEX (1<<INDEX_WIDTH)

#define HW_MEM_SIZE (1 << 27)

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);

typedef struct{
	uint32_t tag	:TAG_WIDTH;
	uint8_t block[BLOCK_SIZE];
	bool valid;
	bool dirty;
}L2CacheSlot;

L2CacheSlot L2cache[NR_SLOT];

void init_L2cache() {
	int i;
	for (i=0; i<NR_SLOT; i++){
		L2cache[i].valid=false;
		L2cache[i].dirty=false;
	}
}

uint32_t L2_set_ass(hwaddr_t addr){
	uint32_t set_index=(addr>>BLOCK_WIDTH)&(NR_INDEX-1);
	uint32_t slot_index=set_index*WAY;
	return slot_index;
}

bool L2hit(hwaddr_t addr, uint32_t* hit_index){ //if hit return hit address, else return set address
	bool is_hit=false;
	uint32_t index=L2_set_ass(addr);
	*hit_index=index;
	int i;
	uint32_t addr_tag=(addr>>(INDEX_WIDTH+BLOCK_WIDTH))&(NR_TAG-1);
	for (i=0;i<WAY;i++){
		if ((L2cache[index+i].valid) && (L2cache[index+i].tag==addr_tag)){
			is_hit=true;
			*hit_index=index+i;
			//Log("L2 hit in set:%d, No.%d slot,%x", index/WAY, i, addr);
			break;
	 	}
	}
	//if (!is_hit) Log("%x: L2miss!", addr);
	return is_hit;
}

uint32_t dram2L2cache(hwaddr_t addr, uint32_t index){
	int i;
	bool rep=true;
	for (i=0; i< WAY; i++){			//have space
		if (L2cache[index+i].valid==0){
			rep=false;
			index+=i;
			break;
 		}
  	}
 	
	if (rep) index+=addr&(WAY-1);
	if (rep&&L2cache[index].dirty){		//dirty write back
		Log("dirty back.\n");
		hwaddr_t addr_rb=(L2cache[index].tag<<(INDEX_WIDTH))+index/WAY;
		addr_rb=addr_rb<<BLOCK_WIDTH;
		for (i=0; i<BLOCK_SIZE; i++){
			dram_write(addr_rb, 1, L2cache[index].block[i]);
		}
	}
	hwaddr_t addr_sta=(addr>>BLOCK_WIDTH)<<BLOCK_WIDTH;
	for (i=0; i<BLOCK_SIZE;  i++){

		L2cache[index].block[i]=dram_read(addr_sta+i, 1);
		//printf("dram2L2cache: %x\n", cache[index].block[i]);
	}
	L2cache[index].tag =(addr>>(INDEX_WIDTH+BLOCK_WIDTH))&(NR_TAG-1);
	L2cache[index].valid=1;
	//L2cache[index].dirty=0;
	return index;
}
uint32_t L2cache_read(hwaddr_t addr, size_t len){
	//printf("L2cache read %x(%d)\n", addr, len);
	uint32_t offset = addr & (BLOCK_SIZE-1);
	uint8_t temp[2 * BLOCK_SIZE];
	uint32_t hit_index;
	bool is_hit=L2hit(addr, &hit_index);
 	if (!is_hit){
		hit_index=dram2L2cache(addr, hit_index);
	}
	//now the dram block is in L2cache.
	
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!(in L2cache)", addr);
	memcpy(temp, L2cache[hit_index].block, BLOCK_SIZE);
	/*
	int i;
	printf("temp:");
	for (i=0;i<2*BLOCK_SIZE;i++){
		printf("%0x",temp[i]&0xff);
	}
	printf("\n");
 	*/
 	if (offset + len > BLOCK_SIZE) {
		/* data cross the slot boundary */
		Log("cross the boundary!");
		*(uint32_t *)(temp+BLOCK_SIZE)=L2cache_read(addr -offset + BLOCK_SIZE, offset + len - BLOCK_SIZE);
	}
	return unalign_rw(temp + offset, 4);
	

}

void L2cache_write(hwaddr_t addr, size_t len, uint32_t data){
	//printf("L2cache write %x-%x\n", addr, data);
	uint32_t hit_index;
	uint32_t offset = addr & (BLOCK_SIZE-1);
	uint8_t temp[2 * BLOCK_SIZE];
	uint8_t mask[2 * BLOCK_SIZE];
	memset(mask, 0, 2 * BLOCK_SIZE);
	memset(mask + offset, 1, len);
	bool is_hit;
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!(in L2cache)", addr);
	is_hit=L2hit(addr, &hit_index);
	if (!is_hit==true){
		dram_write(addr, len, data); 			//miss
		L2cache_read(addr, len);
	}
	*(uint32_t *)(temp + offset)=data;
	memcpy_with_mask(L2cache[hit_index].block, temp, BLOCK_SIZE, mask);
	L2cache[hit_index].dirty=1;
	if (offset + len >BLOCK_SIZE) {
		/* data cross the slot boundary */
		Log("cross the boundary!");
		L2cache_write(addr - offset + BLOCK_SIZE, offset + len - BLOCK_SIZE, *(uint32_t *)(temp + BLOCK_SIZE));
 	}   
	//dram_write(addr, len, data);		
 
}


