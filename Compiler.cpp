//
// Created by shenby on 2023/5/10.
//

#include "Compiler.h"

void AbstractTree::ASTNode::combine() {


}

void AbstractTree::buildAST(ifstream infile) {
    string line;
    string name;
    bool nextABS = false;
    while(getline(infile,line)){
        int type;
        vector<string> tokens;
        string token;
        istringstream iss(line);
        while(iss>>token){
            tokens.push_back(token);
        }
        auto opt = tokens[0];
        if (opt == "val")
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
            case CMD: // mul(add(a,b),c)
                construct(line);
            default:
                throw runtime_error("invalid input!");
        }
    }

}

void AbstractTree::construct(const string &cmd) {
    if (variables.find(cmd)!=variables.end()){
        root = construct(root,VAL,cmd,"",nowABS);
        nowABS = false;
    }
    auto pure = purify(cmd);
    auto tokens = tokenize(pure);
    auto opt = tokens[0];
    int type;
    type = getType(opt);
    if (type == ABS){
        auto Npure = purify(tokens[1]);
        auto Ntokens = tokenize(Npure);
        auto Nopt = Ntokens[0];
        nowABS = true;
        construct(Ntokens[1]);
    } else
        root = construct(root,type,tokens[1],tokens[2]);
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
        type = ERR;
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
    pos3 = str.size()-1;
    pos2 = string::npos;
    for (string::size_type i = 0;i<str.size();i++) {
        char c = str[i];
        if (c == '(')
            bracket++;
        else if (c == ')'){
            bracket--;
        }
        if (c == ',' && bracket==1)
            pos2 = i;
    }
    string pure(str);
    pure.replace(pos1,1," ");
    if (pos2!=string::npos) pure.replace(pos2,1," ");
    pure.replace(pos3,1," ");
    return pure;
}

auto AbstractTree::construct(shared_ptr<ASTNode> roo, int t,
        const string& leftLeafRaw, const string& rightLeafRaw,bool isAbs)-> shared_ptr<ASTNode> {
    if (leftLeafRaw.empty()||rightLeafRaw.empty()){
        return nullptr;
    }
    if (t == VAL){
        double res = variables[leftLeafRaw];
        shared_ptr<ASTNode> now(new ASTNode(res,nowABS));
        return now;
    }else if(t==ABS){
        nowABS = true;

    }
    auto LLP = purify(leftLeafRaw);
    auto RLP = purify(rightLeafRaw);
    auto tokensL = tokenize(LLP);
    auto tokensR = tokenize(RLP);
    auto typeL = getType(tokensL[0]);
    auto typeR = getType(tokensR[0]);

    auto now = make_shared<ASTNode>(t,)
    roo->leftLeaf = construct(roo->leftLeaf,typeL,tokensL[1],tokensL[2]);
    roo->rightLeaf = construct(roo->rightLeaf,typeR,tokensR[1],tokensR[2]);
    return now;
}