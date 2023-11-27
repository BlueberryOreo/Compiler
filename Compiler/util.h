#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <string>
#include <initializer_list>
#include <iomanip>
using namespace std;

#define SUCCESS 0
#define FILENOTFOUND 1
#define WORDERROR 2
#define SYNTAXERROR 3

#define KEYWORDNUM 8

#define nDEBUG_LL
#define nDEBUG_LLNRC
#define nOUTPUT_PREDICT_TABLE
#define nOUTPUT_PARSE_TREE

// 词法单元
struct Token {
	string first, second;
	int pos, line;
};

string infixToPostfix(const string& regex); // 已弃用
