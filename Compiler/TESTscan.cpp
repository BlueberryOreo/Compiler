#include "TESTscan.h"

string keyword[KEYWORDNUM] = {"if", "int", "else", "for", "while", "write", "do", "read"};
string singleword = "+-(){};,:";
string doubleword = "><=!/*";

// ת��ΪСд
char lowerCase(char c) {
	if ('A' <= c && c <= 'Z') return c + 32;
	return c;
}

void lowerCase(string &s) {
	for (int i = 0; i < s.size(); i ++) {
		s[i] = lowerCase(s[i]);
	}
}

// �ж��Ƿ����Ϊ�ؼ���
int testKeyword(char c) {
	for (int i = 0; i < KEYWORDNUM; i ++) {
		if (keyword[i][0] == lowerCase(c)) return i;
	}
	return -1;
}

// �ж��Ƿ����Ϊ���ֽ��
int testSingleword(char c) {
	for (int i = 0; i < singleword.size(); i ++) {
		if (singleword[i] == c) return 1;
	}
	return 0;
}

// �ж��Ƿ����Ϊ˫�ֽ��
int testDoubleword(char c) {
	for (int i = 0; i < doubleword.size(); i ++) {
		if (doubleword[i] == c) return 1;
	}
	return 0;
}

bool testWord(char c) {
	if (('a' <= c && c <= 'z') ||
		('A' <= c && c <= 'Z') ||
		('0' <= c && c <= '9') || 
		c == '_')
		return true;
	return false;
}

// �жϿ��ַ�
bool testBlank(char c) {
	if (c == ' ' || c == '\t' || c == '\n') return true;
	return false;
}

int TESTscan(string pathIn, string pathOut) {
	ifstream ifs;
	ifs.open(pathIn.c_str());
	if (!ifs) {
		error(FILENOTFOUND, &ifs, 0, pathIn);
		return FILENOTFOUND;
	}

	char tmp, next = '\0';
	string id = "";
	while (next || (tmp = getch(ifs)) != EOF) {
		if (next) {
			tmp = next;
			next = '\0';
		}
		if (testBlank(tmp)) {
			testId(id);
			continue;
		}

		int kwIndex = testKeyword(tmp);
		if (kwIndex >= 0) {
			// �����ǹؼ���
			string kw = "";
			kw.push_back(tmp);
			while (tmp = getch(ifs)) {
				if (testBlank(tmp) || tmp == EOF) break;
				if (testSingleword(tmp) || testDoubleword(tmp)) break;
				if(testWord(tmp)) kw.push_back(tmp);
				else {
					error(WORDERROR, &ifs);
					return WORDERROR;
				}
			}
			if (kw == keyword[kwIndex] ||
				(kwIndex + 1 < KEYWORDNUM && kw == keyword[kwIndex + 1]))
			{
				lowerCase(kw);
				outputKw(kw);
			}
			else {
				id += kw;
			}
			if (testBlank(tmp) || tmp == EOF) continue;
		}

		// �ж�����
		if ('0' <= tmp && tmp <= '9' && !id.size()) {
			string num = "";
			bool notZero = false;
			if (tmp != '0') {
				notZero = true;
				num.push_back(tmp);
			}
			while (tmp = getch(ifs)) {
				if (testBlank(tmp) || tmp == EOF) break;
				if (testSingleword(tmp) || testDoubleword(tmp)) break;
				if (tmp != '0') notZero = true;
				if (!notZero) continue;
				if ('0' <= tmp && tmp <= '9') num.push_back(tmp);
				else {
					error(WORDERROR, &ifs);
					return WORDERROR;
				}
			}
			if (num.size() == 0) num.push_back('0');
			outputNum(num);
		}

		// �жϵ��ַ�
		if (testSingleword(tmp)) {
			testId(id);
			outputSingleWord(tmp);
			continue;
		}

		// �ж�˫�ַ�
		if (testDoubleword(tmp)) {
			testId(id);
			next = getch(ifs);
			string dw = "";
			dw.push_back(tmp);
			if (tmp == '/' && next == '*') {
				// ע��
				while (tmp = getch(ifs)) {
					if (tmp == '*') {
						next = getch(ifs);
						if (next == '/') {
							break;
						}
					}
					if (tmp == EOF) {
						error(WORDERROR, &ifs);
						return WORDERROR;
					}
				}
				next = '\0';
				continue;
			}
			if (tmp == '*' && next == '/') {
				error(WORDERROR, &ifs, -1);
				return WORDERROR;
			}
			if (tmp != '/' && tmp != '*' && next == '=') {
				dw.push_back(next);
				next = '\0';
			}
			outputDoubleWord(dw);
			continue;
		}
		
		if (testWord(tmp)) id.push_back(tmp);
		else {
			error(WORDERROR, &ifs);
			return WORDERROR;
		}
	}
	ofstream ofs;
	ofs.open(pathOut.c_str());
	if (!ofs) {
		error(FILENOTFOUND, nullptr, 0, pathOut);
		return FILENOTFOUND;
	}
	output(ofs);
	ifs.close();
	ofs.flush();
	ofs.close();

	return 0;
}
