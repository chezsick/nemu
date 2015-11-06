#include "cpu/exec/template-start.h"

#define cmov(name) {\
	 if (name){ \
	 	OPERAND_W(op_dest, op_src->val); \
	 } \
	print_asm_template2(); \
}

#define CF cpu.EFLAGS.CF
#define ZF cpu.EFLAGS.ZF
#define SF cpu.EFLAGS.SF

#define instr cmova
static void do_execute() {
	cmov(CF==0&&ZF==0)
}
make_instr_helper(rm2r)
#undef instr

#define instr cmovs
static void do_execute() {
	cmov(SF==1)
	/*if (SF==1){
		OPERAND_W(op_dest, op_src->val);
	}
	print_asm_template2();
	*/
}
make_instr_helper(rm2r)
#undef instr
/*
#define instr ja
static void do_execute() {
	if (cpu.EFLAGS.ZF==0&&cpu.EFLAGS.CF==0){	
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jae
static void do_execute() {
	if (cpu.EFLAGS.CF==0){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jb
static void do_execute() {
	if (cpu.EFLAGS.CF==1){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jbe
static void do_execute() {
	if (cpu.EFLAGS.ZF==1||cpu.EFLAGS.CF==1){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jg
static void do_execute() {
	if (cpu.EFLAGS.ZF==0&&(cpu.EFLAGS.SF==cpu.EFLAGS.OF)){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jge
static void do_execute() {
	if (cpu.EFLAGS.SF==cpu.EFLAGS.OF){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jl
static void do_execute() {
	if (cpu.EFLAGS.SF!=cpu.EFLAGS.OF){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jle
static void do_execute() {
	if (cpu.EFLAGS.ZF==1||(cpu.EFLAGS.SF!=cpu.EFLAGS.OF)){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jne
static void do_execute(){
	if (cpu.EFLAGS.ZF==0){
		cpu.eip+=sign_ext(op_src->val);	
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr

#define instr jns
static void do_execute(){
	if (cpu.EFLAGS.SF==0){
		cpu.eip+=sign_ext(op_src->val);
		if (ops_decoded.is_data_size_16)
			cpu.eip&=0x0000ffff;
	}
	print_asm_template1();
}
make_instr_helper(i)
#undef instr
*/
//make_instr_helper(i)
//make_instr_helper(m)
#include "cpu/exec/template-end.h"
