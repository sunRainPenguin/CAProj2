//模拟汇编代码执行过程
#pragma once
#include "Util.h"


class Simulator
{
public:
	Simulator(char* fileName);
	~Simulator();

	void reset();
	void setInstructionAndData(vector<code*> instructions, vector<data*> dataArray);
	void pipeline();

private:
	int dataBegAdd;
	int dataCount;
	vector<code*> instructions;
	vector<data*> dataArray;
	vector<int> registers;
	vector<int> dataVal;

	int cycle;
	int PC;
	code* waitingInst;
	code* exedInst;
	queue<code*> PreIssue;
	queue<code*> PreMEM;
	queue<code*> PreALU;
	queue<code*> PreALUB;
	code* PostMEM;
	code* PostALU;
	code* PostALUB;
	queue<code*> finalPreALU;
	queue<code*> finalPreMEM;
	queue<code*> finalPreALUB;
	queue<code*> finalPreIssue;
	code* finalPostALU;
	code* finalPostALUB;
	code* finalPostMEM;
	int PreALUBState;


	set<string> branchSet;
	set<string> MEMset;
	set<string> ALUBset;
	set<string> ALUset;
	ofstream simulationFile;

	void initInstSet();
	bool checkWrite(int reg, int PreIssueIndex=-1);
	bool checkRead(int reg, int PreIssueIndex = -1, bool isLoad=false);
	bool IFRAW(code* instruction);
	bool branchCheckWrite(int reg, code* instr_compare);			//比较两条指令的RAW关系，src为后面fetch的指令，这里暂时只用于比较IF中的两条指令之间的关系，所以暂时只比较第一条为branch的情况
	bool branchIFRAW(code* inst_src, code* inst_obj);				////比较两条指令的RAW关系
	bool IssueRAW(code* instruction, int PreIssueIndex = -1);
	bool WAWorWAR(code* instruction, int PreIssueIndex = -1);
	void updatePC(code* instruction);
	int IF();		//return 1: fetch a BREAK;
	void Issue();
	void MEM();
	void ALU();
	void ALUB();
	void WB();
	void print();
};

