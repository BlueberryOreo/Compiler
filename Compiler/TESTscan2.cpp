#include "TESTscan2.h"

TESTscan2::TESTscan2()
{
	regId = regNum = NULL;
	ifstream id, number, single, comp_single;
	id.open("./id.tree");
	number.open("./number.tree");
	single.open("./single.tree");
	comp_single.open("./comp_single.tree");
	//comment.open("./comment.tree");
	if (!id) throw "id.tree not found";
	if (!number) throw "number.tree not found";
	if (!single) throw "single.tree not found";
	if (!comp_single) throw "comp_single.tree not found";
	//if (!comment) throw "comment.tree not found";

	// 构建各自动机
	regId = new RegExp("ID");
	regNum = new RegExp("NUM");
	regSingle = new RegExp("SINGLE");
	regDouble = new RegExp("DOUBLE");
	//regCom = new RegExp("COMMENT");
	regId->readFromFile(id);
	regNum->readFromFile(number);
	regSingle->readFromFile(single);
	regDouble->readFromFile(comp_single);
	//regCom->readFromFile(comment);
	regId->init();
	regNum->init();
	regSingle->init();
	regDouble->init();
	//regCom->init();
	id.close();
	number.close();
	single.close();
	comp_single.close();
	//comment.close();
}

TESTscan2::~TESTscan2()
{
	if (regId) delete regId;
	if (regNum) delete regNum;
	if (regSingle) delete regSingle;
	if (regDouble) delete regDouble;
	//if (regCom) delete regCom;
}

int TESTscan2::scan(string& input_file, string& output_file)
{
	ifstream ifs;
	ifs.open(input_file.c_str());
	if (!ifs) return 1;

	char now = getch(ifs), next;
	string tmp = "";
	int state = 0; // 1 - number, 2 - id, 3 - /, 4 - others, 5 - comments
	while (true) {
		if (now == EOF) break;
		next = getch(ifs);
		if (!state) {
			if (now == ' ' || now == '\n' || now == '\t') {
				now = next;
				continue;
			}
			if ('0' <= now && now <= '9') state = 1;
			else if (('a' <= now && now <= 'z') || ('A' <= now && now <= 'Z')) state = 2;
			else if (now == '/') state = 3;
			else state = 4;
		}

		bool res[2] = { false, false };
		if (state == 1) {
			//cout << now << " " << next << endl;
			res[0] = regNum->matchNext(now);
			res[1] = regNum->matchNext(next);
			tmp.push_back(now);
			if (res[0] && !res[1]) {
				outputNum(tmp);
				tmp = "";
				state = 0;
			}
		}
		else if (state == 2) {
			res[0] = regId->matchNext(now);
			res[1] = regId->matchNext(next);
			tmp.push_back(now);
			if (res[0] && !res[1]) {
				bool iskw = false;
				for (int i = 0; i < KEYWORDNUM; i ++) {
					if (tmp == keyword[i]) {
						iskw = true;
						break;
					}
				}
				if (iskw) outputKw(tmp);
				else testId(tmp);
				tmp = "";
				state = 0;
			}
		}
		else if(state == 3){
			if (next == '*') state = 5;
			else {
				outputSingleWord(now);
				state = 0;
			}
		}
		else if (state == 4) {
			if (regSingle->matchNext(now)) outputSingleWord(now);
			else if (regDouble->matchNext(now)) {
				string dw(1, now);
				if (next == '=') {
					dw.push_back(next);
					next = getch(ifs);
				}
				outputDoubleWord(dw);
			}
			else {
				error(WORDERROR, &ifs, -1);
				return 1;
			}
			state = 0;
		}
		else {
			if (now == '*' && next == '/') {
				state = 0;
				next = getch(ifs);
			}
		}
		now = next;
	}
	if (state != 0) return 1;
	ofstream ofs;
	ofs.open(output_file.c_str());
	if (!ofs) return 1;
	output(ofs);
	ifs.close();
	ofs.close();

	return 0;
}
