//
// Created by shenby on 2023/5/10.
//

#ifndef HOMEWORK_COMPILER_H
#define HOMEWORK_COMPILER_H
#include <string>
#include <map>
#include <iostream>
#include "Calculator.h"
#include <fstream>

using namespace std;

class AbstractTree{
friend class ASTNode;
private:
    enum operators{
        ADD = 0,
        SUB = 1,
        MUL = 2,
        DIV = 3,
        ABS = 4,
        POW = 5,
        VAL = 6,
    };
    map<string,double> variables;
    class ASTNode{
    private:
        bool isPure;
        int type;
        string combination;
        ASTNode* leftLeaf;
        ASTNode* rightLeaf;
    public:
        ASTNode(int t,const string& name = ""):type(t),leftLeaf(nullptr),rightLeaf(nullptr),isPure(true),combination(name){};
        void combine();
    };
public:
    AbstractTree();
    void readFile(ifstream&);
    string infixExpress();
};
class Compiler {
private:
    AbstractTree AST;
    Calculator CAL;
public:
    double doCompile(const string& fileName);
    Compiler();
};


#endif //HOMEWORK_COMPILER_H
