#include "Simulator.h"


Simulator::Simulator()
{
	simulationFile.open("simulation.txt", ios::app);
	initInstSet();
	reset();
}


Simulator::~Simulator()
{
	simulationFile.close();
}

void Simulator::reset()
{
	cycle = 0;
	PreALUBState = 0;
	disassembly = "";
	currentCycleStr = "";
	waitingInst = NULL;
	exedInst = NULL;
	PostMEM = NULL;
	PostALU = NULL;
	PostALUB = NULL;
	registers.clear();
	dataVal.clear();
}
void Simulator::setInstructionAndData(vector<code*> instructions, vector<data*> dataArray)
{

	this->instructions = instructions;
	this->dataArray = dataArray;

	int index = 0;
	for (std::vector<data*>::iterator iter = dataArray.begin(); iter != dataArray.end(); iter++)		//初始化dataVal数组和registers数组
	{
		/*dataVal[index] = (*iter)->dataValue;*/
		dataVal.push_back((*iter)->dataValue);
		index++;
	}
	dataBegAdd = codeBegAdd + 4 * instructions.size();
	dataCount = dataArray.size();
	for (int i = 0; i < 32; i++)
	{
		registers.push_back(0);
		regState.push_back(0);
	}
		
}

void Simulator::initInstSet()
{
	branchSet.insert("J");
	branchSet.insert("JR");
	branchSet.insert("BEQ");
	branchSet.insert("BLTZ");
	branchSet.insert("BGTZ");
	MEMset.insert("SW");
	MEMset.insert("LW");
	ALUBset.insert("SLL");
	ALUBset.insert("SRL");
	ALUBset.insert("SRA");
	ALUBset.insert("MUL");
	ALUset.insert("ADD");
	ALUset.insert("SUB");
}

bool Simulator::checkWrite(int reg)
{
	code* instruction = NULL;
	//queue<code*> tempPreIssue = PreIssue;
	//while (tempPreIssue.size() > 0)
	//{
	//	instruction = tempPreIssue.front();
	//	tempPreIssue.pop();
	//	if (instruction->opcode == "LW" && reg == instruction->rt)
	//		return true;
	//	if (instruction->category == 2 && reg == instruction->rt)
	//		return true;
	//	if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
	//		&& reg == instruction->rd)
	//		return true;
	//}

	queue<code*> tempPreMEM = PreMEM;
	instruction = NULL;
	while (tempPreMEM.size() > 0)
	{
		instruction = tempPreMEM.front();
		tempPreMEM.pop();
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}

	queue<code*> tempPreALU = PreALU;
	instruction = NULL;
	while (tempPreALU.size() > 0)
	{
		instruction = tempPreALU.front();
		tempPreALU.pop();
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}

	queue<code*> tempPreALUB = PreALUB;
	instruction = NULL;
	while (tempPreALUB.size() > 0)
	{
		instruction = tempPreALUB.front();
		tempPreALUB.pop();
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}

	instruction = PostMEM;
	if (instruction != NULL)
	{
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}
	
	instruction = PostALU;
	if (instruction != NULL)
	{
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}

	instruction = PostALUB;
	if (instruction != NULL)
	{
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}
	return false;
}

bool Simulator::checkRead(int reg)
{
	code* instruction = NULL;
	/*queue<code*> tempPreIssue = PreIssue;
	while (tempPreIssue.size() > 0)
	{
		instruction = tempPreIssue.front();
		tempPreIssue.pop();
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA" ) && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}*/

	queue<code*> tempPreMEM = PreMEM;
	instruction = NULL;
	while (tempPreMEM.size() > 0)
	{
		instruction = tempPreMEM.front();
		tempPreMEM.pop();
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}

	queue<code*> tempPreALU = PreALU;
	instruction = NULL;
	while (tempPreALU.size() > 0)
	{
		instruction = tempPreALU.front();
		tempPreALU.pop();
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}

	queue<code*> tempPreALUB = PreALUB;
	instruction = NULL;
	while (tempPreALUB.size() > 0)
	{
		instruction = tempPreALUB.front();
		tempPreALUB.pop();
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}

	instruction = PostMEM;
	if (instruction != NULL)
	{
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}

	instruction = PostALU;
	if (instruction != NULL)
	{
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}

	instruction = PostALUB;
	if (instruction != NULL)
	{
		if (instruction->opcode == "SW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rs)
			return true;
		if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
			return true;
		if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
			return true;
	}
	return false;
}

bool Simulator::RAW(code* instruction)
{

	if (instruction->opcode == "JR" && checkWrite(instruction->rs) == true)
		return true;
	if (instruction->opcode == "BEQ" && (checkWrite(instruction->rs) || checkWrite(instruction->rt)))
		return true;
	if ((instruction->opcode == "BLTZ" || instruction->opcode == "BGTZ") && checkWrite(instruction->rs))
		return true;
	return false;
}

bool Simulator::WAWorWAR(code* instruction)
{
	//%%%ISSUE:For MEM instructions, The load instruction must wait 
	if (instruction->opcode == "LW")
	{
		if (checkWrite(instruction->rt) || checkRead(instruction->rt))
			return true;
		else
			return false;
	}
	if (instruction->category == 2)
	{
		if (checkWrite(instruction->rt) || checkRead(instruction->rt))
			return true;
		else
			return false;
	}
	if (ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
	{
		if (checkWrite(instruction->rd) || checkRead(instruction->rd))
			return true;
		else
			return false;
	}
	return false;
}

void Simulator::updatePC(code* instruction)
{
	string opcode = instruction->opcode;
	int target;
	int rt;
	int rs;
	int offset;

	if (opcode == "J")
	{
		target = instruction->target_instr_index;
		PC = target;
	}
	if (opcode == "JR")
	{
		rs = instruction->rs;
		PC = registers[rs];
	}
	if (opcode == "BEQ")
	{
		rs = instruction->rs;
		rt = instruction->rt;
		offset = instruction->offset;		//这里的offset之前乘过4了

		if (registers[rs] == registers[rt])
			PC = PC + offset + 4;
		else
			PC = PC + 4;
	}
	if (opcode == "BLTZ")
	{
		rs = instruction->rs;
		offset = instruction->offset;		//这里的offset之前乘过4了

		if (registers[rs] <0)
			PC = PC + offset + 4;
		else
			PC = PC + 4;
	}
	if (opcode == "BGTZ")
	{
		rs = instruction->rs;
		offset = instruction->offset;		//这里的offset之前乘过4了

		if (registers[rs] >0)
			PC = PC + offset + 4;
		else
			PC = PC + 4;
	}
}

void Simulator::pipeline()
{
	cycle = 0;
	PC = codeBegAdd;
	//while (instructions.size()!=0)
	//{
	//	cycle++;
	//	if (IF() == 1)
	//		break;
	//	Issue();
	//	MEM();
	//	ALU();
	//	ALUB();
	//	WB();
	//	print();
	//}
	while (instructions.size() != 0)
	{
		cycle++;
		WB();
		ALUB();
		ALU();
		MEM();
		Issue();
		if (IF() == 1)
			break;
		PreALU = finalPreALU;
		PreIssue = finalPreIssue;
		print();
	}

}

int Simulator::IF()
{
	//这里暂时假设只判断branch的RAW
	if (waitingInst != NULL )		//如果有正在等待执行的指令，则只执行正在等待的指令
	{
		if (RAW(waitingInst))
			return 0;
		else
		{
			updatePC(waitingInst);
			exedInst = waitingInst;
			return 0;
		}
	}

	//没有等待的指令
	int lineInFile = (PC - codeBegAdd) / 4;		//文件中的第几行指令，不包括数据
	if (PreIssue.size() < preIssueMaxSize && lineInFile < instructions.size())
	{
		if (branchSet.find(instructions[lineInFile]->opcode) != branchSet.end())		//is branch?
		{
			if (RAW(instructions[lineInFile]))
				waitingInst = instructions[lineInFile];
			else
			{
				updatePC(instructions[lineInFile]);
				exedInst = instructions[lineInFile];
			}	
		}
		else
		{
			if (instructions[lineInFile]->opcode == "BREAK")
				return 1;
			PreIssue.push(instructions[lineInFile]);	//%%%这里需要判断stall
			PC = PC + 4;
			if (PreIssue.size() < preIssueMaxSize && (lineInFile + 1) < instructions.size())
			{
				if (instructions[lineInFile + 1]->opcode == "BREAK")
					return 1;
				PreIssue.push(instructions[lineInFile + 1]);
				PC = PC + 4;
			}
		}
	}
	return 0;
}

void Simulator::Issue()
{
	int index = 2;
	code* instruction = NULL;
	queue<code*> tempPreIssue;
	bool needStall;
	string operaterType;
	finalPreIssue = PreIssue;
	while (index>0 && finalPreIssue.size()>0)
	{
		index--;
		instruction = finalPreIssue.front();
		needStall = false;
		if (MEMset.find(instruction->opcode) != MEMset.end())
		{
			if (PreMEM.size() >= preMEMMaxSize)
				needStall = true;
			else
				operaterType = "MEM";
		}
		if (!needStall && ALUBset.find(instruction->opcode) != ALUBset.end())
		{
			if (PreALUB.size() >= preALUBMaxSize)
				needStall = true;
			else
				operaterType = "ALUB";
		}
		if (!needStall && ALUset.find(instruction->opcode) != ALUset.end())
		{
			if (PreALU.size() >= preALUMaxSize)
				needStall = true;
			else
				operaterType = "ALU";
		}
		if (!needStall && WAWorWAR(instruction))
			needStall = true;

		if (needStall)
			tempPreIssue.push(instruction);
		else
		{
			if (operaterType == "MEM")
				PreMEM.push(instruction);
			if (operaterType == "ALUB")
				PreALUB.push(instruction);
			if (operaterType == "ALU")
				PreALU.push(instruction);
		}

		finalPreIssue.pop();
	}

	while (finalPreIssue.size() > 0)		//把PreIssue中未issue的指令加入到tempPreIssue中保存
	{
		instruction = finalPreIssue.front();
		tempPreIssue.push(instruction);
		finalPreIssue.pop();
	}
	finalPreIssue = tempPreIssue;
}
void Simulator::MEM()
{
	if (PreMEM.size() > 0)
	{
		code* instruction = PreMEM.front();
		if (instruction->opcode == "LW")
			PostMEM = instruction;
		else if (instruction->opcode == "SW")
		{
			int rt = instruction ->rt;
			int offset = instruction ->offset;		//这里的offset之前乘过4了
			int base = instruction->base;
			int dataIndex = (registers[base] + offset - dataBegAdd) / 4;
			dataVal[dataIndex] = registers[rt];
		}
		PreMEM.pop();
	}
}
void Simulator::ALU()
{
	finalPreALU = PreALU;
	if (finalPreALU.size() > 0)
	{
		PostALU = finalPreALU.front();
		finalPreALU.pop();
	}
}
void Simulator::ALUB()
{
	if (PreALUB.size() > 0 )
	{
		PreALUBState++;
		if (PreALUBState == 2)
		{
			PostALUB = PreALUB.front();
			PreALUB.pop();
			PreALUBState = 0;
		}
	}
}
void Simulator::WB()
{
	int rd;
	int rt;
	int rs;
	int sa;
	int imme;
	int target;
	int offset;
	int base;
	string funcode;
	string opcode;
	string instrTxt;
	int category;
	int dataIndex = 0;		//文件中第几个数据
	code* instruction = NULL;

	//MEM
	if (PostMEM != NULL)		//LW
	{
		instruction = PostMEM;
		rt = instruction ->rt;
		offset = instruction->offset;		//这里的offset之前乘过4了
		base = instruction->base;
		dataIndex = (registers[base] + offset - dataBegAdd) / 4;
		registers[rt] = dataVal[dataIndex];
		PostMEM = NULL;
	}

	//ALU
	if (PostALU != NULL)		
	{
		instruction = PostALU;
		category = instruction->category;
		opcode = instruction->opcode;
		if (category == 2)
		{
			rs = instruction->rs;
			rt = instruction->rt;
			rd = instruction->rd;
			imme = instruction->immediate;
			funcode = instruction->functionCode;

			if (funcode == "ADD")
				registers[rt] = registers[rs] + imme;
			if (funcode == "SUB")
				registers[rt] = registers[rs] - imme;
			if (funcode == "AND")
				registers[rt] = registers[rs] & imme;
			if (funcode == "NOR")
				registers[rt] = ~(registers[rs] | imme);
			if (funcode == "SLT")
			{
				if (registers[rs] < registers[rt])
					registers[rd] = 1;
				else
					registers[rd] = 0;
			}
		}
		else if (opcode == "ADD")
		{
			rd = instruction->rd;
			rs = instruction->rs;
			rt = instruction->rt;
			registers[rd] = registers[rs] + registers[rt];
		}
		else if (opcode == "SUB")
		{
			rd = instruction->rd;
			rs = instruction->rs;
			rt = instruction->rt;
			registers[rd] = registers[rs] - registers[rt];
		}
		PostALU = NULL;
	}

	//ALUB
	if (PostALUB != NULL)
	{
		instruction = PostALUB;
		opcode = instruction->opcode;
		if (opcode == "SLL")
		{
			rt = instruction ->rt;
			rd = instruction ->rd;
			sa = instruction ->sa;
			registers[rd] = registers[rt] << sa;
		}
		else if (opcode == "SRL")
		{
			rt = instruction ->rt;
			rd = instruction ->rd;
			sa = instruction ->sa;

			bitset<32> tempBitset = registers[rt];
			bitset<32> tempBitsetR = tempBitset >> sa;
			registers[rd] = tempBitsetR.to_ulong();
		}
		else if (opcode == "SRA")
		{
			rt = instruction ->rt;
			rd = instruction ->rd;
			sa = instruction ->sa;
			registers[rd] = registers[rt] >> sa;
		}
		PostALUB = NULL;
	}
}
void Simulator::print()
{
	queue<code*> tempPreIssue = PreIssue;
	queue<code*> tempPreALU = PreALU;
	queue<code*> tempPreALUB = PreALUB;
	queue<code*> tempPreMEM = PreMEM;

	simulationFile << "--------------------" << '\n';
	simulationFile << "Cycle:" << cycle << '\n';
	simulationFile << '\n';
	simulationFile << "IF Unit:" << '\n';
	simulationFile << '\t' << "Waiting Instruction:";
	if (waitingInst != NULL)
		simulationFile << "["<<waitingInst->instr_text<<"]";
	simulationFile << '\n';
	simulationFile << '\t' << "Executed Instruction:";
	if (exedInst != NULL)
		simulationFile << "[" << exedInst->instr_text<<"]";
	simulationFile << '\n';

	simulationFile << "Pre-Issue Buffer:" << '\n';
	code* instruction;
	for (int i = 0; i<preIssueMaxSize; i++)
	{
		simulationFile << '\t' << "Entry " << i << ":";
		if (tempPreIssue.size() > 0)
		{
			instruction = tempPreIssue.front();
			tempPreIssue.pop();
			simulationFile << "[" << instruction->instr_text<<"]";
		}
		simulationFile << '\n';
	}

	simulationFile << "Pre-ALU Queue:" << '\n';
	for (int i = 0; i<preALUMaxSize; i++)
	{
		simulationFile << '\t' << "Entry " << i << ":";
		if (tempPreALU.size() > 0)
		{
			instruction = tempPreALU.front();
			tempPreALU.pop();
			simulationFile << "[" << instruction->instr_text<<"]";
		}
		simulationFile << '\n';
	}
	simulationFile << "Post-ALU Buffer:";
	if (PostALU != NULL)
		simulationFile << "[" << PostALU->instr_text << "]";
	simulationFile << '\n';

	simulationFile << "Pre-ALUB Queue:" << '\n';
	for (int i = 0; i<preALUBMaxSize; i++)
	{
		simulationFile << '\t' << "Entry " << i << ":";
		if (tempPreALUB.size() > 0)
		{
			instruction = tempPreALUB.front();
			tempPreALUB.pop();
			simulationFile << "[" << instruction->instr_text<<"]";
		}
		simulationFile << '\n';
	}
	simulationFile << "Post-ALUB Buffer:";
	if (PostALUB != NULL)
		simulationFile << "[" << PostALUB->instr_text << "]";
	simulationFile << '\n';

	simulationFile << "Pre-MEM Queue:" << '\n';
	for (int i = 0; i<preMEMMaxSize; i++)
	{
		simulationFile << '\t' << "Entry " << i << ":";
		if (tempPreMEM.size() > 0)
		{
			instruction = tempPreMEM.front();
			tempPreMEM.pop();
			simulationFile << "[" << instruction->instr_text<<"]";
		}
		simulationFile << '\n';
	}
	simulationFile << "Post-MEM Buffer:";
	if (PostMEM != NULL)
		simulationFile << "[" << PostMEM->instr_text << "]";
	simulationFile << '\n';
	
	simulationFile << '\n';
	simulationFile << "Registers" << '\n';
	simulationFile << "R00:";
	for (int i = 0; i < 8; i++)
	{
		simulationFile << '\t' << registers[i];
	}
	simulationFile << '\n';
	simulationFile << "R08:";
	for (int i = 8; i < 16; i++)
	{
		simulationFile << '\t' << registers[i];
	}
	simulationFile << '\n';
	simulationFile << "R16:";
	for (int i = 16; i < 24; i++)
	{
		simulationFile << '\t' << registers[i];
	}
	simulationFile << '\n';
	simulationFile << "R24:";
	for (int i = 24; i < 32; i++)
	{
		simulationFile << '\t' << registers[i];
	}
	simulationFile << '\n';
	simulationFile << '\n';

	simulationFile << "Data" << '\n';
	double temp = (double)dataCount / (double)8;
	for (int i = 0; i < (ceil(temp)) - 1; i++)
	{
		simulationFile << Util::int2string(dataBegAdd + i * 32) << ':';
		for (int j = 0 + 8 * i; j < 8 + 8 * i; j++)
		{
			simulationFile << '\t' << dataVal[j];
		}
		simulationFile << '\n';
	}
	//打印最后一行
	int lastLine = ceil(temp);
	simulationFile << Util::int2string(dataBegAdd + (lastLine - 1) * 32) << ':';
	for (int j = 8 * (lastLine - 1); j < dataCount; j++)
	{
		simulationFile << '\t' << dataVal[j];
	}
	simulationFile << '\n';
	simulationFile.flush();
}
