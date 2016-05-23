//表示汇编指令
#pragma once
#include "const.h"

class code
{
public:
	code();
	~code();
	void reset();

	int line;
	int category;			//1:2:-1
	int rs;
	int rt;
	int rd;
	int sa;
	int base;
	int offset;
	string opcode;
	int immediate;
	int target_instr_index;
	string functionCode;
	string instr_text;

};

