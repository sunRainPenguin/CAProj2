#include "const.h"
#include "instruction.h"
#include "Simulator.h"
#include "code.h"
#include "data.h"


int main(int n, char * arg[])
{
	ifstream binaryFile("sample.txt");		//ifstream binaryFile(arg[1]);
	ofstream disassemblyFile("disassembly.txt", ios::out);
	disassemblyFile.open("disassembly.txt", ios::app);
	ofstream simulationFile("simulation.txt", ios::out);

	char binary[35];
	instruction* instructionLine = new instruction();

	int lineNum = 64;
	bool hasBroken = false;
	vector<code*> instructions;
	vector<data*> dataArray;

	while (binaryFile.getline(binary, 35))
	{
		string binaryStr(binary);
		//disassemblyFile << "Read from file: " << str << endl;
		instructionLine->reset();
		instructionLine->setInstruction(binaryStr, lineNum);
		if (!hasBroken)
		{
			instructionLine->Disassemble();
			code* instruction = new code();
			instructionLine->returnCode(instruction);
			instructions.push_back(instruction);
			if (instructionLine->isBreak == true)
				hasBroken = true;
		}
		else
		{
			instructionLine->printComplement();
			data* dataStruc = new data();
			instructionLine->returnData(dataStruc);
			dataArray.push_back(dataStruc);

		}

		lineNum = lineNum + 4;
	}


	binaryFile.close();
	delete instructionLine;
	disassemblyFile.close();
	simulationFile.close();

	Simulator* simulator = new Simulator();
	simulator->setInstructionAndData(instructions,dataArray);
	simulator->pipeline();

	return 0;
}
