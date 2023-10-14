#include "TESTscan2.h"

TESTscan2::TESTscan2()
{
	regId = regNum = NULL;
	ifstream id, number;
	id.open("./id.tree");
	number.open("./number.tree");
	if (!id) throw "letter.tree not found";
	if (!number) throw "number.tree not found";

	regId = new RegExp("ID");
	regNum = new RegExp("NUM");
	regId->readFromFile(id);
	regNum->readFromFile(number);
	regId->init();
	regNum->init();
	id.close();
	number.close();
}

TESTscan2::~TESTscan2()
{
	if (regId) delete regId;
	if (regNum) delete regNum;
}

int TESTscan2::scan(string& input_file, string& output_file)
{
	ifstream ifs;
	ifs.open(input_file.c_str());
	if (!ifs) return 1;

	char now = getch(ifs), next = getch(ifs);
	string tmp;
	//while (true) {
	//	
	//}
	output();

	return 0;
}
