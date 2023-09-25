#include "TESTscan.h"
#include "RegExp.h"

#define INPUTPATH "E:\\Progress\\Compiler\\school\\test.T"
#define OUTPUTPATH "E:\\Progress\\Compiler\\school\\out.T"

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

	RegExp reg;
	
	//int err = TESTscan(inputPath, outputPath);
	//if (!err) {
	//	cout << "词法分析成功" << endl;
	//}
	//else {
	//	cout << "出现错误，停止编译" << endl;
	//}

	return 0;
}

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

*/