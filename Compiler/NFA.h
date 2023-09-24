#pragma once
#include "util.h"
#include "Node.h"

// 不确定有限自动机
class NFA
{
private:
	static int id;
	static int getId();

	static NFA meta(RNode *node); // 由终结符构造
	static NFA Concatenation(NFA &nfa1, NFA &nfa2); // 连接
	static NFA Union(NFA &nfa1, NFA &nfa2); // 并
	static NFA Kleene(NFA &nfa); // 闭包
public:
	NNode* start; // 起始节点
	NNode* end; // 结束节点
	int size;
	NFA(int size=0, NNode* start=NULL, NNode* end=NULL);
	static NFA MYT(RNode *now, set<string> &input); // 由正则表达式的语法分析树构建自动机
	void outputNFA();
	static void delNFA(NNode*delStart);
};

