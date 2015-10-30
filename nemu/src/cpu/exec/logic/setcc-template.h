#include "cpu/exec/template-start.h"
/*
make_helper_v(seta_rm);
make_helper_v(setae_rm);
make_helper_v(setb_rm);
make_helper_v(setbe_rm);
make_helper_v(setc_rm);
make_helper_v(sete_rm);
make_helper_v(setg_rm);
make_helper_v(setge_rm);
make_helper_v(setl_rm);
make_helper_v(setle_rm);
make_helper_v(setna_rm);
make_helper_v(setne_rm);
make_helper_v(setno_rm);
make_helper_v(setnp_rm);
make_helper_v(setns_rm);
make_helper_v(seto_rm);
make_helper_v(setp_rm);
make_helper_v(sets_rm);
make_helper_v(setz_rm);
*/
#define CF cpu.EFLAGS.CF
#define ZF cpu.EFLAGS.ZF
#define OF cpu.EFLAGS.OF
#define SF cpu.EFLAGS.SF
#define PF cpu.EFLAGS.PF

#define instr seta
static void do_execute(){
	OPERAND_W(op_dest, (CF==0&&ZF==0) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setae
static void do_execute(){
	OPERAND_W(op_dest, (CF==0) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setb
static void do_execute(){
	OPERAND_W(op_dest, (CF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setbe
static void do_execute(){
	OPERAND_W(op_dest, (ZF==1||CF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setc
static void do_execute(){
	OPERAND_W(op_dest, (CF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr sete
static void do_execute(){
	OPERAND_W(op_dest, (ZF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setg
static void do_execute(){
	OPERAND_W(op_dest, (ZF==0&&SF==OF) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setge
static void do_execute(){
	OPERAND_W(op_dest, (SF==OF) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setl
static void do_execute(){
	OPERAND_W(op_dest, (SF!=OF) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setle
static void do_execute(){
	OPERAND_W(op_dest, (ZF==1||SF!=OF) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setna
static void do_execute(){
	OPERAND_W(op_dest, (CF==1||ZF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr


#define instr setne
static void do_execute(){
	OPERAND_W(op_dest, (ZF==0) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setno
static void do_execute(){
	OPERAND_W(op_dest, (OF==0) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setnp
static void do_execute(){
	OPERAND_W(op_dest, (PF==0) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setns
static void do_execute(){
	OPERAND_W(op_dest, (SF==0) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr


#define instr seto
static void do_execute(){
	OPERAND_W(op_dest, (OF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setp
static void do_execute(){
	OPERAND_W(op_dest, (PF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr sets
static void do_execute(){
	OPERAND_W(op_dest, (SF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr

#define instr setz
static void do_execute(){
	OPERAND_W(op_dest, (ZF==1) ?1:0);
	print_asm_template1();
}
make_instr_helper(rm)
#undef instr
/*
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
*/
//make_instr_helper(i)
//make_instr_helper(m)
#include "cpu/exec/template-end.h"
