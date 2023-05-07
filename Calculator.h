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
    bool calculate();
private:
    static void reception();
    ostream &output(ostream &os = cout);
    bool wavehand();
    void receive(istream &is = cin);

    enum PRIORITY{
        PRIORITY_LEVEL_0 = 0, //+-
        PRIORITY_LEVEL_1 = 1, //*/
        PRIORITY_LEVEL_2 = 2, //%^
        PRIORITY_LEVEL_3 = 3, //!
        PRIORITY_LEVEL_ERR = -1
    }; //用于定义符号优先级
    enum ABS_NUM{
        ABS_NUM_ODD = 1,
        ABS_NUM_EVEN = 2
    }; //用于计算绝对值符号个数

    stack<char> symbol_stack;
    stack<double> figure_stack;
    static double eval(double ,double ,char );
    string preprocess();
    void mainProcess(string &);
    static bool isInt(double );
    bool isLeftBracket();
    static int getPrior(char );

    double result{};
    vector<string> postfix;
    string raw_input;
    void doCalculate();
};


#endif //HOMEWORK_CALCULATOR_H
