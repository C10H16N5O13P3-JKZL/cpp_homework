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
#include <sstream>
#include <memory>
#include <iostream>

using namespace std;

class AbstractTree{
friend class ASTNode;
private:
    class ASTNode{
    public:
        bool isAbs;
        int type;
        double value{};
        string combination;
        shared_ptr<ASTNode> leftLeaf;
        shared_ptr<ASTNode> rightLeaf;

        ASTNode(double val,bool isABS):value(val),type(VAL),isAbs(isABS),leftLeaf(nullptr),
            rightLeaf(nullptr),combination(to_string(value)){}
        ASTNode(int t,bool abs):type(t),isAbs(abs){}
    };

    shared_ptr<ASTNode> root = nullptr;
    enum operators{
        ADD = 0,
        SUB = 1,
        MUL = 2,
        DIV = 3,
        ABS = 4,
        POW = 5,
        MOD = 6,
        VAL = 7,
        CMD = 8,
        END = 9,
        ERR = 10
    };
    map<string,double> variables;
    double result;
    bool endForResult = false;
    bool nowABS = false;

    shared_ptr<ASTNode> construct(const shared_ptr<ASTNode>& roo, int t, const string& leftLeafRaw, const string& rightLeafRaw,bool isABS = false);
    void construct(const string &);
    static vector<string> tokenize(const string &);
    static string purify(const string &);
    string combine(const shared_ptr<ASTNode> &);
    string combine();
public:
    AbstractTree()=default;
    void buildAST(ifstream&);
    string infixExpress();
    void showVar(const string&);

    static int getType(const basic_string<char> &opt) ;

    bool checkVar(const string &leftLeafRaw);
};

class Compiler {
private:
    AbstractTree AST;
    Calculator CAL;
public:
    double doCompile(const string& fileName);
    string infix();
    Compiler()=default;
};


#endif //HOMEWORK_COMPILER_H
