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
//
