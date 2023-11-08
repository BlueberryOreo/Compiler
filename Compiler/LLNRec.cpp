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

void LLNRec::initFollow()
{
	follow[begin].insert("$");
	while (true) {
		bool changed = false;
		for (it_msv it = gramma.begin(); it != gramma.end(); it++) {
			//cout << it->first << "->" << it->second << endl;
			for (string right : it->second) {
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
				//cout << it->first << " rightItems=" << rightItems << endl; // here
				set<string> tmpFirst = follow[it->first];
				for (int i = rightItems.size() - 1; i >= 0; i--) {
					if (gramma.find(rightItems[i]) == gramma.end()) {
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

LLNRec::LLNRec(string &lexerOut) {
	lexer.open(lexerOut.c_str());
	if (!lexer) throw "Cannot open file " + lexerOut;
	initGramma();
	initFirst();
	initFollow();
#ifdef DEBUG_LLNRC
	for (it_mss it = first.begin(); it != first.end(); it++) {
		cout << "FIRST(" << it->first << ") = " << it->second << endl;
	}

	for (it_mss it = follow.begin(); it != follow.end(); it ++) {
		cout << "FOLLOW(" << it->first << ") = " << it->second << endl;
	}
#endif // DEBUG_LLNRC

}