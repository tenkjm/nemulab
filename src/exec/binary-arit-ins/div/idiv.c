#include "cpu/reg.h"
#include "exec/helper.h"
#include "cpu/modrm.h"

#include "exec/eflags-help.h"
#include "cpu/interrupt.h"

extern char suffix;

make_helper(idiv_rmb2al)
{
	ModR_M m;
	eflags_help_s val1, val2, result;
	m.val = instr_fetch(eip+1, 1);
	if (m.mod == 3) {
		val1.unsign16 = reg_w(R_AX);
		val2.unsign8 = reg_b(m.R_M);
		idiv_ch_eflags(cpu, 16, 8);
		reg_b(R_AH) = val1.unsign8;
		reg_b(R_AL) = result.unsign8;
		print_asm("idivb %%%s", regsb[m.R_M]);

		if (result.sign8 != result.sign16)
			set_interrupt(0);

		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip+1, &addr);
		val2.unsign8 = swaddr_read(addr, 1);
		val1.unsign8 = reg_w(R_AX);
		idiv_ch_eflags(cpu, 16, 8);
		reg_b(R_AH) = val1.unsign8;
		reg_b(R_AL) = result.unsign8;
		print_asm("idivb %s", ModR_M_asm);

		if (result.sign8 != result.sign16)
			set_interrupt(0);

		return 1+len;
	}
}

make_helper(idiv_rmw2ax)
{
	ModR_M m;
	eflags_help_s val1, val2, result;
	m.val = instr_fetch(eip+1, 1);
	if (m.mod == 3) {
		val1.unsign32 = reg_l(R_EAX);
		val2.unsign16 = reg_w(m.R_M);
		idiv_ch_eflags(cpu, 32, 16);
		reg_w(R_DX) = val1.unsign16;
		reg_w(R_AX) = result.unsign16;
		print_asm("idivw %%%s", regsw[m.R_M]);

		if (result.sign16 != result.sign32)
			set_interrupt(0);

		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip+1, &addr);
		val2.unsign16 = swaddr_read(addr, 2);
		val1.unsign32 = reg_l(R_EAX);
		idiv_ch_eflags(cpu, 32, 16);
		reg_w(R_DX) = val1.unsign16;
		reg_w(R_AX) = result.unsign16;
		print_asm("idivw %s", ModR_M_asm);

		if (result.sign16 != result.sign32)
			set_interrupt(0);

		return 1+len;
	}
}

make_helper(idiv_rml2eax)
{
	ModR_M m;
	eflags_help_s val1, val2, result;
	m.val = instr_fetch(eip+1, 1);
	if (m.mod == 3) {
		val1.unsign64 = reg_l(R_EDX);
		val1.unsign64 <<= 32;
		val1.unsign64 += reg_l(R_EAX);
		val2.unsign32 = reg_l(m.R_M);
		idiv_ch_eflags(cpu, 64, 32);
		reg_l(R_EDX) = val1.unsign32;
		reg_l(R_EAX) = result.unsign32;
		print_asm("idivl %%%s", regsl[m.R_M]);

		if (result.sign32 != result.sign64)
			set_interrupt(0);

		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip+1, &addr);
		val2.unsign32 = swaddr_read(addr, 4);
		val1.unsign64 = reg_l(R_EDX);
		val1.unsign64 <<= 32;
		val1.unsign64 += reg_l(R_EAX);
		idiv_ch_eflags(cpu, 64, 32);
		reg_l(R_EDX) = val1.unsign32;
		reg_l(R_EAX) = result.unsign32;
		print_asm("idivl %s", ModR_M_asm);

		if (result.sign32 != result.sign64)
			set_interrupt(0);

		return 1+len;
	}
}

make_helper(idiv_rmv2av)
{
	return suffix=='l'?idiv_rml2eax(eip):idiv_rmw2ax(eip);
}


