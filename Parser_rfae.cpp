#include <iostream>
#include <regex>
#include <string>
#include "RFAE.h"
// #define DEBUG

/*
"{with {fib {fun {n} {if {or {= n 0} {= n 1}} 1 {+ {fib {- n 1}} {fib {- n 2}}}}}} {fib 10}}"

(app (fun mk-rec (app (fun fib (app (id fib) (num 10))) (app (id mk-rec) (fun fib (fun n (if (or (= (id n) (num 0)) (= (id n) (num 1))) (num 1) (add (app (id fib) (sub (id n) (num 1))) (app (id fib) (sub (id n) (num 2)))))))))) (fun body-proc (app (fun fX (app (id fX) (id fX))) (fun fY (app (fun f (app (id body-proc) (id f))) (fun x (app (app (id fY) (id fY)) (id x))))))))

"{with {fac {fun {n} {if {= n 0} 1 {* n {fac {- n 1}}}}}} {fac 10}}"

(app (fun mk-rec (app (fun fac (app (id fac) (num 10))) (app (id mk-rec) (fun fac (fun n (if (= (id n) (num 0)) (num 1) (mul (id n) (app (id fac) (sub (id n) (num 1)))))))))) (fun body-proc (app (fun fX (app (id fX) (id fX))) (fun fY (app (fun f (app (id body-proc) (id f))) (fun x (app (app (id fY) (id fY)) (id x))))))))

*/
class Parser {
public:
    vector<string> splitExpressionAsSubExpressions(string exampleCode);
    vector<string> getSubExpressions(string exampleCode);
    RFAE* parse(string exampleCode);
    bool isNumeric(string str);
    bool isRec(string str);
    string desugar(string str);
};

vector<string> Parser::splitExpressionAsSubExpressions(string exampleCode){

    if ((exampleCode.front() == '{' && exampleCode.back() != '}') || (exampleCode.front() != '{' && exampleCode.back() == '}')){
        #ifdef DEBUG 
        cout << "[splitExpressionAsSubExpressions] not worked : " << exampleCode << endl;
        #endif
        //exit(0);
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
            if(exampleCode.at(i) == '{') openingParenthesisCount++;
            strBuffer += exampleCode.at(i); //just push char in buffer
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
                strBuffer += exampleCode.at(i);
                continue;

            } else if (exampleCode.at(i) == '{'){
                // add scope start with '{'
                openingParenthesisCount++;
                strBuffer += exampleCode.at(i);
                continue;

            } else if (exampleCode.at(i) == '}' && openingParenthesisCount > 0){
                // closed one inner scope; just add to buffer
                openingParenthesisCount--;
                strBuffer += exampleCode.at(i);
                continue;

            } else if (exampleCode.at(i) == '}'){
                // closed scope; buffer is ready to be a subexpression
                sexpressions.push_back(strBuffer);
                strBuffer = "";
                continue;
            }
            
        }
        //for another, simply add to buffer
        strBuffer += exampleCode.at(i);

    }
    //save the last buffer as sexpression
    if(strBuffer.size()) sexpressions.push_back(strBuffer);
    
    //return the list of sexpressioin
    return sexpressions;
}


/*(define (parse sexp)
   (match sexp
        [(? number?)                (num sexp)]
        [(list '+ l r)              (add (parse l) (parse r))]
        [(list '- l r)              (sub (parse l) (parse r))]
        [(list '* l r)              (mult (parse l) (parse r))]
        [(list 'if c l r)           (if (parse c) (parse l) (parse r))]
        [(list 'or l r)             (or (parse l) (parse r))]
        [(list '= l r)              (equl (parse l) (parse r))]
        [(list 'with (list i v) e)  (app (fun i (parse e)) (parse v))]
        [(? symbol?)                (id sexp)]
        [(list 'fun (list p) b)     (fun p (parse b))]
        [(list f a)                 (app (parse f) (parse a))]
        [else                       (error 'parse "bad syntax: ~a" sexp)]))*/
RFAE* Parser::parse(string exampleCode) {
    #ifdef DEBUG
    cout << "[parse] trying to parse " << exampleCode << endl;
    #endif
    vector<string> subExpressions = splitExpressionAsSubExpressions(exampleCode);

    #ifdef DEBUG
    cout << "[parse] just made " << subExpressions.size() << " sub expression. \n";
    for(int i = 0; i < subExpressions.size(); i++){
        cout << i << "st expr: \'" << subExpressions.at(i) << "\'\n";
    }
    #endif

    if (subExpressions.size() == 1 && isNumeric(subExpressions.front())){
        // num
        RFAE *num = new RFAE();
        num->type = NUM;
        num->createNum(subExpressions.front());
        return num;

    }else if (subExpressions.front().compare("+") == 0){
        // add
        RFAE *add = new RFAE();
        add->type = ADD;
        add->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return add;

    } else if (subExpressions.front() == "-"){
        // sub
        RFAE *sub = new RFAE();
        sub->type = SUB;
        sub->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return sub;

    } else if (subExpressions.front() == "*"){
        // mult
        RFAE *mult = new RFAE();
        mult->type = MULT;
        mult->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return mult;

    } else if (subExpressions.front() == "if"){
        // if
        RFAE *iff = new RFAE();
        iff->type = IF;
        iff->createIf(parse(subExpressions.at(1)), parse(subExpressions.at(2)), parse(subExpressions.at(3)));
        return iff;

    } else if (subExpressions.front() == "or"){
        // or
        RFAE *orr = new RFAE();
        orr->type = OR;
        orr->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return orr;

    } else if (subExpressions.front() == "="){
        // equl
        RFAE *equl = new RFAE();
        equl->type = EQUL;
        equl->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return equl;


    } else if(subExpressions.front() == "with"){
        //[(list 'with (list i v) e)  (app (fun i (parse e)) (parse v))]  
        //{with {x 3} {+ x x}} -> (app (fun x (add (id x) (id x))) (num 3))
        RFAE* app = new RFAE();
        app->type = APP;

        RFAE* fun = new RFAE();
        fun->type = FUN;
        #ifdef DEBUG
        cout << "WITH FUN: " << parse(subExpressions.at(1))->fun_expr->name << " : " << subExpressions.at(2) << endl;
        #endif
        fun->createFun((parse(subExpressions.at(1))->fun_expr->name), parse(subExpressions.at(2)));
        
        //cout << "[parse] in with, fun: " << fun->getRFAECode() << endl;
        app->createApp(fun, parse(subExpressions.at(1))->arg_expr);
        return app;

    }else if(subExpressions.front() == "fun"){
        //fun
        RFAE* fun = new RFAE();
        fun->type = FUN;
        if(subExpressions.at(1).back() == '}') subExpressions.at(1)[subExpressions.at(1).size()-1] = 0;
        fun->createFun((parse(subExpressions.at(1))->name), parse(subExpressions.at(2)));
        return fun;

    }else if (subExpressions.size() < 2 && !isNumeric(subExpressions.front())){
        //id
        RFAE* id = new RFAE();
        id->type = ID;
        id->createID(subExpressions.front());
        return id;

    }else if(subExpressions.size() >= 2 ){ //&& !isNumeric(subExpressions.front())
        //[(list f a)                 (app (parse f) (parse a))]
        //app
        RFAE* app = new RFAE();
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

bool Parser::isRec(string str){
    
}

string Parser::desugar(string str){
    
}

