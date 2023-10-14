#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <stack>
#include <string>
#include <initializer_list>
#include <iomanip>
using namespace std;

#define SUCCESS 0
#define FILENOTFOUND 1
#define WORDERROR 2

#define KEYWORDNUM 8

string infixToPostfix(const string& regex); // ÒÑÆúÓÃ
