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
#define PF cpu.EFLAGS.PF
#define OF cpu.EFLAGS.OF
#define AF cpu.EFLAGS.AF
#define instr cmova
static void do_execute() {
	cmov(CF==0&&ZF==0)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovae
static void do_execute() {
	cmov(CF==0)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovb
static void do_execute() {
	cmov(CF==1)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovbe
static void do_execute() {
	cmov(CF==1||ZF==1)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmove
static void do_execute() {
	cmov(ZF==1)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovc
static void do_execute() {
	cmov(CF==1)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovg
static void do_execute() {
	cmov(ZF==0&&SF==OF)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovge
static void do_execute() {
	cmov(SF==OF)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovl
static void do_execute() {
	cmov(SF!=OF)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovle
static void do_execute() {
	cmov(SF!=OF||ZF==1)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovns
static void do_execute() {
	cmov(SF==0)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovs
static void do_execute() {
	cmov(SF==1)
}
make_instr_helper(rm2r)
#undef instr
#define instr cmovne
static void do_execute() {
        cmov(ZF!=0)
}
make_instr_helper(rm2r)
#undef instr

#include "cpu/exec/template-end.h"
