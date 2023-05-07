#include "Calculator.h"

int main() {
    bool flag = true;
    Calculator calculator = Calculator();
    while (flag){
        flag = calculator.calculate();
    }
    return 0;
}
