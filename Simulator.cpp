#include "Simulator.h"


Simulator::Simulator(char* fileName)
{
	simulationFile.open(fileName, ios::app);
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
	waitingInst = NULL;
	exedInst = NULL;
	PostMEM = NULL;
	PostALU = NULL;
	PostALUB = NULL;
	finalPostALU = NULL;
	finalPostMEM = NULL;
	finalPostALUB = NULL;
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
		registers.push_back(0);
		
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

bool Simulator::checkWrite(int reg, int PreIssueIndex)
{
	//%%%最后还是需要添加对之前还未issue的指令的检查
	code* instruction = NULL;
	//与PreIssue中PreIssueIndex之前的指令进行对比检查
	if (PreIssueIndex > 0){
		queue<code*> tempPreIssue;
		//if (PreIssueIndex == preIssueMaxSize)			//如果是IF调用了该函数，则需要比较此次放入PreIssue中的、之前的指令
		//	tempPreIssue = finalPreIssue;
		//else
		tempPreIssue = PreIssue;

		int counter = 0;
		while (counter<PreIssueIndex && tempPreIssue.size()>0)
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
			counter++;
		}
	}

	/*queue<code*> tempPreIssue = PreIssue;
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
	}*/

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

bool Simulator::checkRead(int reg, int PreIssueIndex, bool isLoad)
{
	//%%%最后还是需要添加对之前还未issue的指令的检查
	code* instruction = NULL;
	//与PreIssue中PreIssueIndex之前的指令进行对比检查
	if (PreIssueIndex > 0){
		queue<code*> tempPreIssue = PreIssue;
		int counter = 0;
		while (counter<PreIssueIndex && tempPreIssue.size()>0)
		{
			instruction = tempPreIssue.front();
			tempPreIssue.pop();
			if (instruction->opcode == "SW" && reg == instruction->rt)
			{
				if (isLoad)
				{
					if (Util::findInstrInQueue(instruction, finalPreIssue))
						return true;
				}
				else
					return true;
			}
			if (instruction->category == 2 && reg == instruction->rs)
				return true;
			if ((instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA") && (reg == instruction->rt || reg == instruction->sa))
				return true;
			if ((instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL") && (reg == instruction->rs || reg == instruction->rt))
				return true;
			counter++;
		}
	}
	
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
		{
			if (!isLoad)
				return true;
		}

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

bool Simulator::IFRAW(code* instruction)
{

	if (instruction->opcode == "JR" && checkWrite(instruction->rs, preIssueMaxSize) == true)
		return true;
	if (instruction->opcode == "BEQ" && (checkWrite(instruction->rs, preIssueMaxSize) || checkWrite(instruction->rt, preIssueMaxSize)))
		return true;
	if ((instruction->opcode == "BLTZ" || instruction->opcode == "BGTZ") && checkWrite(instruction->rs, preIssueMaxSize))
		return true;
	return false;
}
bool Simulator::branchIFRAW(code* inst_src, code* inst_obj)
{
	if (inst_src->opcode == "JR" && branchCheckWrite(inst_src->rs, inst_obj) == true)
		return true;
	if (inst_src->opcode == "BEQ" && (branchCheckWrite(inst_src->rs, inst_obj) || branchCheckWrite(inst_src->rt, inst_obj)))
		return true;
	if ((inst_src->opcode == "BLTZ" || inst_src->opcode == "BGTZ") && branchCheckWrite(inst_src->rs, inst_obj))
		return true;
	return false;
}
bool Simulator::IssueRAW(code* instruction, int PreIssueIndex)
{
	if (instruction->opcode == "LW")
	{
		if (checkWrite(instruction->base, PreIssueIndex))
			return true;
		else
			return false;
	}
	if (instruction->opcode == "SW")
	{
		if (checkWrite(instruction->rt, PreIssueIndex) || checkWrite(instruction->base, PreIssueIndex))
			return true;
		else
			return false;
	}
	if (instruction->category == 2)
	{
		if (checkWrite(instruction->rs, PreIssueIndex))
			return true;
		else
			return false;
	}
	if (instruction->opcode == "SLL" || instruction->opcode == "SRL" || instruction->opcode == "SRA")
	{
		if (checkWrite(instruction->rt, PreIssueIndex) || checkWrite(instruction->sa, PreIssueIndex))
			return true;
		else
			return false;
	}
	if (instruction->opcode == "ADD" || instruction->opcode == "SUB" || instruction->opcode == "MUL")
	{
		if (checkWrite(instruction->rs, PreIssueIndex) || checkWrite(instruction->rt, PreIssueIndex))
			return true;
		else
			return false;
	}
	return false;
}

bool Simulator::branchCheckWrite(int reg, code* instr_compare)
{
	code* instruction = instr_compare;
	if (instruction->opcode == "LW" && reg == instruction->rt)
		return true;
	if (instruction->category == 2 && reg == instruction->rt)
		return true;
	if ((ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
		&& reg == instruction->rd)
		return true;
	return false;
}

bool Simulator::WAWorWAR(code* instruction, int PreIssueIndex)
{
	//%%%ISSUE:For MEM instructions, The load instruction must wait 
	if (instruction->opcode == "LW")
	{
		if (checkWrite(instruction->rt, PreIssueIndex) || checkRead(instruction->rt, PreIssueIndex, true))
			return true;
		else
			return false;
	}

	if (instruction->category == 2)
	{
		if (checkWrite(instruction->rt, PreIssueIndex) || checkRead(instruction->rt, PreIssueIndex))
			return true;
		else
			return false;
	}
	if (ALUBset.find(instruction->opcode) != ALUBset.end() || ALUset.find(instruction->opcode) != ALUset.end())
	{
		if (checkWrite(instruction->rd, PreIssueIndex) || checkRead(instruction->rd, PreIssueIndex))
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
	bool isBreak=false;
	while (instructions.size() != 0)
	{
		cycle++;
		WB();
		ALUB();
		ALU();
		MEM();
		Issue();
		if (IF() == 1)
			isBreak=true;
		PreALU = finalPreALU;
		PreALUB = finalPreALUB;
		PreMEM = finalPreMEM;
		PreIssue = finalPreIssue;
		PostALU = finalPostALU;
		PostALUB = finalPostALUB;
		PostMEM = finalPostMEM;
		print();
		if (isBreak)
			break;
	}
	cout << "completed!" << '\n';
}

int Simulator::IF()
{
	//这里暂时假设只判断branch的IFRAW
	if (waitingInst != NULL )		//如果有正在等待执行的指令，则只执行正在等待的指令
	{
		if (IFRAW(waitingInst))
			return 0;
		else
		{
			updatePC(waitingInst);
			exedInst = waitingInst;
			waitingInst = NULL;
			return 0;
		}
	}
	if (exedInst != NULL)
		exedInst = NULL;

	//没有等待的指令
	int lineInFile = (PC - codeBegAdd) / 4;		//文件中的第几行指令，不包括数据
	int counter = 0;
	while (PreIssue.size() + counter < preIssueMaxSize && lineInFile < instructions.size() && counter<2)
	{
		if (branchSet.find(instructions[lineInFile]->opcode) != branchSet.end())		//is branch?
		{
			if (counter == 1 && lineInFile>0 && branchIFRAW(instructions[lineInFile], instructions[lineInFile - 1]))		//如果是fetch的第二条指令、且为branch指令，则另外还需要与第一条fetch的指令进行比较
			{
				waitingInst = instructions[lineInFile];
				break;
			}
			else if (IFRAW(instructions[lineInFile]))
			{
				waitingInst = instructions[lineInFile];
				break;
			}
			else
			{
				updatePC(instructions[lineInFile]);
				exedInst = instructions[lineInFile];
			}
			//if (IFRAW(instructions[lineInFile]))			//不是第二条指令的话，与上个周期中的各个buffer中指令进行比较即可
			//{
			//	waitingInst = instructions[lineInFile];
			//	break;
			//}
			/*else
			{
				updatePC(instructions[lineInFile]);
				exedInst = instructions[lineInFile];
			}
			break;*/
		}
		else
		{
			if (instructions[lineInFile]->opcode == "BREAK")
			{
				exedInst = instructions[lineInFile];
				return 1;
			}
				
			finalPreIssue.push(instructions[lineInFile]);	//%%%这里需要判断stall
			PC = PC + 4;
			lineInFile = lineInFile + 1;
		}
		counter++;
	}
	return 0;
}

void Simulator::Issue()
{
	int entryIndex = 0;
	int counter = 0;
	code* instruction = NULL;
	queue<code*> tempPreIssue;
	bool needStall;
	string operaterType;
	finalPreIssue = PreIssue;
	while (counter<2 && finalPreIssue.size()>0)
	{
		instruction = finalPreIssue.front();
		needStall = false;
		if (MEMset.find(instruction->opcode) != MEMset.end())
		{
			if (PreMEM.size()+counter >= preMEMMaxSize)		//原来PreMEM中的指令数量+新增的指令数量
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
			if (PreALU.size() >= preALUMaxSize)			//这里比较的应该是上个周期结束后的PreALU
				needStall = true;
			else
				operaterType = "ALU";
		}
		if (!needStall && WAWorWAR(instruction, entryIndex))
			needStall = true;
		if (!needStall && IssueRAW(instruction, entryIndex))
			needStall = true;

		if (needStall)
			tempPreIssue.push(instruction);
		else
		{
			if (operaterType == "MEM")
				finalPreMEM.push(instruction);
			if (operaterType == "ALUB")
				finalPreALUB.push(instruction);
			if (operaterType == "ALU")
				finalPreALU.push(instruction);
			counter++;
		}

		finalPreIssue.pop();
		entryIndex++;
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
			finalPostMEM = instruction;
		else if (instruction->opcode == "SW")
		{
			int rt = instruction ->rt;
			int offset = instruction ->offset;		//这里的offset之前乘过4了
			int base = instruction->base;
			int dataIndex = (registers[base] + offset - dataBegAdd) / 4;
			dataVal[dataIndex] = registers[rt];
		}
		finalPreMEM.pop();
	}
}
void Simulator::ALU()
{
	finalPreALU = PreALU;
	if (finalPreALU.size() > 0)
	{
		finalPostALU = finalPreALU.front();
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
			finalPostALUB = PreALUB.front();
			finalPreALUB.pop();
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
		instruction = finalPostMEM;
		rt = instruction ->rt;
		offset = instruction->offset;		//这里的offset之前乘过4了
		base = instruction->base;
		dataIndex = (registers[base] + offset - dataBegAdd) / 4;
		registers[rt] = dataVal[dataIndex];
		finalPostMEM = NULL;
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
		finalPostALU = NULL;
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
		finalPostALUB = NULL;
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
	simulationFile << '\t' << "Waiting Instruction: ";
	if (waitingInst != NULL)
		simulationFile <<waitingInst->instr_text;
	simulationFile << '\n';
	simulationFile << '\t' << "Executed Instruction: ";
	if (exedInst != NULL)
		simulationFile << exedInst->instr_text;
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
