//模拟汇编代码执行过程
#pragma once
#include "code.h"
#include "data.h"
#include "Util.h"


class Simulator
{
public:
	Simulator();
	~Simulator();

	void reset();
	void setInstructionAndData(vector<code*> instructions, vector<data*> dataArray);
	void pipeline();

private:
	int dataBegAdd;
	int dataCount;
	string currentCycleStr;
	string disassembly;
	vector<code*> instructions;
	vector<data*> dataArray;
	vector<int> registers;
	vector<int> regState;		//0: released; 1: writing; 2: reading
	vector<int> dataVal;

	int cycle;
	int PC;
	code* waitingInst;
	code* exedInst;
	queue<code*> PreIssue;
	queue<code*> PreMEM;
	queue<code*> PreALU;
	queue<code*> PreALUB;
	queue<code*> finalPreALU;
	queue<code*> finalPreIssue;
	int PreALUBState;
	code* PostMEM;
	code* PostALU;
	code* PostALUB;

	set<string> branchSet;
	set<string> MEMset;
	set<string> ALUBset;
	set<string> ALUset;
	ofstream simulationFile;

	void initInstSet();
	bool checkWrite(int reg);
	bool checkRead(int reg);
	bool IFRAW(code* instruction);
	bool IssueRAW(code* instruction);
	bool WAWorWAR(code* instruction);
	void updatePC(code* instruction);
	int IF();		//return 1: fetch a BREAK;
	void Issue();
	void MEM();
	void ALU();
	void ALUB();
	void WB();
	void print();
};

