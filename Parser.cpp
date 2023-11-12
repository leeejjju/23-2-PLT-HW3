#include <iostream>
#include <regex>
#include <string>
#include "FAE.h"
#define DEBUG


class Parser {
public:
    // Parser();
    vector<string> splitExpressionAsSubExpressions(string exampleCode);
    vector<string> getSubExpressions(string exampleCode);
    FAE* parse(string exampleCode);
    bool isNumeric(string str);
};

vector<string> Parser::splitExpressionAsSubExpressions(string exampleCode){

    if ((exampleCode.front() == '{' && exampleCode.back() != '}') || (exampleCode.front() != '{' && exampleCode.back() == '}')){
        #ifdef DEBUG 
        cout << "[splitExpressionAsSubExpressions] not worked : " << exampleCode << endl;
        #endif
        // exit(0);
        
    }
    if (exampleCode.front() == '{'){ //in the case { ~~~~ } ??
        exampleCode = exampleCode.substr(1, exampleCode.length() - 1);
        #ifdef DEBUG 
        cout << "[splitExpressionAsSubExpressions] worked : " << exampleCode << endl;
        #endif
    }
    //remove first '{' and return getSubExpressions()...? 
    return getSubExpressions(exampleCode);
}

//split expression as subexpressions... the standard is {}. 
vector<string> Parser::getSubExpressions(string exampleCode) {

    vector<string> sexpressions = vector<string>();
    int openingParenthesisCount = 0;
    string strBuffer = "";

    for (int i = 0; i < exampleCode.length(); i++) {

        if (i == 0 || (i == 0 && exampleCode.at(i) == '{')) { 
            strBuffer = strBuffer + exampleCode.at(i); //just push char in buffer
            if(exampleCode.at(i) == '{') openingParenthesisCount++;
            continue;

        } else if (exampleCode.at(i) == ' ' && openingParenthesisCount == 0){
            // new scope start without '{'
            if (strBuffer.size() > 0) { //if something is in buffer
                sexpressions.push_back(strBuffer); //save it and
                strBuffer = ""; // Ready to start a new buffer(init it)
            }
            continue;

        }else {
            if (exampleCode.at(i) == '{' && openingParenthesisCount == 0){
                // new scope start with '{'
                openingParenthesisCount++;
                strBuffer = "" + exampleCode.at(i);
                continue;

            } else if (exampleCode.at(i) == '{'){
                // add scope start with '{'
                openingParenthesisCount++;
                strBuffer = strBuffer + exampleCode.at(i);
                continue;

            } else if (exampleCode.at(i) == '}' && openingParenthesisCount > 0){
                // closed one inner scope; just add to buffer
                openingParenthesisCount--;
                strBuffer = strBuffer + exampleCode.at(i);
                continue;

            } else if (exampleCode.at(i) == '}'){
                // closed scope; buffer is ready to be a subexpression
                sexpressions.push_back(strBuffer);
                strBuffer = "";
                continue;
            }
            
        }
        //for another, simply add to buffer
        strBuffer = strBuffer + exampleCode.at(i);

    }
    //save the last buffer as sexpression
    if(strBuffer.size()) sexpressions.push_back(strBuffer);
    #ifdef DEBUG
    cout << "[getSubExpression] just made " << sexpressions.size() << " sub expression. \n";
    for(int i = 0; i < sexpressions.size(); i++){
        cout << i << "st expr: " << sexpressions.at(i) << endl;
    }
    #endif
    //return the list of sexpressioin
    return sexpressions;
}

/*(define (parse sexp)
   (match sexp
        [(? number?)                (num sexp)]
        [(list '+ l r)              (add (parse l) (parse r))]
        [(list '- l r)              (sub (parse l) (parse r))]
        [(list 'with (list i v) e)  (app (fun i (parse e)) (parse v))]
        [(? symbol?)                (id sexp)]
        [(list 'fun (list p) b)                 (fun p (parse b))]
        [(list f a)                 (app (parse f) (parse a))]
        [else                       (error 'parse "bad syntax: ~a" sexp)]))*/
FAE* Parser::parse(string exampleCode) {
    #ifdef DEBUG
    cout << "[parse] trying to parse " << exampleCode << endl;
    #endif
    vector<string> subExpressions = splitExpressionAsSubExpressions(exampleCode);

    if (subExpressions.size() == 1 && isNumeric(subExpressions.front())){
        // FAE* num = new Num();
        FAE *num = new FAE();
        num->type = NUM;
        num->createNum(subExpressions.front());
        return num;

    }else if (subExpressions.front() == "+"){
        // FAE* num = new Num();
        FAE *add = new FAE();
        add->type = ADD;
        add->createAdd(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return add;

    } else if (subExpressions.front() == "-"){
        // FAE* num = new Num();
        FAE *sub = new FAE();
        sub->type = SUB;
        sub->createSub(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return sub;

    } else if(subExpressions.front() == "with"){
        //[(list 'with (list i v) e)  (app (fun i (parse e)) (parse v))]  
        // {with {x 3} {+ x x}} -> (app (fun x (add (id x) (id x))) (num 3))
        FAE* app = new FAE();
        app->type = APP;

        FAE* fun = new FAE();
        fun->type = FUN;
        fun->createFun(parse(subExpressions.at(1))->fun_expr, parse(subExpressions.at(2)));
        app->createApp(fun, parse(subExpressions.at(1))->arg_expr);
        return app;

    }else if(subExpressions.front() == "fun"){
        //[(list 'fun (list p) b)                 (fun p (parse b))]
        FAE* fun = new FAE();
        fun->type = FUN;
        if(subExpressions.at(1).back() == '}') subExpressions.at(1)[subExpressions.at(1).size()-1] = 0;
        fun->createFun(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return fun;

    }else if (subExpressions.size() < 2 && !isNumeric(subExpressions.front())){
        FAE* id = new FAE();
        id->type = ID;
        id->createID(subExpressions.front());
        return id;

    }else if(subExpressions.size() >= 2 && !isNumeric(subExpressions.front())){
        //[(list f a)                 (app (parse f) (parse a))]
        FAE* app = new FAE();
        app->type = APP;
        app->createApp(parse(subExpressions.front()), parse(subExpressions.at(1)));
        return app;

    }else{
        cout << "[parse] bad syntax: " << exampleCode << endl;
        exit(1);
    }

}

bool Parser::isNumeric(string str){
    regex pattern("-?\\d+(\\.\\d+)?");
    smatch match;
    return regex_match(str, match, pattern);
}

