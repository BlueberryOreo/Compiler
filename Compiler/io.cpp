#include "io.h"

extern int line = 1;
extern string scanIn = "", scanOut = "";

char getch(ifstream& ifs) {
	 char tmp = ifs.get();
	 scanIn.push_back(tmp);
	 if (tmp == '\n') line++;
	 return tmp;
}

void testId(string& id) {
	if (id.size()) {
		scanOut += "ID\t" + id + "\n";
	}
	id = "";
}

void outputKw(string& kw) {
	scanOut += kw + "\t" + kw + "\n";
}

void outputNum(string& num) {
	scanOut += "NUM\t" + num + "\n";
}

void outputSingleWord(char tmp) {
	scanOut.push_back(tmp);
	scanOut.push_back('\t');
	scanOut.push_back(tmp);
	scanOut.push_back('\n');
}

void outputDoubleWord(string& dw) {
	scanOut += dw + "\t" + dw + "\n";
}

void output(ofstream& ofs)
{
	ofs << scanOut;
}

void error(int type, ifstream& ifs, int shift) {
	if (type == SYNTAXERROR) {
		cout << "Syntax Error in line " << line << ":" << endl;
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