//
// Created by shenby on 2023/5/5.
//

#ifndef HOMEWORK_CALCULATOR_H
#define HOMEWORK_CALCULATOR_H

#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Calculator {
public:
    Calculator() = default;
    static void reception();
    void receive(istream &is = cin);
    double eval();
    ostream &output(ostream &os = cout);
    void clean_buff();
private:
enum PRIORITY{
    PRIORITY_LEVEL_0 = 0,
    PRIORITY_LEVEL_1 = 1,
    PRIORITY_LEVEL_2 = 2,
}; //用于定义符号优先级
enum ABS_NUM{
    ABS_NUM_ODD = 1,
    ABS_NUM_EVEN = 2
}; //用于计算绝对值符号个数

stack<char> symbol_stack;
string preprocess();
void pushin(string &);
static inline bool isLeftOperator(string &temp,string::size_type index) {
    return temp[index-1] == '(' || temp[index-1] == '+' || temp[index-1] == '-' || temp[index-1] == '*' || temp[index-1] == '/' || temp[index-1] == '^';
}

string raw_input;
string formatted_input;
};


#endif //HOMEWORK_CALCULATOR_H
