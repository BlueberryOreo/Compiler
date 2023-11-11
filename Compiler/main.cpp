#include<stdio.h>
#include<ctype.h>
#include "TESTscan.h"
#include "TESTscan2.h"
#include "RegExp.h"
#include "LL.h"
#include "LLNRec.h"

#define INPUTPATH "E:\\Progress\\Compiler\\test.T"
#define OUTPUTPATH "E:\\Progress\\Compiler\\out.T"

#define LLNRC_TEST1

vector<Token> getTokens(string testStr) {
	vector<Token> ret;
	string tmp = "";
	testStr.push_back(' ');
	for (char c: testStr) {
		if (c == ' ') {
			ret.push_back(Token{ tmp, tmp, 0, 0 });
			tmp.clear();
		}
		else tmp.push_back(c);
	}
	return ret;
}

int main() {
	
	string inputPath, outputPath;
#ifdef INPUTPATH
	inputPath = INPUTPATH;
	outputPath = OUTPUTPATH;
#else
	cout << "请输入输入文件位置:" << endl;
	cin >> inputPath;
	cout << "请输入输出文件位置:" << endl;
	cin >> outputPath;
#endif

#ifdef LLNRC_TEST1
	LLNRec parser;
	//string test = "id + id * ( id + id ) $";
	string test = "( id + id ) * id * id + id $";
	vector<Token> testTokens = getTokens(test);
	parser.analyze(testTokens);
#endif // LLNRC_TEST1

	return 0;
}

//extern int TESTscan();
//TESTscan2 lexer;
//extern int TESTparse(string &source, string &lexerOut);
////char Scanin[300], Scanout[300];
////FILE* fin, * fout;
//int main() {
//
//	string inputPath, outputPath;
//#ifdef INPUTPATH
//	inputPath = INPUTPATH;
//	outputPath = OUTPUTPATH;
//#else
//	cout << "请输入输入文件位置:" << endl;
//	cin >> inputPath;
//	cout << "请输入输出文件位置:" << endl;
//	cin >> outputPath;
//#endif
//
//	int es = 0;
//	//es = TESTscan();
//	es = lexer.scan(inputPath, outputPath);
//	if (es > 0) printf("词法分析有错, 编译终止!\n");
//	else printf("词法分析成功!\n");
//	//return 0;
//	if (es == 0)
//	{
//		es = TESTparse(inputPath, outputPath);
//		if (es > 0) printf("语法分析有错, 编译终止!\n");
//		else printf("语法分析成功!\n");
//	}
//	return 0;
//}


/*#include "TESTscan.h"
#include "TESTscan2.h"
#include "RegExp.h"

#define INPUTPATH "E:\\Progress\\Compiler\\test.T"
#define OUTPUTPATH "E:\\Progress\\Compiler\\out.T"

int main() {
	
	string inputPath, outputPath;
#ifdef INPUTPATH
	inputPath = INPUTPATH;
	outputPath = OUTPUTPATH;
#else
	cout << "请输入输入文件位置:" << endl;
	cin >> inputPath;
	cout << "请输入输出文件位置:" << endl;
	cin >> outputPath;
#endif
	 
	//RegExp reg;
	////ifstream com;
	////com.open("./comment.tree");
	////reg.readFromFile(com);
	////com.close();
	//reg.readTree();
	//reg.init();
	//string test = "aaabbb";
	//if (reg.match(test)) {
	//	cout << test << " 被接受" << endl;
	//}
	//else {
	//	cout << test << " 不被接受" << endl;
	//}
	
	TESTscan2 lexer;
	int err = lexer.scan(inputPath, outputPath);
	if (!err) {
		cout << "词法分析成功" << endl;
	}
	else {
		cout << "出现错误，停止编译" << endl;
	}

	return 0;
}
*/

/*
r : a
r : b
r : r | r
r : (r)
r : r*
r : rr
*/

/*
r9 r7
r9 r8
r7 r5
r7 r6
r5 r4
r5 *
r4 (
r4 r3
r4 )
r3 r1
r3 |
r3 r2
r1 a
r2 b
r6 a
r8 b
*/

/*
r8 r7
r8 r11
r7 r6
r7 r10
r6 r5
r6 r9
r5 r4
r5 *
r4 (
r4 r3
r4 )
r3 r1
r3 |
r3 r2
r1 a
r2 b
r9 a
r10 b
r11 b
*/

/*
r1 r2
r1 r3
r2 r4
r2 r5
r4 a
r5 b
r3 b
*/

/*
r1 r2
r1 |
r1 r5
r2 r3
r2 r4
r3 1
r4 0
r5 r6
r5 r12
r6 (
r6 r7
r6 )
r7 r8
r7 |
r7 r9
r8 0
r9 r10
r9 r11
r10 1
r11 1
r12 r13
r12 r15
r13 r14
r13 *
r14 0
r15 1
*/