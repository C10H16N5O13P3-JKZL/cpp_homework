//
// Created by shenby on 2023/5/5.
//
#include <string>
#include "Calculator.h"
#include <cmath>

using namespace std;

/*!
 * 本函数用于
 * 1. 接收输入的字符串
 * 2. 规范化字符串
 * 3. 将规范化后的字符串压入符号栈
 *
 * 默认输入为cin，允许从文件中输入
 * 另：本函数的实际实现交由子函数实现
 *
 * @return 无返回值(void)
 * @param istream
 * @author 水冰月
 *
 * @TODO 重载此函数允许从迭代器中读取
 */
void Calculator::receive(istream &is) {
    getline(is,raw_input);
    auto temp = preprocess();
    mainProcess(temp);
}

/*!
 *
 * @param os
 * @return
 */
ostream& Calculator::output(ostream& os)
{
    return os<<raw_input<<" = "<<result<<endl;
}

string Calculator::preprocess()
{
    string temp = raw_input;
    string::size_type index = 0;
    while ((index=temp.find('-',index)) != string::npos) {
        if (index == 0 || temp[index-1] =='('|| temp[index-1] == '[' || temp[index-1] == '{'){
            temp.insert(index,"0");
        }
        index++;
    }
    index = 0;
    while ((index=temp.find('+',index)) != string::npos) {
        if (index == 0 || temp[index-1] =='('|| temp[index-1] == '[' || temp[index-1] == '{'){
            temp.insert(index,"0");
        }
        index++;
    }
    return temp;
}

void Calculator::mainProcess(string &temp)
{
    int AbsNum = ABS_NUM::ABS_NUM_EVEN;
    string num;
    for (string::size_type i = 0; i < temp.size(); ++i) {
        num = "";
        switch (temp[i]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '^':
            case '%':
                if (symbol_stack.empty() || isLeftBracket() || (symbol_stack.top() == '|' && AbsNum == ABS_NUM_ODD)){
                    symbol_stack.push(temp[i]);
                } else if (getPrior(temp[i])> getPrior(symbol_stack.top())){
                        symbol_stack.push(temp[i]);
                        break;
                    } else {
                        doCalculate();
                        symbol_stack.push(temp[i]);
                        break;
                    }

                break;
            case '!':
                if (!figure_stack.empty() && isInt(figure_stack.top())){
                    int x = (int) figure_stack.top();
                    if (x>0) {
                        figure_stack.pop();
                        int res = 1;
                        while (x != 1) {
                            res *= (x--);
                        }
                        figure_stack.push(res);
                    }else{
                        cerr<<"invalid input: check number before '!'";
                        throw invalid_argument("error factorial");
                    }
                }
                break;
            case '(':
            case '[':
            case '{':
                symbol_stack.push(temp[i]);
            case ')':
                while (symbol_stack.top()!='('){
                    doCalculate();
                }
                symbol_stack.pop();
                break;
            case ']':
                while (symbol_stack.top()!='['){
                    doCalculate();
                }
                symbol_stack.pop();
                break;
            case '}':
                while (symbol_stack.top()!='{'){
                    doCalculate();
                }
                symbol_stack.pop();
                break;
            case '|':
                if (AbsNum == ABS_NUM_EVEN){
                    symbol_stack.push(temp[i]);
                    AbsNum = ABS_NUM_ODD;
                    break;
                } else{
                    while(symbol_stack.top()!='|'){
                        doCalculate();
                    }
                    symbol_stack.pop();
                    double tmp = figure_stack.top();
                    figure_stack.pop();
                    figure_stack.push(abs(tmp));
                }
                break;
            default:
                while (temp[i]>='0'&&temp[i]<='9'||temp[i]=='.'){
                    num+=temp[i++];
                }
                i--;
                figure_stack.push(stod(num));
                break;
        }
    }
    while (!symbol_stack.empty()){
        doCalculate();
    }
    result = figure_stack.top();
}

void Calculator::doCalculate() {
    double x1,x2,res;
    x2 = figure_stack.top();
    figure_stack.pop();
    x1 = figure_stack.top();
    figure_stack.pop();

    res = eval(x1, x2, symbol_stack.top());
    symbol_stack.pop();
    figure_stack.push(res);
}

void Calculator::reception() {
    cout<<"-------------------------------------------------------"<<endl;
    cout<<"| This calculator supports following operations:      |"<<endl;
    cout<<"| +,-,*,/,^,%,negative(-),positive(+),abs             |"<<endl;
    cout<<"| Type equation to calculate                          |"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    cout<<": ";
}

bool Calculator::isInt(double x) {
    auto y = (long int)x;
    return ((y-x)<0.000001 && (x-y)<0.000001);
}

bool Calculator::isLeftBracket() {
    return symbol_stack.top() == '(' || symbol_stack.top() == '[' || symbol_stack.top() == '{';
}

int Calculator::getPrior(char c) {
    switch (c) {
        case '+':
        case '-':
            return PRIORITY_LEVEL_0;
        case '*':
        case '/':
            return PRIORITY_LEVEL_1;
        case '%':
        case '^':
            return PRIORITY_LEVEL_2;
        case '!':
            return PRIORITY_LEVEL_3;
        default:
            return PRIORITY_LEVEL_ERR;
    }
}

/*!
 * 用于双目运算符的计算
 *
 * @param x1
 * @param x2
 * @param c
 * @return
 */
double Calculator::eval(double x1, double x2, char c) {
    switch (c) {
        case '+':
            return x1+x2;
        case '-':
            return x1-x2;
        case '*':
            return x1*x2;
        case '/':
            return x1/x2;
        case '%':
            return fmod(x1,x2);
        case '^':
            return pow(x1,x2);
        default:
            cerr<<"invalid input: unknown operator";
            throw invalid_argument("unknown operator");
    }
}
