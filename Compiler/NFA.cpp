#include "NFA.h"

int NFA::id = 0;

int NFA::getId()
{
	return id++;
}

NFA NFA::meta(RNode* node)
{
	NFA nfa{};
	nfa.start = new ANode(getId());
	nfa.end = new ANode(getId());
	nfa.start->append(node->data, nfa.end);
	return nfa;
}

NFA NFA::Concatenation(NFA &nfa1, NFA &nfa2)
{
	NFA ret{};
	for (int i = 0; i < nfa2.start->edges.size(); i ++) {
		// 将nfa2.start的所有边接到nfa1.end上
		nfa1.end->edges.push_back(nfa2.start->edges[i]);
	}
	//nfa2.end->id = nfa2.start->id;
	delete nfa2.start;
	ret.start = nfa1.start;
	ret.end = nfa2.end;
	return ret;
}

NFA NFA::Union(NFA& nfa1, NFA& nfa2)
{
	NFA ret{};
	ret.start = new ANode(getId());
	ret.end = new ANode(getId());
	ret.start->append(E, nfa1.start);
	ret.start->append(E, nfa2.start);
	nfa1.end->append(E, ret.end);
	nfa2.end->append(E, ret.end);
	return ret;
}

NFA NFA::Kleene(NFA& nfa)
{
	NFA ret{};
	ret.start = new ANode(getId());
	ret.end = new ANode(getId());
	ret.start->append(E, nfa.start);
	nfa.end->append(E, nfa.start);
	ret.start->append(E, ret.end);
	nfa.end->append(E, ret.end);
	return ret;
}

NFA NFA::MSE(RNode* now)
{
	if (now->children.size() == 0) {
		// 是终结符，直接构造自动机
		if (now->data == "|" || now->data == "*" || now->data == "(" || now->data == ")") return NFA();
		cout << "meta" << " " << now->data << endl;
		return meta(now);
	}
	int childNum = now->children.size();
	vector<NFA> nfas;
	char type = 'c'; // 构造类型，c - 连接，u - 并，k - 闭包
	for (int i = 0; i < childNum; i ++) {
		RNode* child = now->children[i];
		if (child->data == "|") {
			type = 'u';
			continue;
		}
		else if (child->data == "*") {
			type = 'k';
			continue;
		}
		else if (child->data == "(" || child->data == ")") continue; // 是括号，略过
		nfas.push_back(MSE(child));
	}

	if (nfas.size() == 1 && type != 'k') return nfas[0];

	if (type == 'c') {
		cout << type << " " << nfas.size() << endl;
		return Concatenation(nfas[0], nfas[1]);
	}
	else if (type == 'u') {
		cout << type << " " << nfas.size() << endl;
		return Union(nfas[0], nfas[1]);
	}
	else {
		cout << type << " " << nfas.size() << endl;
		return Kleene(nfas[0]);
	}
}

void NFA::outputNFA()
{
	queue<ANode*> q;
	q.push(start);
	map<int, int> visited;
	while (!q.empty()) {
		ANode* now = q.front();
		q.pop();
		if (visited[now->id]) continue;
		visited[now->id] = 1;
		cout << now->id << endl;
		for (int i = 0; i < now->edges.size(); i ++) {
			ANode* next = now->edges[i].to;
			if (visited[next->id]) continue;
			q.push(next);
		}
	}
}

// 删除时有问题：两个指针指向同一个节点时，第一个先删了之后，第二个会出现野指针问题
void NFA::delNFA(ANode* delStart)
{
	queue<ANode*> q;
	q.push(delStart);
	while (!q.empty()) {
		ANode* now = q.front();
		q.pop();
		if (!now) continue;
		for (int i = 0; i < now->edges.size(); i ++) {
			q.push(now->edges[i].to);
		}
		delete now;
		now = NULL;
	}
}
