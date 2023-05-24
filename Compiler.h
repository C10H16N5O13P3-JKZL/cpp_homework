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

using namespace std;

class AbstractTree{
friend class ASTNode;
private:
    class ASTNode{
    public:
        bool isAbs;
        int type;
        double value;
        string combination;
        shared_ptr<ASTNode> leftLeaf;
        shared_ptr<ASTNode> rightLeaf;

        ASTNode(double val,bool isABS):value(val),type(VAL),isAbs(isABS),leftLeaf(nullptr),
            rightLeaf(nullptr),combination(to_string(value)){}
        explicit ASTNode(int t, const string& name = ""): type(t), leftLeaf(nullptr),
            rightLeaf(nullptr), isAbs(false), combination(name){ if (t==ABS) isAbs = true;};
        void combine();
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

    shared_ptr<ASTNode> construct(shared_ptr<ASTNode> roo, int t, const string& leftLeafRaw, const string& rightLeafRaw, bool isABS = false);
    void construct(const string &);
    static vector<string> tokenize(const string &);
    static string purify(const string &);
public:
    AbstractTree()=default;
    void buildAST(ifstream);
    string infixExpress();
    void show(const string&);

    static int getType(const basic_string<char> &opt) ;
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
