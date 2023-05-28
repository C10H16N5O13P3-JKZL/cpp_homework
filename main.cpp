#include "Calculator.h"
#include "Compiler.h"

int main() {
    string file;
    getline(cin,file);
    Compiler compiler = Compiler();
    cout<<compiler.doCompile(file)<<endl;
    cout<<compiler.infix()<<endl;
    return 0;
}
// 使用的语言只包括两部分
//无限个定义变量的语句：要求严格空格分隔，无分号结尾，只能写在一行中：形如 var foobar = 123.123
//一个输出语句，要求括号严格封闭，严格大小写，不可使用字面量，型如:mul(add(foo,bar),pow(foo,mod(bar,foo)))
