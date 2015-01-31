#include "cpu/reg.h"
#include "exec/helper.h"
#include "cpu/modrm.h"

#include "exec/eflags-help.h"

extern char suffix;

make_helper(dec_rmb)
{
	eflags_help_s val1, result;
	ModR_M m;
	m.val = instr_fetch(eip+1, 1);
	if (m.mod==3) {
		val1.unsign8 = reg_b(m.reg);
		dec_ch_eflags(cpu, 8, 8);
		reg_b(m.reg) = result.unsign8;

		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip+1, &addr);
		val1.unsign8 = swaddr_read(addr, 1);
		dec_ch_eflags(cpu, 8, 8);
		swaddr_write(addr, 1, result.unsign8);

		return len +1;
	}
}

make_helper(dec_rmw)
{
	eflags_help_s val1, result;
	ModR_M m;
	m.val = instr_fetch(eip+1, 1);
	if (m.mod==3) {
		val1.unsign16 = reg_w(m.reg);
		dec_ch_eflags(cpu, 16, 16);
		reg_w(m.reg) = result.unsign16;

		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip+1, &addr);
		val1.unsign16 = swaddr_read(addr, 2);
		dec_ch_eflags(cpu, 16, 16);
		swaddr_write(addr, 2, result.unsign16);

		return len +1;
	}
}

make_helper(dec_rml)
{
	eflags_help_s val1, result;
	ModR_M m;
	m.val = instr_fetch(eip+1, 1);
	if (m.mod==3) {
		val1.unsign32 = reg_l(m.reg);
		dec_ch_eflags(cpu, 32, 32);
		reg_l(m.reg) = result.unsign32;

		return 2;
	} else {
		swaddr_t addr;
		int len = read_ModR_M(eip+1, &addr);
		val1.unsign32 = swaddr_read(addr, 4);
		dec_ch_eflags(cpu, 32, 32);
		swaddr_write(addr, 4, result.unsign32);

		return len +1;
	}
}

make_helper(dec_rmv)
{
	return suffix=='l'?dec_rml(eip):dec_rmw(eip);
}

make_helper(dec_rw)
{
	eflags_help_s val1, result;
	uint8_t reg = instr_fetch(eip, 1) & 0x07;
	val1.unsign16 = reg_w(reg);
	dec_ch_eflags(cpu, 16, 16);
	reg_w(reg) = result.unsign16;

	return 1;
}

make_helper(dec_rl)
{
	eflags_help_s val1, result;
	uint8_t reg = instr_fetch(eip, 1) & 0x07;
	val1.unsign32 = reg_l(reg);
	dec_ch_eflags(cpu, 32, 32);
	reg_l(reg) = result.unsign32;

	return 1;
}

make_helper(dec_rv)
{
	return suffix=='l'?dec_rl(eip):dec_rw(eip);
}
