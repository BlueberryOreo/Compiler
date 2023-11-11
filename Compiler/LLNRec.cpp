#include "LLNRec.h"

void LLNRec::initGramma()
{
	//begin = "E";
	// 输入语法要求：一个产生式的右侧的每个元素用空格隔开
	//gramma["E"] = vector<string>{ "T E'" };
	//gramma["E'"] = vector<string>{ "+ T E'", E };
	//gramma["T"] = vector<string>{ "F T'" };
	//gramma["T'"] = vector<string>{ "* F T'", E };
	//gramma["F"] = vector<string>{ "( E )", "id" };

	begin = "<program>";
	gramma["<program>"] = vector<string>{ "{  <declaration_list>  <statement_list>  }" };
	gramma["<declaration_list>"] = vector<string>{ "ε <declaration_list_r> " };
	gramma["<declaration_list_r>"] = vector<string>{ " <declaration_state>  <declaration_list_r>", "ε" };
	gramma["<declaration_stat>"] = vector<string>{"int ID ;"};
	gramma["<statement_list>"] = vector<string>{ "ε <statement_list_R> " };
	gramma["<statement_list_R>"] = vector<string>{ " <statement>  <statement_list_R>", "ε" };
	gramma["<statement>"] = vector<string>{" <if_stat> ", " <while_stat>", "<for_stat>", "<read_stat>", "<write_stat>", "<compound_stat>", "< expression_stat> "};
	//gramma["<if_stat>"] = vector<string>{"if ( <expression> ) <statement> [ else <statement> ]"};
	gramma["<while_stat>"] = vector<string>{"while ( <expression> )  <statement> "};
	gramma["<for_stat>"] = vector<string>{"for (   <expression>   ;   <expression>   ;   <expression>   )  <statement> "};
	gramma["<write_stat>"] = vector<string>{"write  <expression> ;"};
	gramma["<read_stat>"] = vector<string>{"read ID ;"};
	gramma["<compound_stat>"] = vector<string>{"{ <statement_list> }"};
	gramma["<expression_stat>"] = vector<string>{" <expression> ;", ";"}; 
	gramma["<expression>"] = vector<string>{"ID = <bool_expr>", "<bool_expr>  "};
	gramma["<bool_expr>"] = vector<string>{"<additive_expr>", "<additive_expr> > <additive_expr>", "<additive_expr> < <additive_expr>",
	"<additive_expr> >= <additive_expr>", "<additive_expr> <= <additive_expr>", "<additive_expr> == <additive_expr>", "<additive_expr> != <additive_expr>"};
	gramma["<additive_expr>"] = vector<string>{ "<term> + <term>", "<term> - <term>" };
	gramma["<term>"] = vector<string>{ "<factor> * <factor>", "<factor> / <factor>" };
	gramma["<factor>"] = vector<string>{"( <expression> )", "ID", "NUM"};

	for (map<string, vector<string> >::iterator it = gramma.begin(); it != gramma.end(); it++) {
		first[it->first] = set<string>();
		follow[it->first] = set<string>();
		predictTable[it->first] = map<string, pair<string, vector<string> > >();
	}
}

// 递归构建FIRST集
set<string> LLNRec::findFirst(string now)
{
	//cout << "now=" << now << endl;
	if (gramma.find(now) == gramma.end()) {
		return set<string>{now};
	}
	if (!first[now].empty()) return first[now]; // 剪枝
	for (string right : gramma[now]) {
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
	for (it_msv it = gramma.begin(); it != gramma.end(); it ++) {
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
		for (it_msv it = gramma.begin(); it != gramma.end(); it++) {
			//cout << it->first << "->" << it->second << endl;
			for (string right : it->second) {
				vector<string> rightItems = getRightItems(right);
				//cout << it->first << " rightItems=" << rightItems << endl; // here
				set<string> tmpFirst = follow[it->first]; // 连续空时FIRST的继承
				// 反向遍历构造follow，以处理空
				for (int i = rightItems.size() - 1; i >= 0; i--) {
					if (gramma.find(rightItems[i]) == gramma.end()) {
						inputSign.insert(rightItems[i]);
						tmpFirst.clear();
						tmpFirst.insert(rightItems[i]);
						continue;
					}
					int lastLen = follow[rightItems[i]].size();
					if ((i < rightItems.size() - 1) && (gramma.find(rightItems[i + 1]) != gramma.end())) {
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

// 书算法4.31
void LLNRec::buildTable()
{
	for (it_msv it = gramma.begin(); it != gramma.end(); it ++) {
		for (auto right: it->second) {
			vector<string> rightItems = getRightItems(right);
			pair<string, vector<string> > p = { it->first, rightItems };
			//cout << rightItems << endl;
			if (gramma.find(rightItems[0]) == gramma.end()) predictTable[it->first][rightItems[0]] = p;

			bool hasEps = (rightItems[0] == E);
			for (auto terminal: first[rightItems[0]]) {
				//cout << it->first << " " << terminal << " " << it->second << endl;
				if (terminal == E) hasEps = true;
				else predictTable[it->first][terminal] = p;
			}
			if (hasEps) {
				for (auto terminal : follow[it->first]) predictTable[it->first][terminal] = p;
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
	//for (it_msv it = gramma.begin(); it != gramma.end(); it ++) {
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
	for (it_msv it = gramma.begin(); it != gramma.end(); it ++) {
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
	cout << "graph Tree{" << endl;
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

// 0 - don't move, 1 - move next, 2 - error1, 3 - error2
int LLNRec::move(Token &t)
{
	if (stk.top() == t.first) {
		//cout << "匹配" << stk.top() << endl;
		stk.pop();
		nodes[nodeStk.top()] = t.first;
		nodeStk.pop();
		return 1;
	}
	if (gramma.find(stk.top()) == gramma.end()) return 2;
	if (predictTable[stk.top()][t.first].first.size() == 0) return 3;
	pair<string, vector<string> > p = predictTable[stk.top()][t.first];
	//cout << p.first << "->" << p.second << endl;

	string nowNode = nodeStk.top();
	nodeStk.pop();
	nodes[nowNode] = stk.top();

	stk.pop();

	vector<string> outputLines;
	for (int i = p.second.size() - 1; i >= 0; i --) {
		if (p.second[i] != E) stk.push(p.second[i]);
		stringstream nodeNo;
		string line;
		nodeNo << "Node" << nodeCnt;
		nodeCnt++;
		line = nowNode + " -- " + nodeNo.str() + ";";
		outputLines.push_back(line);
		if (p.second[i] != E) nodeStk.push(nodeNo.str());
		else nodes[nodeNo.str()] = E;
	}
	for (int i = outputLines.size() - 1; i >= 0; i --) {
		cout << outputLines[i] << endl;
	}
	return 0;
}

LLNRec::LLNRec() {
	initGramma();
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
	showTable();
#endif // DEBUG_LLNRC
}

void LLNRec::analyze(string& lexerOut) {
	initStk();
}

void LLNRec::analyze(vector<Token> &tokens) {
	initStk();
	int idx = 0;
	while (!stk.empty()) {
		int res = move(tokens[idx]);
		if (res == 1) idx++;
		else if (res >= 2) {
			cout << "error" << res - 1 << endl;
			break;
		}
	}

	for (map<string, string>::iterator it = nodes.begin(); it != nodes.end(); it ++) {
		if (it->second == "$") continue;
		cout << it->first << " [label=\"" << it->second << "\"];" << endl;
	}
	cout << "}" << endl;
}