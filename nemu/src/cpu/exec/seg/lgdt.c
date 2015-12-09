#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(lgdt_m) {
         swaddr_t addr=instr_fetch(eip+1, 4);
         cpu.gdtr.limit = swaddr_read(addr, 2, R_SS);
         cpu.gdtr.base = swaddr_read(addr+2, 4, R_SS);
         print_asm("lgdt");
         return 6;
}

