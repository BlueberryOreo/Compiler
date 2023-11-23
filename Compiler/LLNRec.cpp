#include "LLNRec.h"

void LLNRec::initGrammar()
{
	// 输入语法要求：一个产生式的右侧的每个元素用空格隔开
	begin = "E";
	grammar["E"] = vector<string>{ "T E'" };
	grammar["E'"] = vector<string>{ "+ T E'", E };
	grammar["T"] = vector<string>{ "F T'" };
	grammar["T'"] = vector<string>{ "* F T'", E };
	grammar["F"] = vector<string>{ "( E )", "id" };

	//begin = "<program>";
	//grammar["<program>"] = vector<string>{ "{  <declaration_list>  <statement_list>  }" };
	//grammar["<declaration_list>"] = vector<string>{ "<declaration_list_R> " };
	//grammar["<declaration_list_R>"] = vector<string>{ " <declaration_stat>  <declaration_list_R>", E };
	//grammar["<declaration_stat>"] = vector<string>{"int ID ;"};
	//grammar["<statement_list>"] = vector<string>{ "<statement_list_R> " };
	//grammar["<statement_list_R>"] = vector<string>{ " <statement>  <statement_list_R>", E };
	//grammar["<statement>"] = vector<string>{" <if_stat> ", " <while_stat>", "<for_stat>", "<read_stat>", "<write_stat>", "<compound_stat>", "<expression_stat> "};
	//grammar["<if_stat>"] = vector<string>{"if ( <expression> ) <statement> <else_part>"};
	//grammar["<else_part>"] = vector<string>{"else <statement>", E};
	//grammar["<while_stat>"] = vector<string>{"while ( <expression> )  <statement> "};
	//grammar["<for_stat>"] = vector<string>{"for (   <expression>   ;   <expression>   ;   <expression>   )  <statement> "};
	//grammar["<write_stat>"] = vector<string>{"write  <expression> ;"};
	//grammar["<read_stat>"] = vector<string>{"read ID ;"};
	//grammar["<compound_stat>"] = vector<string>{"{ <statement_list> }"};
	//grammar["<expression_stat>"] = vector<string>{" <expression> ;", ";"}; 
	//grammar["<expression>"] = vector<string>{"ID = <bool_expr>", "<bool_expr>"}; // 问题
	//grammar["<bool_expr>"] = vector<string>{"<additive_expr> <bool_expr_right>"};
	//grammar["<bool_expr_right>"] = vector<string>{"> <additive_expr>", "< <additive_expr>", 
	//	">= <additive_expr>", "<= <additive_expr>", "== <additive_expr>", "!= <additive_expr>", E};
	//grammar["<additive_expr>"] = vector<string>{ "<term> <additive_expr_right>"};
	//grammar["<additive_expr_right>"] = vector<string>{"+ <term>", "- <term>", E};
	//grammar["<term>"] = vector<string>{ "<factor> <term_right>" };
	//grammar["<term_right>"] = vector<string>{"* <factor>", "/ <factor>", E};
	//grammar["<factor>"] = vector<string>{"( <expression> )", "ID", "NUM"};

	for (map<string, vector<string> >::iterator it = grammar.begin(); it != grammar.end(); it++) {
		first[it->first] = set<string>();
		follow[it->first] = set<string>();
		predictTable[it->first] = map<string, pair<string, vector<string> > >();
	}
}

// 递归构建FIRST集
set<string> LLNRec::findFirst(string now)
{
	//cout << "now=" << now << endl;
	if (grammar.find(now) == grammar.end()) {
		return set<string>{now};
	}
	if (!first[now].empty()) return first[now]; // 剪枝
	for (string right : grammar[now]) {
		//cout << "right=" << right << endl;
		vector<string> rightItems = getRightItems(right);
		bool allEps = true;
		for (string r: rightItems) {
			bool hasEps = false;
			for (string item: findFirst(r)) {
				if (item == E) {
					hasEps = true;
					break;
				}
				first[now].insert(item);
			}
			if (!hasEps) {
				allEps = false;
				break;
			}
			if (allEps) first[now].insert(E);
		}
	}
	return first[now];
}

void LLNRec::initFirst()
{
	for (it_msv it = grammar.begin(); it != grammar.end(); it ++) {
		findFirst(it->first);
	}
}

// 解析产生式右侧字符串
vector<string> LLNRec::getRightItems(string& right)
{
	vector<string> rightItems;
	right.push_back(' ');
	string tmpRight = "";
	for (char c : right) {
		if (c == ' ') {
			if (tmpRight.size() == 0) continue;
			rightItems.push_back(tmpRight);
			tmpRight.clear();
		}
		else {
			tmpRight.push_back(c);
		}
	}
	return rightItems;
}

void LLNRec::initFollow()
{
	follow[begin].insert("$");
	inputSign.insert("$");
	while (true) {
		bool changed = false;
		for (it_msv it = grammar.begin(); it != grammar.end(); it++) {
			//cout << it->first << "->" << it->second << endl;
			for (string right : it->second) {
				vector<string> rightItems = getRightItems(right);
				//cout << it->first << " rightItems=" << rightItems << endl; // here
				set<string> tmpFirst = follow[it->first]; // 连续空时FIRST的继承
				// 反向遍历构造follow，以处理空
				for (int i = rightItems.size() - 1; i >= 0; i--) {
					if (grammar.find(rightItems[i]) == grammar.end()) {
						inputSign.insert(rightItems[i]);
						tmpFirst.clear();
						tmpFirst.insert(rightItems[i]);
						continue;
					}
					int lastLen = follow[rightItems[i]].size();
					if ((i < rightItems.size() - 1) && (grammar.find(rightItems[i + 1]) != grammar.end())) {
						for (auto item : first[rightItems[i + 1]]) {
							if (item != E) follow[rightItems[i]].insert(item);
						}
					}

					set_union(follow[rightItems[i]].begin(), follow[rightItems[i]].end(), tmpFirst.begin(), tmpFirst.end(),
						inserter(follow[rightItems[i]], follow[rightItems[i]].begin()));

					if (follow[rightItems[i]].size() != lastLen) changed = true;

					if (first[rightItems[i]].find(E) == first[rightItems[i]].end()) tmpFirst.clear(); // 如果当前FIRST没有空，那么清除tmpFirst
					else {
						// 否则将当前的FIRST合并到tmpFirst中，以添加到下一个非终结符的FOLLOW中
						set_union(first[rightItems[i]].begin(), first[rightItems[i]].end(), tmpFirst.begin(), tmpFirst.end(), inserter(tmpFirst, tmpFirst.begin()));
						tmpFirst.erase(E);
					}
				}
			}
		}
		if (!changed) break;
	}
}

bool LLNRec::specialCheck(string left, string rightItems0, string terminal) {
	if (left == "<else_part>" && rightItems0 == E && terminal == "else") return true; // 处理悬空else
	if (left == "<expression>" && rightItems0 == "<bool_expr>" && terminal == "ID") return true;
	//if (left == "<bool_expr>" && rightItems0 == "<additive_expr>" && terminal == "NUM") return true;
	return false;
}

// 书算法4.31
void LLNRec::buildTable()
{
	for (it_msv it = grammar.begin(); it != grammar.end(); it ++) {
		for (auto right: it->second) {
			vector<string> rightItems = getRightItems(right);
			pair<string, vector<string> > p = { it->first, rightItems };
			//cout << rightItems << endl;
			if (grammar.find(rightItems[0]) == grammar.end()) predictTable[it->first][rightItems[0]] = p;

			bool hasEps = (rightItems[0] == E);
			for (auto terminal: first[rightItems[0]]) {
				//cout << it->first << " " << terminal << " " << it->second << endl;
				if (terminal == E) hasEps = true;
				else if(!specialCheck(it->first, rightItems[0], terminal)) predictTable[it->first][terminal] = p;
			}
			if (hasEps) {
				for (auto terminal : follow[it->first]) {
					if(!specialCheck(it->first, rightItems[0], terminal)) predictTable[it->first][terminal] = p; // 特别处理
				}
			}
		}
	}
}

void LLNRec::showTable()
{
	//cout << "预测分析表" << endl;
	//vector<string> sign;
	//cout << setw(10) << "";
	//for (auto s: inputSign) {
	//	sign.push_back(s);
	//	cout << setw(14) << left << s;
	//}
	//cout << endl;
	//for (it_msv it = grammar.begin(); it != grammar.end(); it ++) {
	//	cout << setw(10) << left << it->first;
	//	for (auto s: sign) {
	//		pair<string, vector<string> > p = predictTable[it->first][s];
	//		stringstream ss;
	//		if(p.first.size()) ss << p.first << "->" << p.second;
	//		cout << setw(14) << left << ss.str();
	//	}
	//	cout << endl;
	//}
	cout << "预测分析表" << endl;
	vector<string> sign;
	cout << "\\t";
	for (auto s : inputSign) {
		sign.push_back(s);
		cout << s << "\\t";
	}
	cout << endl;
	for (it_msv it = grammar.begin(); it != grammar.end(); it ++) {
		cout << it->first << "\\t";
		for (auto s: sign) {
			pair<string, vector<string> > p = predictTable[it->first][s];
			stringstream ss;
			if(p.first.size()) ss << p.first << "->" << p.second;
			cout << ss.str() << "\\t";
		}
		cout << endl;
	}
}

void LLNRec::initStk() {
	stk.push("$");
	stk.push(begin);
	nodeStk.push("Node0"); // $
	nodeStk.push("Node1"); // begin
	nodeCnt = 2;
#ifdef OUTPUT_PARSE_TREE
	cout << "graph Tree{" << endl;
#endif
}

void LLNRec::outputStk() {
	stack<string> tmp;
	while (!stk.empty()) {
		tmp.push(stk.top());
		stk.pop();
		cout << tmp.top() << " ";
	}
	cout << endl;
	while (!tmp.empty()) {
		stk.push(tmp.top());
		tmp.pop();
	}
}

void LLNRec::updatePoints(vector<string>& nextLayer, string &nowNode) {
	vector<string> outputLines;
	for (int i = nextLayer.size() - 1; i >= 0; i--) {
		if (nextLayer[i] != E) stk.push(nextLayer[i]);
		stringstream nodeNo;
		string line;
		nodeNo << "Node" << nodeCnt;
		nodeCnt++;
		line = nowNode + " -- " + nodeNo.str() + ";";
		outputLines.push_back(line);
		if (nextLayer[i] != E) nodeStk.push(nodeNo.str());
		else nodes[nodeNo.str()] = E;
	}
#ifdef OUTPUT_PARSE_TREE
	for (int i = outputLines.size() - 1; i >= 0; i--) {
		cout << outputLines[i] << endl;
	}
#endif
}

// 0 - don't move, 1 - move next, 2 - error1, 3 - error2, -1 - go back
int LLNRec::move(Token &t)
{
	//cout << stk.top() << " " << t << endl;
	if (stk.top() == "<expression>") {
		// expression特判，向前读一个再决定产生式
		if (t.first == "ID") {
			isIdExpr = true;
			return 1;
		}

		string nowNode = nodeStk.top();
		nodeStk.pop();
		nodes[nowNode] = stk.top();
		stk.pop();

		vector<string> p;
		if (isIdExpr && t.first == "=") {
#ifndef OUTPUT_PARSE_TREE
			cout << "输出<expression>->ID = <bool_expr>" << endl;
#endif
			p.push_back("ID");
			p.push_back("=");
			p.push_back("<bool_expr>");
		}
		else {
#ifndef OUTPUT_PARSE_TREE
			cout << "输出<expression>-><bool_expr>" << endl;
#endif
			p.push_back("<bool_expr>");
		}

		updatePoints(p, nowNode);
		if (isIdExpr) {
			isIdExpr = false;
			return -1;
		}
		return 0;
	}
	if (stk.top() == t.first) {
#ifndef OUTPUT_PARSE_TREE
		cout << "匹配" << stk.top() << endl;
#endif
		stk.pop();
		nodes[nodeStk.top()] = t.first;
		nodeStk.pop();
		return 1;
	}
	if (grammar.find(stk.top()) == grammar.end()) return 2;
	if (predictTable[stk.top()][t.first].first.size() == 0) return 3;
	pair<string, vector<string> > p = predictTable[stk.top()][t.first];
#ifndef OUTPUT_PARSE_TREE
	cout << "输出" << p.first << "->" << p.second << endl;
#endif
	string nowNode = nodeStk.top();
	nodeStk.pop();
	nodes[nowNode] = stk.top();

	stk.pop();

	updatePoints(p.second, nowNode);
	return 0;
}

LLNRec::LLNRec() {
	initGrammar();
	initFirst();
	initFollow();
	buildTable();
#ifdef DEBUG_LLNRC
	for (it_mss it = first.begin(); it != first.end(); it++) {
		cout << "FIRST(" << it->first << ") = " << it->second << endl;
	}

	for (it_mss it = follow.begin(); it != follow.end(); it ++) {
		cout << "FOLLOW(" << it->first << ") = " << it->second << endl;
	}
	//showTable();
#endif // DEBUG_LLNRC
}

vector<Token> LLNRec::getTokensFromFile(string& lexerOut) {
	ifstream lexerIfs;
	lexerIfs.open(lexerOut.c_str());
	if (!lexerIfs) throw "Cannot open file " + lexerOut;
	vector<Token> ret;
	Token now;
	while (lexerIfs >> now) {
		ret.push_back(now);
	}
	ret.push_back(Token{"$", "$", now.pos + (int)now.second.size(), now.line});
	return ret;
}

int LLNRec::analyze(string &sourcePath, string& lexerOut) {
	vector<Token> tokens = getTokensFromFile(lexerOut);
	int res = 1;
	try {
		res = analyze(tokens);
	}
	catch (Token errorToken) {
		string info = "Unexpected token get \"" + errorToken.second + "\"";
		ifstream ifs(sourcePath);
		error(SYNTAXERROR, &ifs, errorToken.pos - 2, info, errorToken.line);
		ifs.close();
	}
	return res;
}

int LLNRec::analyze(vector<Token> &tokens) {
	initStk();
	int idx = 0;
	while (!stk.empty()) {
		int res = move(tokens[idx]);
		if (res == 1) idx++;
		else if (res >= 2) {
			//cout << "error" << res - 1 << " " << tokens[idx] << endl;
			throw tokens[idx];
		}
		else if (res == -1) idx--;
	}

#ifdef OUTPUT_PARSE_TREE
	for (map<string, string>::iterator it = nodes.begin(); it != nodes.end(); it ++) {
		if (it->second == "$") continue;
		cout << it->first << " [label=\"" << it->second << "\"];" << endl;
	}
	cout << "}" << endl;
#endif
	return 0;
}