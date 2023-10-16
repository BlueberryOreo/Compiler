#include "LL.h"

int program();
int compoundStat();
int statement();
int expressionStat();
int expression();
int boolExpr();
int additiveExpr();
int term();
int factor();
int ifStat();
int whileStat();
int forStat();
int writeStat();
int readStat();
int declarationStat();
int declarationList();
int statementList();
int compoundList();

//extern char token[20], token1[40];
struct Token {
	string first, second;
};
//extern char Scanout[300];
ifstream fp;

ifstream& operator>>(ifstream &fin, Token &t) {
	fin >> t.first >> t.second;
	return fin;
}

//语法分析程序
int TESTparse(string &lexerOut) {
	int es = 0;
	fp.open(lexerOut.c_str());
	if (!fp)
	{
		cout << "Cannot find " << lexerOut << endl;
		es = 10;
	}
	if (es == 0) es = program();
	printf(" == 语法分析结果 == \n");
	switch (es)
	{
	case 0: printf("语法分析成功\n"); break;
	case 10: cout << "打开文件 " << lexerOut << " 失败" << endl;; break;
	case 1: printf("\n"); break; // program大括号未闭合
	case 2: printf("\n"); break; // declarationError
	case 3: printf("\n"); break;
	case 4: printf("\n"); break;
	case 5: printf("\n"); break;
	case 6: printf("\n"); break;
	case 7: printf("\n"); break;
	}
	fp.close();
	return(es);
}

//  <program> -> { <declaration_list><statement_list> }
int program() 
{
	Token t;
	fp >> t;
	if (t.second == "{") {
		int es = declarationList();
		if (es) return es;
		es = statementList();
		fp >> t;
		if (t.second == "}") return es;
		return 1;
	}
	else {
		return 1;
	}
}

// <compound_stat> -> {<statement_list>}
int compoundStat()
{
	Token t;
	auto tmpPos = fp.tellg();
	fp >> t;
	if (t.first != "{") {
		fp.seekg(tmpPos);
		return -1;
	}
	int es = statementList();

	return 0;
}

int expressionStat()
{
	return 0;
}

//<表达式> -> <标识符>=<布尔表达式><布尔表达式>
//<expr> -> ID=<bool _expr> | <bool_expr>
int expression()
{
	//int es = 0, fileadd;
	//char token2[20], token3[40];
	//if (strcmp(token, "ID") == 0)
	//{
	//	fileadd = ftell(fp); //记住当前文件位置
	//	fscanf(fp, "%s %s\n", &token2, &token3);
	//	printf("%s %s \n", token2, token3);
	//	if (strcmp(token2, "=") == 0) //赋值表达式
	//	{
	//		fscanf(fp, "%s %s\n", &token, &token1);
	//		printf("%s %s\n", token, token1);
	//		es = boolExpr();
	//		if (es > 0)return(es);
	//	}
	//	else
	//	{
	//		fseek(fp, fileadd, 0); //若非=, 则文件指针回到=前的标识符
	//		printf("%s $s\n", token, token1);
	//		es = boolExpr();
	//		if (es > 0) return(es);
	//	}
	//}
	//else es = boolExpr();
	//return(es);
	return 0;
}

int boolExpr()
{
	return 0;
}

int additiveExpr()
{
	return 0;
}

int term()
{
	return 0;
}

int factor()
{
	return 0;
}

int ifStat()
{
	return 0;
}

int whileStat()
{
	return 0;
}

int forStat()
{
	return 0;
}

int writeStat()
{
	return 0;
}

int readStat()
{
	return 0;
}

// <declaration_stat> -> int ID;
int declarationStat()
{
	Token t;
	auto tmpPos = fp.tellg();
	fp >> t;
	if (t.first != "int") {
		fp.seekg(tmpPos);
		return -1;
	}
	fp >> t;
	if (t.first != "ID") return 2;
	fp >> t;
	if (t.first != ";") return 2;
	return 0;
}

int declarationListRight() {
	int es = declarationStat();
	if (es == -1) return 0;
	else return es;
	es = declarationListRight();
	return es;
}

// <declaration_list> -> <declaration_list><declaration_stat> | ε
/*
	<declaration_list> -> ε<R>
	<R> -> <declaration_state><R> | ε
*/
int declarationList()
{
	return declarationListRight();
}

//<statement> -> <if_stat> | <while_stat> | <for_stat> | <read_stat> | <write_stat> | <compound_stat> | <expression_stat>
int statement()
{
	Token t;
	auto tmpPosBef = fp.tellg();
	fp >> t;
	fp.seekg(tmpPosBef);
	if (t.first == "if") return ifStat();
	if (t.first == "while") return whileStat();
	if (t.first == "for") return forStat();
	if (t.first == "read") return readStat();
	if (t.first == "write") return writeStat();
	int es = compoundStat();
	if (es != -1) return es;
	es = expressionStat();
	return es;
}

int statementRight() {
	int es = statement();
	if (es == -1) return 0;
	else return es;
	es = statementRight();
	return es;
}

// <statement_list> -> <satement_list><statement> | ε
/*
	<statement_list> -> ε<R>
	<R> -> <statement><R> | ε
*/
int statementList()
{
	return statementRight();
}

int compoundList()
{
	return 0;
}
