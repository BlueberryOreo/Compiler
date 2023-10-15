#pragma once
#include "util.h"
#include "io.h"
#include "RegExp.h"

class TESTscan2
{
private:
	string keyword[KEYWORDNUM] = { "if", "int", "else", "for", "while", "write", "do", "read" };
	//string singleword = "+-(){};,:*";
	//string doubleword = "><=!";
	RegExp *regNum; // 数字自动机
	RegExp *regId; // 标识符自动机
	RegExp *regSingle; // 单分界符自动机
	RegExp *regDouble; // 双分界符自动机
	//RegExp *regCom;
	string lower(string &s);

public:
	TESTscan2();
	~TESTscan2();
	int scan(string &input_file, string &output_file);
};

