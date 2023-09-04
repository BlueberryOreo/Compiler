#include "io.h"

extern int line = 1;
extern string scanIn = "", scanOut = "";

// 从文件中读取一个字符，同时存到scanIn中
char getch(ifstream& ifs) {
	 char tmp = ifs.get();
	 scanIn.push_back(tmp);
	 if (tmp == '\n') line++;
	 return tmp;
}

// 判断标识符变量是否为空，如果不为空则输出当前标识符
void testId(string& id) {
	if (id.size()) {
		scanOut += "ID\t" + id + "\n";
	}
	id = "";
}

// 输出关键字
void outputKw(string& kw) {
	scanOut += kw + "\t" + kw + "\n";
}

// 输出数字
void outputNum(string& num) {
	scanOut += "NUM\t" + num + "\n";
}

// 输出单分界符
void outputSingleWord(char tmp) {
	scanOut.push_back(tmp);
	scanOut.push_back('\t');
	scanOut.push_back(tmp);
	scanOut.push_back('\n');
}

// 输出双分界符
void outputDoubleWord(string& dw) {
	scanOut += dw + "\t" + dw + "\n";
}

// 输出scanOut到文件中
void output(ofstream& ofs)
{
	ofs << scanOut;
}

// 错误处理
void error(int type, ifstream& ifs, int shift) {
	if (type == WORDERROR) {
		cout << "Word Error in line " << line << ":" << endl;
		char tmp;
		int idx = scanIn.size() - 1 + shift;
		int errorIdx = idx;
		while (scanIn[idx - 1] != '\n') idx--;
		while ((tmp = getch(ifs)) != '\n' && tmp != EOF);
		for (int i = idx; i < scanIn.size(); i++) cout << scanIn[i];
		if (scanIn[scanIn.size() - 1] != '\n') cout << endl;
		for (int i = idx; i < errorIdx - 2; i++) cout << ' ';
		cout << "~~^~~" << endl;
		cout << "Unexpected character found '" << scanIn[errorIdx] << "'" << endl;
	}
}