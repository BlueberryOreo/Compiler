#include "RecursionParser.h"

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

//extern char Scanout[300];
ifstream fp;
int currentPos, currentLine;
int tab = 0;
int labelId = 0;

string outputPath = "./out.asm";
class VarTable {
public:
	static int datap;
	map<string, int> _vartable;
	//map<int, string> _dVartable;

	int add(string &id) {
		if (_vartable.find(id) == _vartable.end()) {
			_vartable[id] = datap;
			//_dVartable[datap] = id;
			datap++;
			return 0;
		}
		return 1;
	}

	void lookUp(string &id, int &addr) {
		if (_vartable.find(id) == _vartable.end()) addr = -1;
		else addr = _vartable[id];
	}
} vartable;

int VarTable::datap = 0;
ofstream fout;

void getToken(Token &t) {
	fp >> t;
	currentPos = t.pos;
	currentLine = t.line;
	//cout << currentPos << " " << currentLine << endl;
}

// 获取下一个Label id
int getLabelId() {
	return ++labelId;
}

void setLabel(int id) {
	fout << "LABEL" << id << ":" << endl;
}

void jmpLabel(string opt, int id) {
	fout << opt << " LABEL" << id << endl;
}

//语法分析程序
int TESTparse(string &source, string &lexerOut) {
	int es = 0;
	fp.open(lexerOut.c_str());
	ifstream sourcef;
	sourcef.open(source);
	if (!fp) {
		cout << "Cannot find " << lexerOut << endl;
		es = 10;
	}
	if (!sourcef) {
		cout << "Cannot find " << source << endl;
		es = 10;
	}
	fout.open(outputPath.c_str());
	if (es == 0) es = program();
	//printf(" == 语法分析结果 == \n");
	switch (es)
	{
	case 0: printf("语法分析成功\n"); break;
	case 10: error(FILENOTFOUND, NULL, 0, lexerOut); break;
	case 1: error(SYNTAXERROR, &sourcef, currentPos, "括号未闭合", currentLine); break; // 括号未闭合
	case 2: error(SYNTAXERROR, &sourcef, currentPos, "声明错误", currentLine); break; // declarationError
	case 3: error(SYNTAXERROR, &sourcef, currentPos, "缺少结尾", currentLine); break; // 没有结尾
	case 4: error(SYNTAXERROR, &sourcef, currentPos, "意外出现的表达式", currentLine); break; // 表达式错误
	case 5: error(SYNTAXERROR, &sourcef, currentPos, "if语法错误", currentLine); break; // if语句错误
	case 6: error(SYNTAXERROR, &sourcef, currentPos, "while语法错误", currentLine); break; // while语句错误
	case 7: error(SYNTAXERROR, &sourcef, currentPos, "for语法错误", currentLine); break; // for语句错误
	case 8: error(SYNTAXERROR, &sourcef, currentPos, "read语法错误", currentLine); break; // read语句错误
	case 9: error(SYNTAXERROR, &sourcef, currentPos, "符号重定义", currentLine); break;
	case 11: error(SYNTAXERROR, &sourcef, currentPos, "符号未定义", currentLine); break;
	//case 12: printf("\n"); break;
	//case 13: printf("\n"); break;
	}
	fp.close();
	sourcef.close();
	return(es);
}

//  <program> -> { <declaration_list><statement_list> }
int program() 
{
#ifdef OUTPUT_TREE
	cout << "<program>" << endl;
	tab++;
#endif
	Token t;
	getToken(t);
	if (t.first == "{") {
#ifdef DEBUG_LL
		cout << "<program> -> " << t << endl;
#endif // DEBUG_LL
		int es = declarationList();
#ifdef DEBUG_LL
		cout << "81 <program> es=" << es << endl;
#endif // DEBUG_LL
		if (es > 0) return es;
		es = statementList();
		if (es > 0) return es;
		getToken(t);
#ifdef DEBUG_LL
		cout << "<program> -> " << t << endl;
#endif // DEBUG_LL

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
	getToken(t);
	if (t.first != "{") {
		fp.seekg(tmpPos);
		return -1;
	}
	int es = statementList();
	if (es) return es;
	getToken(t);
	if (t.first != "}") return 1;
	return 0;
}

//  <expression_stat> -> <expression>; | ;
int expressionStat()
{
	int es = expression();
	if (es != 0) return es;
	Token t;
	getToken(t);
	if (es == 0) fout << "POP" << endl;
#ifdef DEBUG_LL
	cout << "<expression_stat> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first != ";") return 3;
	return 0;
}

//<表达式> -> <标识符>=<布尔表达式><布尔表达式>
//<expr> -> ID=<bool _expr> | <bool_expr>
int expression()
{
	int es = 0;
	auto begin = fp.tellg();
	Token t1;
	getToken(t1);
#ifdef DEBUG_LL
	cout << "<expr> -> " << t1 << endl;
#endif // DEBUG_LL
	if (t1.first == "ID"){
		int addr;
		vartable.lookUp(t1.second, addr);
		if (addr < 0) return 11;
		Token t2;
		getToken(t2);
#ifdef DEBUG_LL
		cout << "<expr> -> " << t2 << endl;
#endif // DEBUG_LL
		if (t2.first == "=") {//赋值表达式
			es = boolExpr();
			if (es > 0) return es;
			fout << "STO " << addr << endl;
		}
		else {
			//cout << "169here" << endl;
			//fseek(fp, fileadd, 0); //若非=, 则文件指针回到=前的标识符
			fp.seekg(begin);
			//printf("%s $s\n", token, token1);
			//cout << t1 << endl;
			es = boolExpr();
			//cout << "175 es=" << es << endl;
			if (es > 0) return es;
		}
	}
	else {
		fp.seekg(begin);
		es = boolExpr();
	}
	return es;
}

/*
<bool_expr> -> <additive_expr> 
			 | <additive_expr>(>|<|>=|<=|==|!=)<additive_expr>
*/
int boolExpr()
{
	int es = additiveExpr();
	if (es != 0) return es;
	auto tmp = fp.tellg();
	Token t;
	getToken(t);
#ifdef DEBUG_LL
	cout << "<bool_expr> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first == ">" || t.first == "<" || t.first == ">=" || t.first == "<=" || t.first == "==" || t.first == "!=") {
		es = additiveExpr();
		if (es > 0) return es;
		if (t.first == ">") fout << "GT" << endl;
		if (t.first == "<") fout << "LES" << endl;
		if (t.first == ">=") fout << "GE" << endl;
		if (t.first == "<=") fout << "LE" << endl;
		if (t.first == "==") fout << "EQ" << endl;
		if (t.first == "!=") fout << "NOTEQ" << endl;
		return es;
	}
	fp.seekg(tmp);
	return es;
}

// <additive_expr> -> <term>{(+|-)<term>}
int additiveExpr()
{
	int es = term();
	if (es != 0) return es;
	auto tmp = fp.tellg();
	Token t;
	getToken(t);
#ifdef DEBUG_LL
	cout << "<additive_expr> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first == "+" || t.first == "-") {
		es = term();
		if (es > 0) return es;
		if (t.first == "+") fout << "ADD" << endl;
		if (t.first == "-") fout << "SUB" << endl;
		return es;
	}
	fp.seekg(tmp);
	return es;
}

// <term> -> <factor>{(*|/)<factor>}
int term()
{
	int es = factor();
	if (es != 0) return es;
	auto tmp = fp.tellg();
	Token t;
	getToken(t);
#ifdef DEBUG_LL
	cout << "<term> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first == "*" || t.first == "/") {
		es = factor();
		if (es > 0) return es;
		if (t.first == "*") fout << "MULT" << endl;
		if (t.first == "/") fout << "DIV" << endl;
		return es;
	}
	fp.seekg(tmp);
	return es;
}

// <factor> -> (<expression>) | ID | NUM
int factor()
{
	auto begin = fp.tellg();
	Token t;
	getToken(t);
#ifdef DEBUG_LL
	cout << "<factor> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first == "(") {
		int es = expression();
		if (es > 0) return es;
		getToken(t);
		if (t.first != ")") return 1;
		else return 0;
	}
	else if (t.first == "ID") {
		int addr;
		vartable.lookUp(t.second, addr);
		if (addr < 0) return 11;
		fout << "LOAD " << addr << endl;
		return 0;
	}
	else if (t.first == "NUM") {
		fout << "LOADI " << t.second << endl;
		return 0;
	}
	fp.seekg(begin);
	return -1;
}

// <if_stat> -> if(<expression>) <statement> [else <statement>]
int ifStat()
{
	auto begin = fp.tellg();
	Token t;
	getToken(t);
	if (t.first != "if") {
		fp.seekg(begin);
		return -1;
	}
	getToken(t);
	if (t.first != "(") return 5;
	int es = expression();
	if (es > 0) return es;

	// addLabel
	int labelA = getLabelId();
	int labelB = getLabelId();

	getToken(t);
	if (t.first != ")") return 5;
	jmpLabel("BRF", labelA);
	es = statement();
	if (es > 0) return es;

	jmpLabel("BR", labelB);
	setLabel(labelA);

	auto tmp = fp.tellg();
	getToken(t);
	if (t.first == "else") es = statement();
	else fp.seekg(tmp);
	setLabel(labelB);
	return es;
}

// <while_stat> -> while(<expression>) <statement>
int whileStat()
{
	auto begin = fp.tellg();
	Token t;
	getToken(t);
	if (t.first != "while") {
		fp.seekg(begin);
		return -1;
	}

	// addLabel
	int labelA = getLabelId();
	int labelB = getLabelId();
	setLabel(labelA);

	getToken(t);
	if (t.first != "(") return 6;
	int es = expression();
	if (es > 0) return es;
	getToken(t);
	if (t.first != ")") return 6;
	jmpLabel("BRF", labelB);
	es = statement();
	if (es > 0) return es;

	jmpLabel("BR", labelA);
	setLabel(labelB);

	return es;
}

// <for_stat> -> for(<expression>;<expression>;<expression>) <statement>
int forStat()
{
	auto begin = fp.tellg();
	Token t;
	getToken(t);
	if (t.first != "for") {
		fp.seekg(begin);
		return -1;
	}
	getToken(t);
	if (t.first != "(") return 7;

	int labels[4] = { getLabelId(), getLabelId(), getLabelId(), getLabelId() };

	int es = expression();
	if (es > 0) return es;
	if (es == 0) fout << "POP" << endl;
	getToken(t);
	if (t.first != ";") return 7;

	setLabel(labels[0]);
	es = expression();
	if (es > 0) return es;
	if (es == 0) {
		jmpLabel("BRF", labels[1]);
		jmpLabel("BR", labels[2]);
	}
	getToken(t);
	if (t.first != ";") return 7;

	setLabel(labels[3]);
	es = expression();
	if (es > 0) return es;
	if (es == 0) {
		fout << "POP" << endl;
		jmpLabel("BR", labels[0]);
	}

	getToken(t);
	if (t.first != ")") return 7;
	setLabel(labels[2]);
	es = statement();
	if (es > 0) return es;

	jmpLabel("BR", labels[3]);
	setLabel(labels[1]);

	return es;
}

//  <write_stat> -> write <expression>;
int writeStat()
{
	auto begin = fp.tellg();
	Token t;
	getToken(t);
	if (t.first != "write") {
		fp.seekg(begin);
		return -1;
	}
	int es = expression();
#ifdef DEBUG_LL
	cout << "346 es=" << es << endl;
#endif
	if (es > 0) return es;

	fout << "OUT" << endl;

	getToken(t);
#ifdef DEBUG_LL
	cout << "351 token=" << t << endl;
#endif
	if (t.first != ";") return 3;
	return 0;
}

// <read_stat> -> read ID;
int readStat()
{
	auto begin = fp.tellg();
	Token t;
	getToken(t);
	if (t.first != "read") {
		fp.seekg(begin);
		return -1;
	}
	getToken(t);
	if (t.first != "ID") return 8;

	int addr;
	vartable.lookUp(t.second, addr);
	if (addr < 0) return 11;
	fout << "IN" << endl;
	fout << "STO " << addr << endl;
	fout << "POP" << endl;

	getToken(t);
	if (t.first != ";") return 3;
	return 0;
}

// <declaration_stat> -> int ID;
int declarationStat()
{
	Token t;
	auto tmpPos = fp.tellg();
	getToken(t);
#ifdef DEBUG_LL
	cout << "326 <declaration_stat> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first != "int") {
		fp.seekg(tmpPos);
		return -1;
	}
	getToken(t);
#ifdef DEBUG_LL
	cout << "335 <declaration_stat> -> " << t << endl;
#endif // DEBUG_LL

	if (t.first != "ID") return 2;

	if (vartable.add(t.second)) return 9;

	getToken(t);
#ifdef DEBUG_LL
	cout << "341 <declaration_stat> -> " << t << endl;
#endif // DEBUG_LL
	if (t.first != ";") return 2;
	return 0;
}

int declarationListRight() {
	int es = declarationStat();
	if (es == -1) return 0;
	else if(es > 0) return es;
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
	int es = declarationListRight();
	if (es <= 0) return 0;
	return es;
}

//<statement> -> <if_stat> | <while_stat> | <for_stat> | <read_stat> | <write_stat> | <compound_stat> | <expression_stat>
int statement()
{
	Token t;
	auto tmpPosBef = fp.tellg();
	getToken(t);
#ifdef DEBUG_LL
	cout << "<statement> -> " << t << endl;
#endif // DEBUG_LL

	fp.seekg(tmpPosBef);
	if (t.first == "if") return ifStat();
	if (t.first == "while") return whileStat();
	if (t.first == "for") return forStat();
	if (t.first == "read") return readStat();
	if (t.first == "write") return writeStat();
	if (t.first == "{") return compoundStat();
	return expressionStat();
}

int statementRight() {
	int es = statement();
	if (es == -1) return 0;
	else if(es > 0) return es;
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
	int es = statementRight();
	if (es <= 0) return 0;
	return es;
}

int compoundList()
{
	return 0;
}
