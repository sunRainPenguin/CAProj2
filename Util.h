//一些通用的函数
#pragma once
#include "const.h"

class Util
{
public:
	Util();
	~Util();
	static string int2string(int intVal);
	static int binaryToDecimal(string src);
	static string replaceFirstOf(string src, string match, string fill);
};

