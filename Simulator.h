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
	vector<int> dataVal;

	int cycle;
	int PC;
	bool canFetchNew;
	string waitingInst;
	string exedInst;
	queue<code*> PreIssue;
	queue<code*> PreMEM;
	queue<code*> PreALU;
	queue<code*> PreALUB;
	code* PostMEM;
	code* PostALU;
	code* PostALUB;
	code* IFWaiting;

	set<string> branchSet;
	ofstream simulationFile;

	void initBranch();
	bool needStall(code* instruction);
	void IF();
	void Issue();
	void MEM();
	void ALU();
	void ALUB();
	void WB();
	void print();
};

