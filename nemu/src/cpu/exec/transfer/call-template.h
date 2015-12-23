#include "cpu/exec/template-start.h"

#define instr call

static void do_execute() {
	if (ops_decoded.opcode==0xe8){
		if (ops_decoded.is_data_size_16){ //n=2;
			cpu.esp-=2;
			MEM_W(cpu.esp,(cpu.eip&0xff)+1+DATA_BYTE, R_SS);
			//if (ops_decoded.opcode==0xe8){
			cpu.eip+=op_src->val&0xffff;	
			//}
			/*if (ops_decoded.opcode==0xff){
				cpu.eip=((op_src->val)&0xffff)-2;
			}*/
		}
		else {
			cpu.esp-=4;
			MEM_W(cpu.esp,cpu.eip+1+DATA_BYTE, R_SS);
			//if (ops_decoded.opcode==0xe8){
			cpu.eip+=op_src->val;
			//}
			/*if (ops_decoded.opcode==0xff){
				cpu.eip=(op_src->val)-2;
			}*/
		}
	}
	if (ops_decoded.opcode==0xff)
	{
		if (ops_decoded.is_data_size_16){
			cpu.esp-=2;
			MEM_W(cpu.esp,(cpu.eip&0xff)+2, R_SS);
			cpu.eip=((op_src->val)&0xffff)-2;
		}
		else{
			cpu.esp-=4;
			Log("esp:%x, oldeip:%x", cpu.esp, cpu.eip);
			MEM_W(cpu.esp,cpu.eip+2, R_SS);
			cpu.eip=(op_src->val)-2;	
			Log("eip:%x, val:%x", cpu.eip, op_src->val);
		}
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)
#include "cpu/exec/template-end.h"
