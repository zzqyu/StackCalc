#include <cstdlib>    // Provides EXIT_SUCCESS
#include <cstring>    // Provides strchr
#include <string>
#include <stack>	//stack
#include <iostream>
#include <vector>
#include <sstream> 
#include <cmath>

using namespace std;

//����� ��ȣ �迭 
const char SYMBOLS[] = { '^', '*', '/', '%', '+', '-', '(', ')' };
//�� �迭�� ������
const int SYMBOL_SIZE = 8;

//��ȣ �˻� �Լ�
bool is_balanced_parentheses(string s);
//infix���� postfix������ �ٲٴ� �Լ�
vector<string> infix_to_postfix(vector<string> v);
//postfix���� ����ϴ� �Լ�
bool postfix_calc(vector<string> v, int& result);
//������ �����ϴ� �Լ�
bool calc(int n1, int n2, char op, int& result);
//�� ��ȣ�� �켱������ �����ϴ� �Լ�
int order(string oper);
//��� �ּ� ������ ������ �ִ� �Լ�
vector<string> string_to_tok(string strTarget);
//���ڿ��� �Ϻ� ���ڸ� ��ü�� �����ϴ� �Լ�
string replaceAll(string str, const string &pattern, const string &replace);
//������ �а� �� ���� ���� ���� ����ϴ� �Լ�
bool read_and_evaluate();

int main() {
	while(read_and_evaluate());
	return 0;
}
bool read_and_evaluate(){
	//precondition: --
	//Postcondition: ������ �Է¹ް�, ��� ���
	//0�� �Է��ϸ� false, ����� �����ϸ� true
	string input_str;//�Է��� ������ ��� ����
	const string STR_ERROR = "Error!";//���� ��¹���
	int* answer = new int ;//���� ���
	cout<<"input >> ";
	cin >> input_str;//���� �Է� ����
	vector<string> v;//��� �ּ� ����(operand, operator, parentheses)��� ����
	if(input_str=="0") return false;//0�� �Է��ϸ� false
	if(!is_balanced_parentheses(input_str)){//��ȣ�˻�
		cout<<STR_ERROR<<endl;//���� ���
	}
	else{
		v = string_to_tok(input_str);//��� �ּ� ������ ��ȯ
		if(postfix_calc(infix_to_postfix(v), *answer))//���� ���
			cout<<"= "<< *answer <<endl;//����� ���
		else
			cout<<STR_ERROR<<endl;//���� ���
	}
	return true;
}

bool is_balanced_parentheses(string s) {
	//precondition: ����
	//Postcondition: ���� ��ȣ�˻�, ������ ������ ��� true
	stack<char> ps;//��ȣ�� ��� ����
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == SYMBOLS[6])//���� ��ȣ�� ���
			ps.push(SYMBOLS[6]);//���ÿ� �־��ش�.
		else if (s[i] == SYMBOLS[7]&&!ps.empty())//�ݴ� ��ȣ�� ���
			ps.pop();//���ÿ��� ����
	}
	return ps.empty();
}

bool calc(int n1, int n2, char op, int& result) {
	//precondition: operand 2��, operator, ����� ���� ����
	//n2�� 0�̰�, �����⳪ ������������ �����ϴ� �� Ȯ��
	//Postcondition: ��� ���� ����� result�� ���, true�� ����
	if(n2==0&&(op=='/'||op=='%'))
		return false;
	switch (op)
	{
	case '+': result= n1 + n2; break;
	case '-': result= n1 - n2;break;
	case '*': result= n1 * n2;break;
	case '/': result= n1 / n2;break;
	case '%': result= n1 % n2;break;
	case '^': result= pow(n1, n2);break;
	}
	return true;
}

bool postfix_calc(vector<string> v, int& result) {
	//precondition: ������ �ּ� ������ ���� ����, ��� ���� ���� ����
	//Postcondition: ��� ���� ����� result�� ���, true�� ����
	stack<int> st;
	int n2;
	for (int i = 0; i < v.size(); i++) {
		//operator�� ���
		if (order(v[i])>=0&&order(v[i])<=2&&!st.empty()) {
			n2 = st.top();
			st.pop();
			int* temp = new int;
			//���� �ֻ���� 2���� operand�� operator�� �´� ���� ����
			bool is_not_error = calc(st.top(), n2, v[i].c_str()[0], *temp);
			//���� ������ �߻��ϸ� false ����
			if(!is_not_error) return is_not_error;
			//������ ������ ���� �ֻ���� 2���� operand�� pop
			st.pop();
			//�� ����� ���� ���ÿ� �߰�
			st.push(*temp);
		}
		//operator�� �ƴ� ��쿡�� push�� �ش�.
		else {
			st.push(atoi(v[i].c_str()));
		}
	}
	//��������� stack���� ��� ��� ���� ���´�
	//�� ����� result�� ����
	result = st.top();
	//����� ���������� ���� ������ �˸��� true�� ����
	return true;
}
vector<string> infix_to_postfix(vector<string> v) {
	//precondition: ������ �ּ� ������ ���� ����
	//Postcondition: ������ ������ infix���� postfix���� �ٲ۴�.
	stack<string> s;
	vector<string> answer;
	for (int i = 0; i < v.size(); i++) {
		if (order(v[i]) == 6) {//���� ��ȣ�� ���
			s.push(v[i]);//���ÿ� �߰�
		}
		else if (order(v[i]) == 7) {//�ݴ°�ȣ�� ���
			//���� ��ȣ�� ���ö����� ���Ϳ� �߰��ϰ�
			//���ÿ����� pop���ش�.
			while(!s.empty() && order(s.top())!=6){
				answer.push_back(s.top());
				s.pop();
			}
			s.pop();
		}
		else if(order(v[i]) == -1) {//operand�� ���
			answer.push_back(v[i]);//���Ϳ� �߰��Ѵ�.
		}
		else if (order(v[i])>=0&&order(v[i])<=2) {//operator�� ���
			if (!s.empty()&& order(s.top())!=6 && 
				((order(s.top()) < order(v[i]))||
				order(s.top()) == order(v[i]))) {
				//����켱������ ���� ���� ��
				answer.push_back(s.top());//���Ϳ� �߰��ϰ�
				s.pop();//���ÿ����� pop���ش�
			}
			s.push(v[i]);//���ÿ� �߰�
		}
		
	}
	while (!s.empty()) {//for�� �� ���� ������ ������� ���� ���
		answer.push_back(s.top());//���Ϳ� �߰��ϰ� ���ÿ����� pop���ش�.
		s.pop();
	}
	return answer;
}

int order(string oper) {
	//precondition: ������ �ּ� ����
	//Postcondition: ������ �ּ� ������ ��ȣ�� �ο��Ѵ�.
	int o = 3;
	int i = 0;
	string symbols(SYMBOLS);
	i=symbols.find(oper.c_str()[0]);
	if (i == 0) o = 0;//'^'�ϰ�� 0����
	else if (i>0 && i <= 3) o = 1;//*/%�� ��� 1����
	else if (i == 4 || i == 5) o = 2;//+-�� ��� 2����
	else if (i >= 6)o=i;//��ȣ�� �� Symbols�� �ε��� �ο�
	else o=-1; //operand�� -1
	return o;
}
string replaceAll(string str, const string &pattern, const string &replace){
	//precondition: ������ ���ڿ�, ���� ��� ���ڿ�, ��ü ���ڿ�
	//Postcondition: ���ڿ����� ���� ��� ���ڿ��� ã�Ƽ� ��ü ���ڿ��� �ٲپ� �ش�.
	string result = str;
	string::size_type pos = 0;
	string::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}
	return result;
}
vector<string> string_to_tok(string strTarget){
	//precondition: ���� ���ڿ�
	//Postcondition: ���ڿ��� ���� �� ������ �ּ� �������� ���� ���ͷ� ��ȯ
	int     nCutPos;
	int     nIndex = 0;
	string strTok = "|";
	vector<string> answer;
	for (int i = 0; i < 8; i++) {
		//������ �κп� '|'�� �߰��� �־��ش�
		strTarget = replaceAll(
			strTarget, string(1, SYMBOLS[i]), strTok+ string(1, SYMBOLS[i]) + strTok);
	}
	stringstream ss(strTarget);
    string item;
	//'|'�� �������� ����
    while (getline(ss, item, strTok.c_str()[0])) {
    	if(item!=""){
        	answer.push_back(item);
        }
    }
    return answer;
}
