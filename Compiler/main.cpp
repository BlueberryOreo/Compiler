#include "TESTscan.h"

#define INPUTPATH "E:\\Progress\\Compiler\\school\\test.T"
#define OUTPUTPATH "E:\\Progress\\Compiler\\school\\out.T"

int main() {
	
	string inputPath, outputPath;
#ifdef INPUTPATH
	inputPath = INPUTPATH;
	outputPath = OUTPUTPATH;
#else
	cin >> inputPath >> outputPath;
#endif
	
	int err = TESTscan(inputPath, outputPath);
	if (!err) {
		cout << "�ʷ������ɹ�" << endl;
	}
	else {
		cout << "���ִ���ֹͣ����" << endl;
	}

	return 0;
}