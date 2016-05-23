#include "code.h"


code::code()
{
	reset();
}


code::~code()
{
}
void code::reset()
{
	line = -1;
	category = -1;
	rs = -1;
	rt = -1;
	rd = -1;
	sa = -1;
	base = -1;
	offset = -1;
	opcode = "";
	immediate = -1;
	target_instr_index = -1;
	functionCode = "";
	instr_text = "";
}