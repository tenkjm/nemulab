#include "cpu/reg.h"
#include "exec/helper.h"
#include "cpu/modrm.h"

#include "exec/eflags-help.h"

make_helper(std)
{
	set_df(cpu);
	return 1;
}