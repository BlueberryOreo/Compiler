#pragma once
#include "util.h"
#include "Node.h"

// 不确定有限自动机
class NFA
{
private:
	static int id;
	static int getId();

	static NFA meta(RNode *node); // 有终结符构造
	static NFA Concatenation(NFA &nfa1, NFA &nfa2); // 连接
	static NFA Union(NFA &nfa1, NFA &nfa2); // 并
	static NFA Kleene(NFA &nfa); // 闭包
public:
	ANode* start; // 起始节点
	ANode* end; // 结束节点
	static NFA MSE(RNode *now); // 由正则表达式的语法分析树构建自动机
	void outputNFA();
};

