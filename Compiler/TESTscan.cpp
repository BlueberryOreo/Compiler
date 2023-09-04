#include "TESTscan.h"

string keyword[KEYWORDNUM] = {"if", "int", "else", "for", "while", "write", "do", "read"};
string singleword = "+-*(){};,:";
string doubleword = "><=!/";

char lowerCase(char c) {
	if ('A' <= c && c <= 'Z') return c + 32;
	return c;
}

int testKeyword(char c) {
	for (int i = 0; i < KEYWORDNUM; i ++) {
		if (keyword[i][0] == lowerCase(c)) return i;
	}
	return -1;
}

int testSingleword(char c) {
	for (int i = 0; i < singleword.size(); i ++) {
		if (singleword[i] == c) return 1;
	}
	return 0;
}

int testDoubleword(char c) {
	for (int i = 0; i < doubleword.size(); i ++) {
		if (doubleword[i] == c) return 1;
	}
	return 0;
}

int TESTscan(string pathIn, string pathOut) {
	ifstream ifs;
	ifs.open(pathIn.c_str());
	if (!ifs) {
		error(FILENOTFOUND, ifs);
		return FILENOTFOUND;
	}

	char tmp, next = '\0';
	string id = "";
	while (next || (tmp = getch(ifs)) != EOF) {
		if (next) {
			tmp = next;
			next = '\0';
		}
		if (tmp == ' ' || tmp == '\n') {
			testId(id);
			continue;
		}

		int kwIndex = testKeyword(tmp);
		if (kwIndex >= 0) {
			// ¿ÉÄÜÊÇ¹Ø¼ü×Ö
			string kw = "";
			kw.push_back(lowerCase(tmp));
			while (tmp = getch(ifs)) {
				if (tmp == ' ' || tmp == '\n' || tmp == EOF) break;
				if (testSingleword(tmp) || testDoubleword(tmp)) break;
				kw.push_back(lowerCase(tmp));
			}
			if (kw == keyword[kwIndex] ||
				(kwIndex + 1 < KEYWORDNUM && kw == keyword[kwIndex + 1]))
			{
				outputKw(kw);
			}
			else {
				id += kw;
			}
			if (tmp == ' ' || tmp == '\n' || tmp == EOF) continue;
		}

		// ÅÐ¶ÏÊý×Ö
		if ('0' <= tmp && tmp <= '9' && !id.size()) {
			string num = "";
			bool notZero = false;
			if (tmp != '0') {
				notZero = true;
				num.push_back(tmp);
			}
			while (tmp = getch(ifs)) {
				if (tmp == ' ' || tmp == '\n' || tmp == EOF) break;
				if (testSingleword(tmp) || testDoubleword(tmp)) break;
				if (tmp != '0') notZero = true;
				if (!notZero) continue;
				if ('0' <= tmp && tmp <= '9') num.push_back(tmp);
				else {
					error(SYNTAXERROR, ifs);
					return SYNTAXERROR;
				}
			}
			if (num.size() == 0) num.push_back('0');
			outputNum(num);
		}

		// ÅÐ¶Ïµ¥×Ö·û
		if (testSingleword(tmp)) {
			testId(id);
			outputSingleWord(tmp);
			continue;
		}

		// ÅÐ¶ÏË«×Ö·û
		if (testDoubleword(tmp)) {
			testId(id);
			next = getch(ifs);
			string dw = "";
			dw.push_back(tmp);
			if (tmp == '/' && next == '*') {
				// ×¢ÊÍ
				bool innerComment = false;
				while (tmp = getch(ifs)) {
					if (innerComment) {
						if (tmp == '*') {
							error(SYNTAXERROR, ifs);
							return SYNTAXERROR;
						}
						innerComment = false;
					}
					if (tmp == EOF) {
						error(SYNTAXERROR, ifs);
						return SYNTAXERROR;
					}
					if (tmp == '*') {
						next = ifs.get();
						if (next == EOF) {
							error(SYNTAXERROR, ifs);
							return SYNTAXERROR;
						}
						if (next == '/') break;
					}
					if (tmp == '/') innerComment = true;
				}
				next = '\0';
				continue;
			}
			if (tmp != '/' && next == '=') {
				dw.push_back(next);
				next = '\0';
			}
			else if (tmp == '!') {
				error(SYNTAXERROR, ifs, -1);
				return SYNTAXERROR;
			}
			outputDoubleWord(dw);
			continue;
		}
		
		if (('a' <= tmp && tmp <= 'z') ||
			('A' <= tmp && tmp <= 'Z') ||
			('0' <= tmp && tmp <= '9') || tmp == '_') id.push_back(tmp);
		else {
			error(SYNTAXERROR, ifs);
			return SYNTAXERROR;
		}
	}
	ofstream ofs;
	ofs.open(pathOut.c_str());

	output(ofs);
	ifs.close();
	ofs.flush();
	ofs.close();

	return 0;
}
