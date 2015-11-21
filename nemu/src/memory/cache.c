#include "common.h"
#include "burst.h"
#include "misc.h"

//64b 64kb 1024line 
#define TAG_WIDTH 14	//27-7-6
#define BLOCK_WIDTH 6	//64B
#define SLOT_WIDTH 10	//1K line
#define INDEX_WIDTH 7	//128 set
#define WAY 8

#define BLOCK_SIZE (1<<BLOCK_WIDTH)
#define NR_TAG (1<<TAG_WIDTH)
#define NR_SLOT (1<<SLOT_WIDTH)			
#define NR_INDEX (1<<INDEX_WIDTH)

#define HW_MEM_SIZE (1 << 27)

uint32_t dram_read(hwaddr_t, size_t);

typedef struct{
	uint32_t tag	:TAG_WIDTH;
	uint8_t block[BLOCK_SIZE];
	bool valid;

}CacheSlot;

CacheSlot cache[NR_SLOT];

void init_cache() {
	int i;
	for (i=0; i<NR_SLOT; i++){
		cache[i].valid=false;
	}
}

uint32_t set_ass(hwaddr_t addr){
	uint32_t set_index=(addr>>BLOCK_WIDTH)&(NR_INDEX-1);
	uint32_t slot_index=set_index*WAY;
	return slot_index;
}

bool hit(hwaddr_t addr, uint32_t* hit_index){ //if hit return hit address, else return set address
	bool is_hit=false;
	uint32_t index=set_ass(addr);
	*hit_index=index;
	int i;
	uint32_t addr_tag=(addr>>(INDEX_WIDTH+BLOCK_WIDTH))&(NR_TAG-1);
	for (i=0;i<WAY;i++){
		if ((cache[index+i].valid) && (cache[index+i].tag==addr_tag)){
			is_hit=true;
			*hit_index=index+i;
			//Log("hit! in set:%d, No.%d slot,%x", index/WAY, i, addr);
			break;
	 	}
	}
	//if (!is_hit) Log("%x: miss!", addr);
	return is_hit;
}

uint32_t dram2cache(hwaddr_t addr, uint32_t index){
	int i;
	bool rep=true;
	for (i=0; i< WAY; i++){			//have space
		if (cache[index+i].valid==0){
			rep=false;
			index+=i;
			break;
 		}
  	}
 	if (rep) index+=addr&(WAY-1);
	hwaddr_t addr_sta=(addr>>BLOCK_WIDTH)<<BLOCK_WIDTH;
	for (i=0; i<BLOCK_SIZE;  i++){

		cache[index].block[i]=dram_read(addr_sta+i, 1);
		//printf("dram2cache: %x\n", cache[index].block[i]);
	}
	cache[index].tag =(addr>>(INDEX_WIDTH+BLOCK_WIDTH))&(NR_TAG-1);
	cache[index].valid=1;

	return index;
}
uint32_t cache_read(hwaddr_t addr, size_t len){
	//printf("cache read %x(%d)\n", addr, len);
	uint32_t offset = addr & (BLOCK_SIZE-1);
	uint8_t temp[2 * BLOCK_SIZE];
	uint32_t hit_index;
	bool is_hit=hit(addr, &hit_index);
 	if (!is_hit){
		hit_index=dram2cache(addr, hit_index);
	}
	//now the dram block is in cache.
	
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!(in cache)", addr);
	memcpy(temp, cache[hit_index].block, BLOCK_SIZE);
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
		*(temp+BLOCK_SIZE)=cache_read(addr -offset + BLOCK_SIZE, offset + len - BLOCK_SIZE);
	}
	return unalign_rw(temp + offset, 4);
	

}

void cache_write(hwaddr_t addr, size_t len, uint32_t data){
	printf("cache write %x-%x\n", addr, data);
	uint32_t hit_index;
	uint32_t offset = addr & (BLOCK_SIZE-1);
	uint8_t temp[2 * BLOCK_SIZE];
	uint8_t mask[2 * BLOCK_SIZE];
	memset(mask, 0, 2 * BLOCK_SIZE);
	memset(mask + offset, 1, len);
	bool is_hit;
	is_hit=hit(addr, &hit_index);
	if (is_hit==true){
		*(uint32_t *)(temp + offset)=data;
		memcpy_with_mask(cache[hit_index].block, temp, BLOCK_SIZE, mask);
		if (offset + len >BLOCK_SIZE) {
			/* data cross the slot boundary */
			Log("cross the boundary!");
			cache_write(addr - offset + BLOCK_SIZE, offset + len - BLOCK_SIZE, *(uint32_t *)(temp + BLOCK_SIZE));
 		}   
		
 	}
}


