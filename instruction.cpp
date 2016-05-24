#include "instruction.h"


instruction::instruction()
{
	disassemblyFile.open("disassembly.txt", ios::app);
	reset();
}

void instruction::reset()
{
	binaryCode = "";
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
	isBreak = false;
	dataValue = -1;
}
instruction::~instruction()
{
	disassemblyFile.close();
}

void instruction::setInstruction(string binaryText, int lineNum)
{
	binaryCode = binaryText;
	line = lineNum;
}

bool instruction::Disassemble()
{
	//∆•≈‰Category2
	if (matchCategory2()) return true;
	if (matchSpecial()) return true;
	if (matchOthers()) return true;
	if (matchSpecial2()) return true;
	return false;
}

bool instruction::matchCategory2()
{
	//∆•≈‰Category2
	string first6 = binaryCode.substr(0, 6);
	if (first6 != "110000" && first6 != "110001" && first6 != "100001" && first6 != "110010" && first6 != "110011" && first6 != "110101")
		return false;
	else
		category = 2;

	if (first6 == "110000")
		functionCode = "ADD";

	if (first6 == "110001")
		functionCode = "SUB";

	if (first6 == "100001")
		functionCode = "MUL";

	if (first6 == "110010")
		functionCode = "AND";

	if (first6 == "110011")
		functionCode = "NOR";

	if (first6 == "110101")
		functionCode = "SLT";

	opcode = functionCode;
	rs = Util::binaryToDecimal(binaryCode.substr(6, 5));
	rt = Util::binaryToDecimal(binaryCode.substr(11, 5));
	immediate = Util::binaryToDecimal(binaryCode.substr(16, 16));
	printFormat();

	string instrText = functionCode + "\t"+"R" + Util::int2string(rt) + ", R" + Util::int2string(rs) + ", #" + Util::int2string(immediate);
	disassemblyFile << instrText<<'\n';
	this->instr_text = instrText;
	return true;
}

bool instruction::matchSpecial()
{
	if (binaryCode.substr(0, 6) != "000000")
		return false;

	if (binaryCode.substr(0,32) == "00000000000000000000000000000000")
	{
		disassemblyFile << binaryCode << '\t' << line << ' ' << 0<<'\n';
		return true;
	}

	if (binaryCode.substr(26, 6) == "001101")
	{
		opcode = "BREAK";
		printFormat();
		string instrText = opcode;
		disassemblyFile << instrText<<'\n';
		this->instr_text = instrText;
		isBreak = true;
		return true;
	}

	if (binaryCode.substr(11, 10) == "0000000000" && binaryCode.substr(26, 6) == "001000")
	{
		rs = Util::binaryToDecimal(binaryCode.substr(6, 5));
		opcode = "JR";
		printFormat();
		string instrText = opcode + "\t"+"R" + Util::int2string(rs);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;

		return true;
	}

	if (binaryCode.substr(21, 11) == "00000100000" || binaryCode.substr(21, 11) == "00000100010")
	{
		if (binaryCode.substr(21, 11) == "00000100000")
			opcode = "ADD";
		else
			opcode = "SUB";

		rs = Util::binaryToDecimal(binaryCode.substr(6, 5));
		rt = Util::binaryToDecimal(binaryCode.substr(11, 5));
		rd = Util::binaryToDecimal(binaryCode.substr(16, 5));

		printFormat();
		string instrText = opcode + "\t" + "R" + Util::int2string(rd) + ", R" + Util::int2string(rs) + ", R" + Util::int2string(rt);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;
		return true;
	}

	if (binaryCode.substr(6, 5) == "00000" && (binaryCode.substr(26, 6) == "000000" || binaryCode.substr(26, 6) == "000010" || binaryCode.substr(26, 6) == "000011"))
	{
		if (binaryCode.substr(26, 6) == "000000")
			opcode = "SLL";
		else if (binaryCode.substr(26, 6) == "000010")
			opcode = "SRL";
		else
			opcode = "SRA";

		rt = Util::binaryToDecimal(binaryCode.substr(11, 5));
		rd = Util::binaryToDecimal(binaryCode.substr(16, 5));
		sa = Util::binaryToDecimal(binaryCode.substr(21, 5));

		//SLL rd, rt, sa ; SRL rd, rt, sa ; SRA rd, rt, sa
		printFormat();
		string instrText = opcode + "\t"+"R" + Util::int2string(rd) + ", R" + Util::int2string(rt) + ", #" + Util::int2string(sa);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;
		return true;
	}

	return false;
}
bool instruction::matchSpecial2()
{
	if (binaryCode.substr(0, 6) != "011100")
		return false;

	if (binaryCode.substr(21, 11) == "00000000010")
	{
		opcode = "MUL";
		rs = Util::binaryToDecimal(binaryCode.substr(6, 5));
		rt = Util::binaryToDecimal(binaryCode.substr(11, 5));
		rd = Util::binaryToDecimal(binaryCode.substr(16, 5));

		//MUL rd, rs, rt
		printFormat();
		string instrText = opcode + "\t" + "R" + Util::int2string(rd) + ", R" + Util::int2string(rs) + ", R" + Util::int2string(rt);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;
		return true;
	}
	return false;
}

bool instruction::matchOthers()
{
	if (binaryCode.substr(0, 6) == "000010")
	{
		//J target
		opcode = "J";
		target_instr_index = 4 * (Util::binaryToDecimal(binaryCode.substr(6, 26)));
		printFormat();
		string instrText = opcode + "\t" + "#" + Util::int2string(target_instr_index);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;

		return true;
	}
	if (binaryCode.substr(0, 6) == "100011" || binaryCode.substr(0, 6) == "101011")
	{
		if (binaryCode.substr(0, 6) == "100011")
			opcode = "LW";
		else
			opcode = "SW";

		base = Util::binaryToDecimal(binaryCode.substr(6, 5));
		rt = Util::binaryToDecimal(binaryCode.substr(11, 5));
		offset = Util::binaryToDecimal(binaryCode.substr(16, 16));

		//SW rt, offset(base) ; LW rt, offset(base)
		printFormat();
		string instrText = opcode + "\t" + "R" + Util::int2string(rt) + ", " + Util::int2string(offset) + "(R" + Util::int2string(base) + ")";
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;
		return true;
	}
	if (binaryCode.substr(0, 6) == "000100")
	{
		opcode = "BEQ";
		rs = Util::binaryToDecimal(binaryCode.substr(6, 5));
		rt = Util::binaryToDecimal(binaryCode.substr(11, 5));
		offset = (Util::binaryToDecimal(binaryCode.substr(16, 16))) * 4;

		//BEQ rs, rt, offset
		printFormat();
		string instrText = opcode + "\t" + "R" + Util::int2string(rs) + ", R" + Util::int2string(rt) + ", #" + Util::int2string(offset);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;
		return true;
	}
	if (binaryCode.substr(11, 5) == "00000" && (binaryCode.substr(0, 6) == "000001"  || binaryCode.substr(0, 6) == "000111"))
	{
		if (binaryCode.substr(0, 6) == "000001")
			opcode = "BLTZ";
		else
			opcode = "BGTZ";

		rs = Util::binaryToDecimal(binaryCode.substr(6, 5));
		offset = (Util::binaryToDecimal(binaryCode.substr(16, 16))) * 4;

		//BLTZ rs, offset ; BGTZ rs, offset
		printFormat();
		string instrText = opcode + "\t" + "R" + Util::int2string(rs) + ", #" + Util::int2string(offset);
		disassemblyFile << instrText << '\n';
		this->instr_text = instrText;
		return true;
	}
	return false;
}


void instruction::printFormat()
{
	disassemblyFile << binaryCode.substr(0, 6);
	disassemblyFile << " ";
	for (int i = 0; i <= 3; i++)
	{
		disassemblyFile << binaryCode.substr(6 + 5 * i, 5);
		disassemblyFile << " ";
	}
	disassemblyFile << binaryCode.substr(26, 6) << '\t';
	disassemblyFile << line<<'\t';
}
void instruction::printComplement()
{
	string src = binaryCode.substr(0, 32);
	int integer = bitset<maxDigitBitCount>(src).to_ulong();
	disassemblyFile << binaryCode.substr(0,32)<<'\t'<<line;
	disassemblyFile << '\t' << integer<< '\n';

	this->dataValue = integer;
}
void instruction::returnCode(code* instruction)
{
	instruction->base = base;
	instruction->category = category;
	instruction->functionCode = functionCode;
	instruction->immediate = immediate;
	instruction->target_instr_index = target_instr_index;
	instruction->line = line;
	instruction->offset = offset;
	instruction->opcode = opcode;
	instruction->rd = rd;
	instruction->rs = rs;
	instruction->rt = rt;
	instruction->sa = sa;
	instruction->instr_text = instr_text;
}
void instruction::returnData(data* dataStruc)
{
	dataStruc->line = line;
	dataStruc->dataValue = dataValue;
}