//
// Created by shenby on 2023/5/10.
//

#include "Compiler.h"

/// 总入口函数，前端调用这个就可以了
/// \param infile
void AbstractTree::buildAST(ifstream& infile) {
    string line;
    string name;
    while(getline(infile,line)){
        int type;
        vector<string> tokens;
        string token;
        istringstream iss(line);
        while(iss>>token){
            tokens.push_back(token);
        }
        auto opt = tokens[0];
        if (opt == "var")
            type = VAL;
        else if (opt == "RES")
            type = END;
        else
            type = CMD;

        switch (type) {
            case VAL: // var a = 123
                name = tokens[1];
                variables[name] = Calculator().receive(tokens[3]);
                break;
            case END: // RES
                endForResult = true;
                break;
            case CMD: // mul(add(a,b),c)
                construct(line);
                break;
            default:
                throw runtime_error("invalid input!");
        }
    }

}

//！入口树建构函数，用于第一次处理和处理第一个abs（可能仍有bug）
void AbstractTree::construct(const string &cmd) {
    if (variables.find(cmd)!=variables.end()){
        root = make_shared<ASTNode>(variables[cmd],nowABS);
        nowABS = false;
        return;
    }
    auto pure = purify(cmd);
    auto tokens = tokenize(pure);
    auto opt = tokens[0];
    int type;
    type = getType(opt);
    if (type == ABS){
        nowABS = true;
        auto Npure = purify(tokens[1]);
        auto Ntokens = tokenize(Npure);
        if (checkVar(Ntokens[0])) {
            nowABS = false;
            root = make_shared<ASTNode>(variables[Ntokens[0]], true);
        }
        else{
            construct(tokens[1]);
        }
    } else {
        nowABS = false;
        root = construct(root, type, tokens[1], tokens[2],true);
    }
}

//！获取操作符的类型
int AbstractTree::getType(const basic_string<char> &opt) {
    int type;
    if (opt == "add")
        type = ADD;
    else if (opt == "sub")
        type = SUB;
    else if (opt == "mul")
        type = MUL;
    else if (opt == "div")
        type = DIV;
    else if (opt == "abs")
        type = ABS;
    else if (opt == "pow")
        type = POW;
    else if (opt == "mod")
        type = MOD;
    else
        type = VAL;
    return type;
}

//!
//! 切分语句，非常简单的函数
//! \param pure
//! \return
vector<string> AbstractTree::tokenize(const string &pure) {
    vector<string> tokens;
    string token;
    istringstream iss(pure);
    while(iss>>token)
        tokens.push_back(token);
    return tokens;
}

string AbstractTree::purify(const string &str) {
    int bracket = 0;
    string::size_type pos1,pos2,pos3;
    pos1 = str.find('(');
    pos3 = (str[str.size()-1]==')')?str.size()-1:string ::npos;
    pos2 = string::npos;
    for (string::size_type i = 0;i<str.size();i++) {
        char c = str[i];
        if (c == '(')
            bracket++;
        else if (c == ')'){
            bracket--;
        }
        if (c == ',' && bracket==1) {
            pos2 = i;
            break;
        }
    }
    string pure(str);
    if (pos1!=string::npos) pure.replace(pos1,1," ");
    if (pos2!=string::npos) pure.replace(pos2,1," ");
    if (pos3!=string::npos) pure.replace(pos3,1," ");
    return pure;
}

/// 里·树建构函数，用于超过一层的迭代，思路很复杂，我也不确保没有bug
/// \param roo
/// \param t
/// \param leftLeafRaw
/// \param rightLeafRaw
/// \param isABS
/// \return
auto AbstractTree::construct(const shared_ptr<ASTNode>& roo, int t,
        const string& leftLeafRaw, const string& rightLeafRaw,bool isABS)-> shared_ptr<ASTNode> {
    if (leftLeafRaw.empty()||rightLeafRaw.empty()){
        return nullptr;
    }
    auto now1 = make_shared<ASTNode>(t, nowABS||isABS);
    bool doLeft = true,doRight = true; //控制是否左右迭代
    if (checkVar(leftLeafRaw)){
        now1->leftLeaf = make_shared<ASTNode>(variables[leftLeafRaw], false);
        doLeft = false;
    }//如果左孩子是变量
    if (checkVar(rightLeafRaw)){
        now1->rightLeaf = make_shared<ASTNode>(variables[rightLeafRaw], false);
        doRight = false;
    }//如果右孩子是变量

    if (doLeft) {
        auto LLP = purify(leftLeafRaw);
        auto tokensL = tokenize(LLP);
        auto typeL = getType(tokensL[0]);
        bool flagL = true;
        while (typeL == ABS){
            nowABS = true;
            LLP = purify(tokensL[1]);
            tokensL = tokenize(LLP);
            if (checkVar(tokensL[0])) {
                nowABS = false;
                now1->leftLeaf = make_shared<ASTNode>(variables[tokensL[0]],true);
                flagL = false;
                break;
            }
            else if (getType(tokensL[0]) == ABS){
                continue;
            }
            typeL = getType(tokensL[0]);
        }
        if (flagL) {
            now1->leftLeaf = construct(now1->leftLeaf, typeL, tokensL[1], tokensL[2]);
            nowABS = false;
        }
    }
    if (doRight) {
        bool flagR = true;
        auto RLP = purify(rightLeafRaw);
        auto tokensR = tokenize(RLP);
        auto typeR = getType(tokensR[0]);
        while (typeR == ABS){
            nowABS = true;
            RLP = purify(tokensR[1]);
            tokensR = tokenize(RLP);
            if (checkVar(tokensR[0])) {
                nowABS = false;
                now1->rightLeaf = make_shared<ASTNode>(variables[tokensR[0]],true);
                flagR = false;
                break;
            }
            else if (getType(tokensR[0]) == ABS){
                continue;
            }
            typeR = getType(tokensR[0]);
        }
        if (flagR) {
            now1->rightLeaf = construct(now1->rightLeaf, typeR, tokensR[1], tokensR[2]);
            nowABS = false;
        }
    }

    return now1;
}

/// 辅助函数，检查是否为变量名
/// \param leftLeafRaw
/// \return
bool AbstractTree::checkVar(const string &leftLeafRaw) { return variables.find(leftLeafRaw) != variables.end(); }

/// 未使用的辅助函数，用于展示变量的值
/// \param var
void AbstractTree::showVar(const string &var) {
    cout<<var<<" = "<<variables.at(var)<<endl;
}

/// 组合为中缀表示法的树启动函数，其实没啥用，定义多了
/// \return
string AbstractTree::combine() {
    return combine(root);
}

/// 实际干活的中缀构建函数，本质上是树的遍历，没什么好说的
/// \param roo
/// \return
string AbstractTree::combine(const shared_ptr<ASTNode> &roo) {
    auto type = roo->type;
    char opt;
    switch (type) {
        case ADD:opt = '+';
            break;
        case SUB:opt = '-';
            break;
        case MUL:opt = '*';
            break;
        case DIV:opt = '/';
            break;
        case POW:opt = '^';
            break;
        case MOD:opt = '%';
            break;
        case VAL:
            return roo->isAbs?"|("+ to_string(roo->value)+")|":"("+ to_string(roo->value)+")";
        default:
            throw runtime_error("Unexcepted type");
    }
    if (roo->isAbs)
        return "|(" + combine(roo->leftLeaf) +")"+ opt +"("+combine(roo->rightLeaf) + ")|";
    else
        return "((" + combine(roo->leftLeaf) + ")" + opt + "(" + combine(roo->rightLeaf) + "))";
}

/// 构建中缀表达式的树的内部接口
/// \return
string AbstractTree::infixExpress() {
    return combine();
}

/// 编译器主函数，用于编译+计算
/// \param fileName
/// \return
double Compiler::doCompile(const string &fileName) {
    ifstream in(fileName);
    if (!in)
        throw runtime_error("cant open file");
    AST.buildAST(in);
    return CAL.receive(AST.infixExpress());
}

/// 编译器调用树的中缀表达式的函数，前端可见，前端用这个展示编译结果就行
/// \return
string Compiler::infix() {
    return AST.infixExpress();
}
