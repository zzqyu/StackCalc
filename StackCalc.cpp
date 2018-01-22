#include <cstdlib>    // Provides EXIT_SUCCESS
#include <cstring>    // Provides strchr
#include <string>
#include <stack>	//stack
#include <iostream>
#include <vector>
#include <sstream> 
#include <cmath>

using namespace std;

//사용할 기호 배열 
const char SYMBOLS[] = { '^', '*', '/', '%', '+', '-', '(', ')' };
//윗 배열의 사이즈
const int SYMBOL_SIZE = 8;

//괄호 검사 함수
bool is_balanced_parentheses(string s);
//infix식을 postfix식으로 바꾸는 함수
vector<string> infix_to_postfix(vector<string> v);
//postfix식을 계산하는 함수
bool postfix_calc(vector<string> v, int& result);
//연산을 수행하는 함수
bool calc(int n1, int n2, char op, int& result);
//각 기호의 우선순위를 배정하는 함수
int order(string oper);
//계산 최소 단위로 나누어 주는 함수
vector<string> string_to_tok(string strTarget);
//문자열의 일부 문자를 대체해 수정하는 함수
string replaceAll(string str, const string &pattern, const string &replace);
//계산식을 읽고 그 식의 값을 구해 출력하는 함수
bool read_and_evaluate();

int main() {
	while(read_and_evaluate());
	return 0;
}
bool read_and_evaluate(){
	//precondition: --
	//Postcondition: 계산식을 입력받고, 결과 출력
	//0을 입력하면 false, 계산을 수행하면 true
	string input_str;//입력한 계산식을 담는 변수
	const string STR_ERROR = "Error!";//에러 출력문구
	int* answer = new int ;//계산식 결과
	cout<<"input >> ";
	cin >> input_str;//계산식 입력 받음
	vector<string> v;//계산 최소 단위(operand, operator, parentheses)담는 벡터
	if(input_str=="0") return false;//0을 입력하면 false
	if(!is_balanced_parentheses(input_str)){//괄호검사
		cout<<STR_ERROR<<endl;//에러 출력
	}
	else{
		v = string_to_tok(input_str);//계산 최소 단위로 변환
		if(postfix_calc(infix_to_postfix(v), *answer))//계산식 계산
			cout<<"= "<< *answer <<endl;//계산결과 출력
		else
			cout<<STR_ERROR<<endl;//에러 출력
	}
	return true;
}

bool is_balanced_parentheses(string s) {
	//precondition: 계산식
	//Postcondition: 계산식 괄호검사, 수행결과 스택이 비면 true
	stack<char> ps;//괄호를 담는 스택
	for (int i = 0; i < s.length(); i++) {
		if (s[i] == SYMBOLS[6])//여는 괄호일 경우
			ps.push(SYMBOLS[6]);//스택에 넣어준다.
		else if (s[i] == SYMBOLS[7]&&!ps.empty())//닫는 괄호일 경우
			ps.pop();//스택에서 뺀다
	}
	return ps.empty();
}

bool calc(int n1, int n2, char op, int& result) {
	//precondition: operand 2개, operator, 계산결과 담을 공간
	//n2가 0이고, 나누기나 나머지연산을 수행하는 지 확인
	//Postcondition: 계산 수행 결과를 result에 담고, true를 리턴
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
	//precondition: 계산식의 최소 단위를 담은 벡터, 계산 값을 담을 공간
	//Postcondition: 계산 수행 결과를 result에 담고, true를 리턴
	stack<int> st;
	int n2;
	for (int i = 0; i < v.size(); i++) {
		//operator인 경우
		if (order(v[i])>=0&&order(v[i])<=2&&!st.empty()) {
			n2 = st.top();
			st.pop();
			int* temp = new int;
			//스택 최상단의 2개의 operand를 operator에 맞는 연산 수행
			bool is_not_error = calc(st.top(), n2, v[i].c_str()[0], *temp);
			//수행 에러가 발생하면 false 리턴
			if(!is_not_error) return is_not_error;
			//연산이 끝나면 스택 최상단의 2개의 operand를 pop
			st.pop();
			//위 계산결과 값을 스택에 추가
			st.push(*temp);
		}
		//operator가 아닌 경우에는 push해 준다.
		else {
			st.push(atoi(v[i].c_str()));
		}
	}
	//결과적으로 stack에는 계산 결과 값만 남는다
	//그 결과를 result에 저장
	result = st.top();
	//계산이 정상적으로 수행 됐음을 알리는 true를 리턴
	return true;
}
vector<string> infix_to_postfix(vector<string> v) {
	//precondition: 계산식의 최소 단위를 담은 벡터
	//Postcondition: 계산식의 순서를 infix에서 postfix으로 바꾼다.
	stack<string> s;
	vector<string> answer;
	for (int i = 0; i < v.size(); i++) {
		if (order(v[i]) == 6) {//여는 괄호일 경우
			s.push(v[i]);//스택에 추가
		}
		else if (order(v[i]) == 7) {//닫는괄호일 경우
			//여는 괄호가 나올때까지 벡터에 추가하고
			//스택에서는 pop해준다.
			while(!s.empty() && order(s.top())!=6){
				answer.push_back(s.top());
				s.pop();
			}
			s.pop();
		}
		else if(order(v[i]) == -1) {//operand일 경우
			answer.push_back(v[i]);//벡터에 추가한다.
		}
		else if (order(v[i])>=0&&order(v[i])<=2) {//operator일 경우
			if (!s.empty()&& order(s.top())!=6 && 
				((order(s.top()) < order(v[i]))||
				order(s.top()) == order(v[i]))) {
				//연산우선순위가 위와 같을 때
				answer.push_back(s.top());//벡터에 추가하고
				s.pop();//스택에서는 pop해준다
			}
			s.push(v[i]);//스택에 추가
		}
		
	}
	while (!s.empty()) {//for문 다 돌고 스택이 비어있지 않은 경우
		answer.push_back(s.top());//벡터에 추가하고 스택에서는 pop해준다.
		s.pop();
	}
	return answer;
}

int order(string oper) {
	//precondition: 계산식의 최소 단위
	//Postcondition: 계산식의 최소 단위에 번호를 부여한다.
	int o = 3;
	int i = 0;
	string symbols(SYMBOLS);
	i=symbols.find(oper.c_str()[0]);
	if (i == 0) o = 0;//'^'일경우 0순위
	else if (i>0 && i <= 3) o = 1;//*/%일 경우 1순위
	else if (i == 4 || i == 5) o = 2;//+-일 경우 2순위
	else if (i >= 6)o=i;//괄호는 걍 Symbols의 인덱스 부여
	else o=-1; //operand는 -1
	return o;
}
string replaceAll(string str, const string &pattern, const string &replace){
	//precondition: 수정할 문자열, 수정 대상 문자열, 대체 문자열
	//Postcondition: 문자열에서 수정 대상 문자열을 찾아서 대체 문자열로 바꾸어 준다.
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
	//precondition: 계산식 문자열
	//Postcondition: 문자열을 분할 해 계산식의 최소 단위들을 담은 벡터로 변환
	int     nCutPos;
	int     nIndex = 0;
	string strTok = "|";
	vector<string> answer;
	for (int i = 0; i < 8; i++) {
		//분할할 부분에 '|'을 추가해 넣어준다
		strTarget = replaceAll(
			strTarget, string(1, SYMBOLS[i]), strTok+ string(1, SYMBOLS[i]) + strTok);
	}
	stringstream ss(strTarget);
    string item;
	//'|'를 기준으로 분할
    while (getline(ss, item, strTok.c_str()[0])) {
    	if(item!=""){
        	answer.push_back(item);
        }
    }
    return answer;
}
