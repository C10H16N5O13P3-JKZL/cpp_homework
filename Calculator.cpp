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
 */
void Calculator::receive(istream &is) {
    getline(is,raw_input);
    auto temp = preprocess();
    mainProcess(temp);
}


/*!
 * 输出函数，支持接受一个输出地
 *
 * @param os
 * @return
 */
ostream& Calculator::output(ostream& os)
{
    return os<<raw_input<<" = "<<result<<endl;
}
/*!
 * 预处理函数，用于将单独的负数和正数转化为0-+x的形式，将单目运算符转化为双目版本
 *
 * @return
 */
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
/*!
 * 主计算步骤
 * 主要内容是逐个检查由preprocess生成的temp字符串，并进行计算
 * 计算方式为边找边运算，保证符号栈不存在一对同类括号
 *
 * @param temp
 */
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
                break;
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
    figure_stack.pop();
}
/*！
 * 辅助函数，用于包装弹出两数并计算的过程
 */
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
/*！
 * 接待函数，提供一段提示页面
 */
void Calculator::reception() {
    cout<<"-------------------------------------------------------"<<endl;
    cout<<"| This calculator supports following operations:      |"<<endl;
    cout<<"| +,-,*,/,^,%,negative(-),positive(+),abs             |"<<endl;
    cout<<"| Type equation to calculate                          |"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
}
/// 辅助函数，包装了检查是否为整数的过程，考虑了浮点误差
/// \param x
/// \return bool
bool Calculator::isInt(double x) {
    auto y = (long int)x;
    return ((y-x)<0.000001 && (x-y)<0.000001);
}
/// 辅助函数，包装了检查括号的过程
/// \return bool
bool Calculator::isLeftBracket() {
    return symbol_stack.top() == '(' || symbol_stack.top() == '[' || symbol_stack.top() == '{';
}
/*！
 * 用于获取符号优先级
 * 遇到未定义符号直接抛出错误，不过实际上不会发生，异常符号会被忽略
 */
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
/*!
 * 挥手函数，用于结束计算
 *
 * @return bool 返回一个代表是否再次运算的布尔值
 */
bool Calculator::waveHand() {
    cout<<"If you do not want to calculate another equation ,enter 'n' to exit."<<endl;
    cout<<"Or enter 'c' to continue"<<endl;
    char c;
    cin>>c;
    return c != 'n';
}
/*！
 * 包装函数，用于将计算器的所有步骤隐藏起来
 */
bool Calculator::calculate(ostream &os) {
    reception();
    receive();
    output(os);
    return waveHand();
}

void Calculator::receive(string &s) {
    raw_input = s;
    auto temp = preprocess();
    mainProcess(temp);
}
