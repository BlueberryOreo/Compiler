#include "TESTscan.h"

#define NINPUTPATH "E:\\Progress\\Compiler\\school\\test.T"
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
	
	int err = TESTscan(inputPath, outputPath);
	if (!err) {
		cout << "词法分析成功" << endl;
	}
	else {
		cout << "出现错误，停止编译" << endl;
	}

	return 0;
}