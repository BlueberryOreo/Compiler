#include "io.h"

int line = 1;
extern string scanIn = "", scanOut = "";
streampos pos = 0;
bool isNewLine = false;

// 从文件中读取一个字符，同时存到scanIn中
char getch(ifstream& ifs) {
	if (isNewLine) line++, isNewLine = false;
	char tmp = ifs.get();
	scanIn.push_back(tmp);
	pos = ifs.tellg();
	if (tmp == '\n') isNewLine = true;
	return tmp;
}

string getPosAndLine(string &s) {
	return to_string((int)pos - s.size()) + "\t" + to_string(line);
}

// 判断标识符变量是否为空，如果不为空则输出当前标识符
void testId(string& id) {
	if (id.size()) {
		scanOut += "ID\t" + id + "\t" + getPosAndLine(id) + "\n";
	}
	id = "";
}

// 输出关键字
void outputKw(string& kw) {
	scanOut += kw + "\t" + kw + "\t" + getPosAndLine(kw) + "\n";
}

// 输出数字
void outputNum(string& num) {
	scanOut += "NUM\t" + num + "\t" + getPosAndLine(num) + "\n";
}

// 输出单分界符
void outputSingleWord(char tmp) {
	scanOut.push_back(tmp);
	scanOut.push_back('\t');
	scanOut.push_back(tmp);
	//scanOut.push_back('\n');
	scanOut += "\t" + to_string((int)pos - 1) + "\t" + to_string(line) + "\n";
}

// 输出双分界符
void outputDoubleWord(string& dw) {
	scanOut += dw + "\t" + dw + "\t" + getPosAndLine(dw) + "\n";
}

// 输出scanOut到文件中
void output(ofstream& ofs)
{
	if (scanOut[scanOut.size() - 1] == '\n') scanOut.pop_back();
	ofs << scanOut;
}

ostream& operator<<(ostream& out, vector<string>& v) {
	for (auto item : v) {
		out << item << " ";
	}
	return out;
}

ifstream& operator>>(ifstream& fin, Token& t) {
	fin >> t.first >> t.second >> t.pos >> t.line;
	return fin;
}

ostream& operator<<(ostream& out, Token& t) {
	out << "(" << t.first << ", " << t.second << ")";
	return out;
}

ostream& operator<<(ostream& out, set<string>& s) {
	for (auto item : s) {
		out << item << " ";
	}
	return out;
}

void output()
{
	cout << scanOut << endl;
}

// 错误处理
void error(int type, ifstream* ifs, int shift, string info, int inputLine) {
	if (type == WORDERROR) {
		cout << "Word Error in line " << (inputLine > 0 ? inputLine: line) << ":" << endl;
		char tmp;
		int idx = scanIn.size() - 1 + shift;
		int errorIdx = idx;
		while (scanIn[idx - 1] != '\n') idx--;
		while ((tmp = getch(*ifs)) != '\n' && tmp != EOF);
		for (int i = idx; i < scanIn.size(); i++) cout << scanIn[i];
		if (scanIn[scanIn.size() - 1] != '\n') cout << endl;
		for (int i = idx; i < errorIdx - 2; i++) cout << ' ';
		cout << "~~^~~" << endl;
		cout << "Unexpected character found '" << scanIn[errorIdx] << "'" << endl;
	}
	if (type == FILENOTFOUND) {
		cout << "File not found" << (info.size() ? (": " + info) : "") << endl;
	}
	if (type == SYNTAXERROR) {
		cout << "Syntax Error in line " << inputLine << ":" << endl;
		deque<char> q;
		char c;
		int tmp = shift;
		//ifs->seekg(shift - 2);
		//cout << (char)ifs->get() << " " << info << endl;
		ifs->seekg(shift);
		//cout << shift << " " << inputLine << endl;
		while ((c = ifs->get()) != '\n' && c != EOF) q.push_back(c);
		ifs->seekg(--shift);
		while ((c = ifs->get()) != '\n' && shift >= 0) q.push_front(c), ifs->seekg(--shift);
		//cout << shift << " " << inputLine << endl;
		if (q.empty()) {
			ifs->seekg(--shift);
			q.push_front((char)ifs->get());
		}
		while (!q.empty()) {
			cout << q.front();
			q.pop_front();
		}
		tmp -= shift;
		cout << endl;
		cout << string(max(0, tmp - 2), ' ') << string(min(2, tmp - 2), '~') << "^~~" << endl;
		cout << info << endl;
	}
}