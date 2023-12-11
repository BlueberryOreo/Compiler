#include "TVM.h"

int TVM::push(int d)
{
	if (stktop >= MAX - 1) return 1;
	stk[++stktop] = d;
	return 0;
}

int TVM::get()
{
	return stk[stktop];
}

int TVM::pop()
{
	if (stktop == -1) return 1;
	stktop--;
	return 0;
}

int TVM::stoi(string& s)
{
	int res = 0;
	for (int i = 0; i < s.size(); i ++) {
		res *= 10;
		res += (int)(s[i] - '0');
	}
	return res;
}

TVM::TVM()
{
	this->reset();
}

void TVM::reset()
{
	PC = 0;
	code.clear();
	memset(data, 0, sizeof data);
	stktop = -1;
}

int TVM::analyze(Code& c)
{
	PC++;
	if (c[0] == "LOAD") {
		return push(data[stoi(c[1])]);
	}
	else if (c[0] == "LOADI") {
		return push(stoi(c[1]));
	}
	else if (c[0] == "STO") {
		data[stoi(c[1])] = get();
	}
	else if (c[0] == "POP") {
		return pop();
	}
	else if (c[0] == "ADD") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] += top;
	}
	else if (c[0] == "SUB") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] -= top;
	}
	else if (c[0] == "MULT") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] *= top;
	}
	else if (c[0] == "DIV") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] /= top;
	}
	else if (c[0] == "BR") {
		if (labels.find(c[1]) == labels.end()) return 1;
		PC = labels[c[1]];
	}
	else if (c[0] == "BRF") {
		if (!get()) {
			if (labels.find(c[1]) == labels.end()) return 1;
			PC = labels[c[1]];
		}
	}
	else if (c[0] == "EQ") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() == top;
	}
	else if (c[0] == "NOTEQ") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() != top;
	}
	else if (c[0] == "GT") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() > top;
	}
	else if (c[0] == "LES") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() < top;
	}
	else if (c[0] == "GE") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() >= top;
	}
	else if (c[0] == "LE") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() <= top;
	}
	else if (c[0] == "AND") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() && top;
	}
	else if (c[0] == "OR") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = get() || top;
	}
	else if (c[0] == "NOT") {
		int top = get();
		int es = pop();
		if (es) return es;
		stk[stktop] = !stk[stktop];
	}
	else if (c[0] == "IN") {
		int input;
		cin >> input;
		return push(input);
	}
	else if (c[0] == "OUT") {
		int out = get();
		int es = pop();
		if (es) return es;
		cout << out << endl;
	}
	else if (c[0] == "STOP") {
		return 2;
	}
	else return 1;

	return 0;
}

int TVM::run(const string& asmPath)
{
	ifstream ifs;
	ifs.open(asmPath.c_str());
	if (!ifs) throw "File not found.";

	string line;
	int idx = 0;
	while (getline(ifs, line)) {
		if (line.back() == '\n') line.pop_back();
		if (line.size() == 0) continue;

		Code c = Code(line);
		if (c.isLabel) {
			labels[c[0]] = idx;
			continue;
		}
		code.push_back(c);
		idx++;
	}

	//for (int i = 0; i < code.size(); i ++) {
	//	cout << i << ": ";
	//	code[i].out();
	//}

	//for (unordered_map<string, int>::iterator it = labels.begin(); it != labels.end(); it ++) {
	//	cout << it->first << ": " << it->second << endl;
	//}
	//
	//return 0;

	int es = 0;
	while (PC < code.size()) {
		es = analyze(code[PC]);
		if (es) break;
	}

	reset();
	return es;
}

Code::Code(string c)
{
	string cmd = "";
	if (c.back() == ':') isLabel = true, c.pop_back();
	else isLabel = false;
	c.push_back(' ');
	for (int i = 0; i < c.size(); i++) {
		if (c[i] == ' ') {
			code.push_back(cmd);
			cmd.clear();
			continue;
		}
		cmd.push_back(c[i]);
	}
}

string& Code::operator[](int idx)
{
	// TODO: 在此处插入 return 语句
	return code[idx];
}

void Code::out()
{
	for (auto c: code) {
		cout << c << " ";
	}
	cout << endl;
}
