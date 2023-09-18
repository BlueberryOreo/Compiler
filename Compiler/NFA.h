#pragma once
#include "util.h"
#include "Node.h"

// ��ȷ�������Զ���
class NFA
{
private:
	static int id;
	static int getId();

	static NFA meta(RNode *node); // ���ս������
	static NFA Concatenation(NFA &nfa1, NFA &nfa2); // ����
	static NFA Union(NFA &nfa1, NFA &nfa2); // ��
	static NFA Kleene(NFA &nfa); // �հ�
public:
	ANode* start; // ��ʼ�ڵ�
	ANode* end; // �����ڵ�
	static NFA MSE(RNode *now); // ���������ʽ���﷨�����������Զ���
	void outputNFA();
	static void delNFA(ANode *delStart);
};
