//
// Created by shenby on 2023/5/10.
//

#include "Compiler.h"

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

bool AbstractTree::checkVar(const string &leftLeafRaw) { return variables.find(leftLeafRaw) != variables.end(); }

void AbstractTree::showVar(const string &var) {
    cout<<variables.at(var)<<endl;
}

string AbstractTree::combine() {
    return combine(root);
}

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
        case VAL:return roo->isAbs?"|("+ to_string(roo->value)+")|":"("+ to_string(roo->value)+")";
        default:
            throw runtime_error("Unexcepted type");
    }
    if (roo->isAbs)
        return "|(" + combine(roo->leftLeaf) +")"+ opt +"("+combine(roo->rightLeaf) + ")|";
    else
        return "((" + combine(roo->leftLeaf) + ")" + opt + "(" + combine(roo->rightLeaf) + "))";
}

string AbstractTree::infixExpress() {
    return combine();
}

double Compiler::doCompile(const string &fileName) {
    ifstream in(fileName);
    if (!in)
        throw runtime_error("cant open file");
    AST.buildAST(in);
    return CAL.receive(AST.infixExpress());
}

string Compiler::infix() {
    return AST.infixExpress();
}
