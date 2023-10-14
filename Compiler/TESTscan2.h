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
	RegExp *regNum; // �����Զ���
	RegExp *regId; // ��ʶ���Զ���
	RegExp *regSingle; // ���ֽ���Զ���
	RegExp *regDouble; // ˫�ֽ���Զ���
	//RegExp *regCom;

public:
	TESTscan2();
	~TESTscan2();
	int scan(string &input_file, string &output_file);
};
