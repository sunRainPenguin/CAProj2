//反汇编过程
#pragma once
#include "Util.h"

class instruction
{
public:
	bool isBreak;
	instruction();
	~instruction();

	void reset();
	void setInstruction(string binaryText, int lineNum);
	bool Disassemble();
	void printComplement();
	void returnCode(code* instruction);
	void returnData(data* dataStruc);

private:
	string binaryCode;
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
	//data
	int dataValue;

	ofstream disassemblyFile;

	bool matchCategory2();
	bool matchSpecial();
	bool matchSpecial2();
	bool matchOthers();
	void printFormat();
};

