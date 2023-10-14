#include "util.h"

int priority_level(char a) //��������������ȼ������ȼ����壺�հ���*��>���ӷ���.��>�����㣨|��>����
{
    if (a == '(' || a == ')')
        return 1;
    else if (a == '|')
        return 2;
    else if (a == '.')
        return 3;
    else if (a == '*')
        return 4;
    else
        return 5;
}

bool is_operand(char a) //�ж��Ƿ����������������ֺ���ĸ����������
{
    if (a >= '0' && a <= '9')
        return true;
    if (a >= 'a' && a <= 'z')
        return true;
    if (a >= 'A' && a <= 'Z')
        return true;
    return false;
}

/*ʹ��a��b��c����������������������֮���ʡ�����ӷ���
*a *( ab a( )(
*/

string infixToPostfix(const string& infix_expression) //����׺ת��Ϊ��׺�����Ҳ�������׺��ʡ�Ե����ӷ�
{
    char temp[255] = {0};
    int j = 0;
    stack<char> st;
    for (int i = 0; i < infix_expression.size(); i++)
    {
        if (is_operand(infix_expression[i]))
        {
            temp[j++] = infix_expression[i];
        }
        else
        {
            if (st.empty())
            {
                st.push(infix_expression[i]);
            }
            else
            {
                char a = st.top();
                if (priority_level(infix_expression[i]) <= priority_level(a))
                {
                    while (priority_level(a) >= priority_level(infix_expression[i]))
                    {
                        if (a != '(' && a != ')')
                            temp[j++] = a;
                        st.pop();
                        if (st.empty())
                            break;
                        else
                            a = st.top();
                    }
                    st.push(infix_expression[i]);
                }
                else
                {
                    st.push(infix_expression[i]);
                }
            }
        }
        if (i < infix_expression.size() - 1) //�ж���һ�������ǲ������ӷ�
            if ((is_operand(infix_expression[i]) && infix_expression[i + 1] == '(') ||
                (is_operand(infix_expression[i]) && is_operand(infix_expression[i + 1])) ||
                (infix_expression[i] == '*' && is_operand(infix_expression[i + 1])) ||
                (infix_expression[i] == '*' && infix_expression[i + 1] == '(') ||
                (infix_expression[i] == ')' && infix_expression[i + 1] == '('))
            {
                if (st.empty())
                {
                    st.push('.');
                }
                else
                {
                    char a = st.top();
                    if (priority_level('.') <= priority_level(a))
                    {
                        while (priority_level(a) >= priority_level('.'))
                        {
                            if (a != '(' && a != ')')
                                temp[j++] = a;
                            st.pop();
                            if (st.empty())
                                break;
                            else
                                a = st.top();
                        }
                        st.push('.');
                    }
                    else
                    {
                        st.push('.');
                    }
                }
            }
    }
    while (!st.empty())
    {
        if (st.top() != '(' && st.top() != ')')
            temp[j++] = st.top();
        st.pop();
    }
    string temp0(temp);
    return temp0;
}