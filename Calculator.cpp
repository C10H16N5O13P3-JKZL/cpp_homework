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
 *  重载此函数允许从迭代器中读取
 */
void Calculator::receive(istream &is) {
    getline(is,raw_input);
    format();
    pushin();
}
/*!
 *
 * @return
 */
double Calculator::eval() {
    return 0;
}
