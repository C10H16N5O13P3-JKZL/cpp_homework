//
// Created by shenby on 2023/5/5.
//
#include <string>
#include "Calculator.h"

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
    cout<<temp;
    pushin(temp);
}
/*!
 *  本函数用于
 *  计算算式的值
 *
 *  默认取用私有变量中保存的字符串
 * @return 计算结果（double）
 * @param None
 * @author 水冰月
 */
double Calculator::eval() {
    return 0;
}

ostream& Calculator::output(ostream& os)
{
    return os;
}

void Calculator::clean_buff()
{
}

string Calculator::preprocess()
{
    string temp = raw_input;
    string::size_type index = 0;
    while ((index=temp.find('-',index)) != string::npos) {
        if (index-1 <0 || temp[index-1] =='('){
            temp.insert(index,"0");
        }
        index++;
    }
    index = 0;
    while ((index=temp.find('+',index)) != string::npos) {
        if (index-1 <0 || temp[index-1] =='('){
            temp.insert(index,"0");
        }
        index++;
    }
    return temp;
}



void Calculator::pushin(string &temp)
{
}

void Calculator::reception() {
    cout<<"-------------------------------------------------------"<<endl;
    cout<<"| This calculator supports following operations:      |"<<endl;
    cout<<"| +,-,*,/,^,%,negative(-),positive(+)                 |"<<endl;
    cout<<"| Type equation to calculate                          |"<<endl;
    cout<<"-------------------------------------------------------"<<endl;
}
