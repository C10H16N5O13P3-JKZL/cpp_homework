//
// Created by shenby on 2023/5/5.
//

#ifndef HOMEWORK_CALCULATOR_H
#define HOMEWORK_CALCULATOR_H

#include <vector>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

class Calculator {
public:
    Calculator() = default;
    bool calculate(ostream &os = cout);
    void receive(istream &is = cin);
    ostream &output(ostream &os = cout);
    void receive(string &);
private:
    enum PRIORITY{
        PRIORITY_LEVEL_0 = 0, //+ -
        PRIORITY_LEVEL_1 = 1, //* /
        PRIORITY_LEVEL_2 = 2, //% ^
        PRIORITY_LEVEL_3 = 3, //!
        PRIORITY_LEVEL_ERR = -1
    }; //用于定义符号优先级
    enum ABS_NUM{
        ABS_NUM_ODD = 1,
        ABS_NUM_EVEN = 2
    }; //用于记录绝对值符号奇偶性

    stack<char> symbol_stack; //符号栈，用于保存符号
    stack<double> figure_stack; //数字栈，用于保存数字
    double result{}; //结果数字
    string raw_input; //原始输入

    static bool waveHand();
    static double eval(double ,double ,char );
    static void reception();
    string preprocess();
    void mainProcess(string &);
    static bool isInt(double );
    bool isLeftBracket();
    static int getPrior(char );
    void doCalculate();
};


#endif //HOMEWORK_CALCULATOR_H
