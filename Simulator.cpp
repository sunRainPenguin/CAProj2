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
	queue<code*> tempPreIssue = PreIssue;
	code* instruction = NULL;
	while (tempPreIssue.size() > 0)
	{
		instruction = tempPreIssue.front();
		tempPreIssue.pop();
		if (instruction->opcode == "LW" && reg == instruction->rt)
			return true;
		if (instruction->category == 2 && reg == instruction->rt)
			return true;
		if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
			&& reg == instruction->rd)
			return true;
	}

	queue<code*> tempPreMEM = PreMEM;
	code* instruction = NULL;
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
	code* instruction = NULL;
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
	code* instruction = NULL;
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
	queue<code*> tempPreIssue = PreIssue;
	code* instruction = NULL;
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
	}

	queue<code*> tempPreMEM = PreMEM;
	code* instruction = NULL;
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
	code* instruction = NULL;
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
	code* instruction = NULL;
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
	if (instruction->opcode == "LW" && (checkWrite(instruction->rt) || checkRead(instruction->rt)))
		return true;
	if (instruction->category == 2 && (checkWrite(instruction->rt) || checkRead(instruction->rt)))
		return true;
	if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
		&& (checkWrite(instruction->rd) || checkRead(instruction->rd)))
		return true;

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
	while (instructions.size()!=0)
	{
		cycle++;
		if (IF() == 1)
			break;
		Issue();
		MEM();
		ALU();
		ALUB();
		WB();
	}

}

int Simulator::IF()
{
	int lineInFile = (PC - codeBegAdd) / preIssueMaxSize;		//文件中的第几行指令，不包括数据
	if (PreIssue.size() < preIssueMaxSize && lineInFile < instructions.size() && waitingInst == NULL)
	{
		if (branchSet.find(instructions[lineInFile]->opcode) != branchSet.end())		//is branch?
		{
			if (RAW(instructions[lineInFile]))
				waitingInst = instructions[lineInFile];
			else
				updatePC(instructions[lineInFile]);
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
	while (index>0 && PreIssue.size()>0)
	{
		index--;
		instruction = PreIssue.front();
		needStall = false;
		if (MEMset.find(instruction->opcode) != MEMset.end())
		{
			if (PreMEM.size() >= 2)
				needStall = true;
			else
				operaterType = "MEM";
		}
		if (!needStall && ALUBset.find(instruction->opcode) != ALUBset.end())
		{
			if (PreALUB.size() >= 2)
				needStall = true;
			else
				operaterType = "ALUB";
		}
		if (!needStall && ALUset.find(instruction->opcode) != ALUset.end())
		{
			if (PreALU.size() >= 2)
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

		PreIssue.pop();
	}
	PreIssue = tempPreIssue;
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
	if (PreALU.size() > 0)
	{
		PostALU = PreALU.front;
		PreALU.pop();
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

}
void Simulator::print()
{

}
