#include "Simulator.h"


Simulator::Simulator()
{
	simulationFile.open("simulation.txt", ios::app);
	initBranch();
	reset();
}


Simulator::~Simulator()
{
	simulationFile.close();
}

void Simulator::reset()
{
	cycle = 0;
	canFetchNew = true;
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
		registers.push_back(0);
}

void Simulator::initBranch()
{
	branchSet.insert("J");
	branchSet.insert("JR");
	branchSet.insert("BEQ");
	branchSet.insert("BLTZ");
	branchSet.insert("BGTZ");
}
bool Simulator::needStall(code* instruction)
{

}

void Simulator::pipeline()
{
	cycle = 0;
	PC = codeBegAdd;
	while (instructions.size()!=0)
	{
		cycle++;
		IF();
		Issue();
		
	}

}

void Simulator::IF()
{
	int lineInFile = (PC - codeBegAdd) / 4;		//文件中的第几行指令，不包括数据
	if (PreIssue.size() < 4)
	{
		if (lineInFile < instructions.size())
		{
			PreIssue.push(instructions[lineInFile]);	//%%%这里需要判断stall
			if (branchSet.find(instructions[lineInFile]->opcode)!=branchSet.end())
			{
				
			}
			PC = PC + 4;
		}
		if (PreIssue.size() < 4 && (lineInFile + 1) < instructions.size())
		{
			PreIssue.push(instructions[lineInFile + 1]);
			PC = PC + 4;
		}		
	}

	
}

void Simulator::Issue()
{

}
void Simulator::MEM()
{

}
void Simulator::ALU()
{

}
void Simulator::ALUB()
{

}
void Simulator::WB()
{

}
void Simulator::print()
{

}
