#include "common.h"
#include "burst.h"
#include "misc.h"
#include "memory/cache.h"

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

#define L2
#ifdef L1
uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
#endif
#ifdef L2
uint32_t L2cache_read(hwaddr_t, size_t);
void L2cache_write(hwaddr_t, size_t, uint32_t);
#endif
struct{
	uint32_t Hit;
	uint32_t Miss;
}hit_rate;

void print_hit_rate(){
	printf("hit:%d,\tmiss:%d,\ttotal time:%ld\n", hit_rate.Hit, hit_rate.Miss, (long int)(hit_rate.Hit*2+hit_rate.Miss*100));
	printf("rate:%f\n", (double)hit_rate.Hit/(hit_rate.Hit+hit_rate.Miss));
}
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
	hit_rate.Hit=0;
	hit_rate.Miss=0;
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
	if (is_hit) hit_rate.Hit++;else hit_rate.Miss++;
	return is_hit;
}
void print_hit(hwaddr_t addr){
	uint32_t index;
	if (hit(addr,&index)){
		printf("hit! in set:%d, No.%d slot,\t0x%x,\toffset:%d\n", index/WAY, index%WAY, addr,addr&(BLOCK_SIZE-1));
		printf("tag:%x\n",cache[index].tag);
		int i;
		for (i=0;i<BLOCK_SIZE;i++){
			printf("%0x ",cache[index].block[i]);
			if ((i+1)%16==0) printf("\n");
		}
	}
	else {
		printf("Address 0x%0x miss!\n", addr);
	}
}
#ifdef L1
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
	for (i=0; i<BLOCK_SIZE;  i+=4){

		*(uint32_t *)(cache[index].block+i)=dram_read(addr_sta+i, 4);
		//printf("dram2cache: %x\n", cache[index].block[i]);
	}
	cache[index].tag =(addr>>(INDEX_WIDTH+BLOCK_WIDTH))&(NR_TAG-1);
	cache[index].valid=1;

	return index;
}
#endif
#ifdef L2
uint32_t L2cache2cache(hwaddr_t addr, uint32_t index){
	 int i;
	 bool rep=true;
	 for (i=0; i< WAY; i++){                 //have space
		 if (cache[index+i].valid==0){
			 rep=false;
			 index+=i;
			 break;
		 }
	 }
	 if (rep) index+=addr&(WAY-1);
	 hwaddr_t addr_sta=(addr>>BLOCK_WIDTH)<<BLOCK_WIDTH;
	  for (i=0; i<BLOCK_SIZE;  i+=4){
		 *(uint32_t *)(cache[index].block+i)=L2cache_read(addr_sta+i, 4);
	 }
	 cache[index].tag =(addr>>(INDEX_WIDTH+BLOCK_WIDTH))&(NR_TAG-1);
	 cache[index].valid=1;
	  return index;
}
#endif
uint32_t cache_read(hwaddr_t addr, size_t len){
	//printf("cache read %x(%d)\n", addr, len);
	uint32_t offset = addr & (BLOCK_SIZE-1);
	uint8_t temp[2 * BLOCK_SIZE];
	uint32_t hit_index;
	bool is_hit=hit(addr, &hit_index);
 	if (!is_hit){
#ifdef L1
		hit_index=dram2cache(addr, hit_index);
#endif
#ifdef L2
		hit_index=L2cache2cache(addr, hit_index);
#endif
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
		//Log("cross the boundary!");
		*(uint32_t *)(temp+BLOCK_SIZE)=cache_read(addr -offset + BLOCK_SIZE, offset + len - BLOCK_SIZE);
	}
	return unalign_rw(temp + offset, 4);
	

}

void cache_write(hwaddr_t addr, size_t len, uint32_t data){
	//printf("cache write %x-%x\n", addr, data);
	uint32_t hit_index;
	uint32_t offset = addr & (BLOCK_SIZE-1);
	uint8_t temp[2 * BLOCK_SIZE];
	uint8_t mask[2 * BLOCK_SIZE];
	memset(mask, 0, 2 * BLOCK_SIZE);
	memset(mask + offset, 1, len);
	bool is_hit;
	Assert(addr < HW_MEM_SIZE, "physical address %x is outside of the physical memory!(in cache)", addr);
	is_hit=hit(addr, &hit_index);
	if (is_hit==true){
		*(uint32_t *)(temp + offset)=data;
		if(hit_index==74*8||addr==0x101280) printf("fuck here change eip!!!\n");
		memcpy_with_mask(cache[hit_index].block, temp, BLOCK_SIZE, mask);
		if (offset + len >BLOCK_SIZE) {
			/* data cross the slot boundary */
			//Log("cross the boundary!");
			cache_write(addr - offset + BLOCK_SIZE, offset + len - BLOCK_SIZE, *(uint32_t *)(temp + BLOCK_SIZE));
 		}   
		
 	}
#ifdef	L1
	dram_write(addr, len, data);
#endif
#ifdef L2
	L2cache_write(addr, len, data);
#endif
}


