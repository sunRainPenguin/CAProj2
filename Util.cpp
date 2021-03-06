#include "Util.h"


Util::Util()
{
}


Util::~Util()
{
}
string Util::int2string(int intVal)
{
	stringstream ss;
	ss << intVal;
	string str = ss.str();
	return str;
}
int Util::binaryToDecimal(string src)
{
	return (bitset<maxDigitBitCount>(src).to_ulong());
}
string Util::replaceFirstOf(string src, string match, string fill)
{
	int x = src.find_first_of(match);
	string result = src;
	if (x!=-1)
		result = src.replace(x, 1, fill);
	return result;

}
bool Util::findInstrInQueue(code* instruction, queue<code*> objQueue)
{
	queue<code*> tempQueue = objQueue;
	code* instrInQueue=NULL;
	while (tempQueue.size()>0)
	{
		instrInQueue = tempQueue.front();
		tempQueue.pop();
		if (instruction->instr_text == instrInQueue->instr_text)
			return true;
	}
	return false;
}
