#include "LLNRec.h"

void LLNRec::initGramma()
{
	begin = "E";
	gramma["E"] = vector<string>{ "T E'" };
	gramma["E'"] = vector<string>{ "+ T E'", E };
	gramma["T"] = vector<string>{ "F T'" };
	gramma["T'"] = vector<string>{ "* F T'", E };
	gramma["F"] = vector<string>{ "( E )", "ID" };

	for (map<string, vector<string> >::iterator it = gramma.begin(); it != gramma.end(); it++) {
		first[it->first] = set<string>();
		follow[it->first] = set<string>();
		predictTable[it->first] = map<string, pair<string, vector<string> > >();
	}
}

set<string> LLNRec::findFirst(string now)
{
	//cout << "now=" << now << endl;
	if (gramma.find(now) == gramma.end()) {
		return set<string>{now};
	}
	if (!first[now].empty()) return first[now];
	for (string right : gramma[now]) {
		//cout << "right=" << right << endl;
		string tmpFirst = "";
		for (char c : right) {
			if (c == ' ') break;
			tmpFirst.push_back(c);
		}
		for (string s : findFirst(tmpFirst)) {
			first[now].insert(s);
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

vector<string> LLNRec::getRightItems(string& right)
{
	vector<string> rightItems;
	right.push_back(' ');
	string tmpRight = "";
	for (char c : right) {
		if (c == ' ') {
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
				set<string> tmpFirst = follow[it->first];
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

					if (first[rightItems[i]].find(E) == first[rightItems[i]].end()) tmpFirst.clear();
					else {
						set_union(first[rightItems[i]].begin(), first[rightItems[i]].end(), tmpFirst.begin(), tmpFirst.end(), inserter(tmpFirst, tmpFirst.begin()));
						tmpFirst.erase(E);
					}
				}
			}
		}
		if (!changed) break;
	}
}

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
	cout << "Ô¤²â·ÖÎö±í" << endl;
	vector<string> sign;
	cout << setw(10) << "";
	for (auto s: inputSign) {
		sign.push_back(s);
		cout << setw(14) << left << s;
	}
	cout << endl;
	for (it_msv it = gramma.begin(); it != gramma.end(); it ++) {
		cout << setw(10) << left << it->first;
		for (auto s: sign) {
			pair<string, vector<string> > p = predictTable[it->first][s];
			stringstream ss;
			if(p.first.size()) ss << p.first << "->" << p.second;
			cout << setw(14) << left << ss.str();
		}
		cout << endl;
	}
}

LLNRec::LLNRec(string &lexerOut) {
	lexer.open(lexerOut.c_str());
	if (!lexer) throw "Cannot open file " + lexerOut;
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
#endif // DEBUG_LLNRC
	showTable();
}