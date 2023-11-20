#include <iostream>
#include <regex>
#include <string>
#include "RFAE.h"
// #define DEBUG

/*
"{with {fib {fun {n} {if {or {= n 0} {= n 1}} 1 {+ {fib {- n 1}} {fib {- n 2}}}}}} {fib 10}}"
{with {mk-rec {fun {body-proc} {with {fX {fun {fY} {with {f {fun {x} {{fY fY} x}}} {body-proc f}}}} {fX fX}}}} 
{with {fib {mk-rec {fun {fib} {fun {n} {if {or {= n 0} {= n 1}} 1 {+ {fib {- n 1}} {fib {- n 2}}}}}}}} {fib 10}}}


(app (fun mk-rec (app (fun fib (app (id fib) (num 10))) (app (id mk-rec) (fun fib (fun n (if (or (= (id n) (num 0)) (= (id n) (num 1))) (num 1) (add (app (id fib) (sub (id n) (num 1))) (app (id fib) (sub (id n) (num 2)))))))))) (fun body-proc (app (fun fX (app (id fX) (id fX))) (fun fY (app (fun f (app (id body-proc) (id f))) (fun x (app (app (id fY) (id fY)) (id x))))))))
(app (fun mk-rec (app (fun fib (app (id fib) (num 10))) (app (id mk-rec) (fun fib (fun n (if (or (= (id n) (num 0)) (= (id n) (num 1))) (num 1) (add (app (id fib) (sub (id n) (num 1))) (app (id fib) (sub (id n) (num 2)))))))))) (fun body-proc (app (fun fX (app (id fX) (id fX))) (fun fY (app (fun f (app (id body-proc) (id f))) (fun x (app (app (id fY) (id fY)) (id x))))))))




"{with {fac {fun {n} {if {= n 0} 1 {* n {fac {- n 1}}}}}} {fac 10}}"
{with {mk-rec {fun {body-proc} {with {fX {fun {fY} {with {f {fun {x} {{fY fY} x}}} {body-proc f}}}} {fX fX}}}} {with {fac {mk-rec {fun {fac} {fun {n} {if {= n 0} 1 {* n {fac {- n 1}}}}}}}} {fac 10}}}
{with {fac {mk-rec {fun {fac} {fun {n} {if {= n 0} 1 {* n {fac {- n 1}}}}}}}} {fac 10}}}


(app (fun mk-rec (app (fun fac (app (id fac) (num 10))) (app (id mk-rec) (fun fac (fun n (if (= (id n) (num 0)) (num 1) (mul (id n) (app (id fac) (sub (id n) (num 1)))))))))) (fun body-proc (app (fun fX (app (id fX) (id fX))) (fun fY (app (fun f (app (id body-proc) (id f))) (fun x (app (app (id fY) (id fY)) (id x))))))))
(app (fun mk-rec (app (fun fac (app (id fac) (num 10))) (app (id mk-rec) (fun fac (fun n (if (= (id n) (num 0)) (num 1) (mul (id n) (app (id fac) (sub (id n) (num 1)))))))))) (fun body-proc (app (fun fX (app (id fX) (id fX))) (fun fY (app (fun f (app (id body-proc) (id f))) (fun x (app (app (id fY) (id fY)) (id x))))))))

*/

class Parser {
public:
    vector<string> splitExpressionAsSubExpressions(string exampleCode);
    vector<string> getSubExpressions(string exampleCode);
    RFAE* parse(string exampleCode);
    bool isNumeric(string str);
    bool isRec(string id, string str);
    string desugar(string id, string value, string body);
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

    }else if (subExpressions.front().compare("+") == 0 && (subExpressions.size() == 3)){
        // add
        RFAE *add = new RFAE();
        add->type = ADD;
        add->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return add;

    } else if (subExpressions.front() == "-" && (subExpressions.size() == 3)){
        // sub
        RFAE *sub = new RFAE();
        sub->type = SUB;
        sub->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return sub;

    } else if (subExpressions.front() == "*" && (subExpressions.size() == 3)){
        // mult
        RFAE *mult = new RFAE();
        mult->type = MULT;
        mult->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return mult;

    } else if (subExpressions.front() == "if" && (subExpressions.size() == 4)){
        // if
        RFAE *iff = new RFAE();
        iff->type = IF;
        iff->createIf(parse(subExpressions.at(1)), parse(subExpressions.at(2)), parse(subExpressions.at(3)));
        return iff;

    } else if (subExpressions.front() == "or" && (subExpressions.size() == 3)){
        // or
        RFAE *orr = new RFAE();
        orr->type = OR;
        orr->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return orr;

    } else if (subExpressions.front() == "=" && (subExpressions.size() == 3)){ 
        // equl
        RFAE *equl = new RFAE();
        equl->type = EQUL;
        equl->createAE(parse(subExpressions.at(1)), parse(subExpressions.at(2)));
        return equl;


    } else if(subExpressions.front() == "with" && (subExpressions.size() == 3)){
        //[(list 'with (list i v) e)  (app (fun i (parse e)) (parse v))]  
        //{with {x 3} {+ x x}} -> (app (fun x (add (id x) (id x))) (num 3))

        // with (a b) c
        //app (fun a c) b

        // (parse(subExpressions.at(1))->fun_expr->name) : id 
        // parse(subExpressions.at(2)) : body
        // parse(subExpressions.at(1))->arg_expr : value
        string id = (parse(subExpressions.at(1))->fun_expr->name);
        string body = subExpressions.at(2);
        string value = subExpressions.at(1);

        if(isRec(id, value)){
            return parse(desugar(id, value.substr(id.length()+1, value.length()-id.length()+1), body));
        }else{
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
        }

    }else if(subExpressions.front() == "with" && (subExpressions.size() == 2)){

        cout << "what the fuck is this\n";
        cout << "[1] " << subExpressions.front() << "\n";
        cout << "[2] " << subExpressions.at(1) << "\n";

        exit(0);
    }else if(subExpressions.front() == "fun" && (subExpressions.size() == 3)){
        //fun
        RFAE* fun = new RFAE();
        fun->type = FUN;
        if(subExpressions.at(1).back() == '}') subExpressions.at(1)[subExpressions.at(1).size()-1] = 0;
        fun->createFun((parse(subExpressions.at(1))->name), parse(subExpressions.at(2)));
        return fun;

    }else if (subExpressions.size() == 1 && !isNumeric(subExpressions.front())){
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

bool Parser::isRec(string id, string str){
    //with를 만났을 때 검사
    // body만 깔끔하게 나오면 좋겠는데 그게 안되는 것 같으니까... 일단 통으로 받고 적절히 걸러야 할듯

    // body 안에 첫 인자의 id가 존재하는가? -> 검거요건 1 
    // body 안에 if문이 쓰이는가? -> 검거요건 2
    // body 안에 mk-rec이 존재하는가? -> 사격중지 아군이다
    // 여기에 body를 넣어서 구경해보자 티비. 
    bool flag = false;
    size_t first_id = str.find(id); //일단 하나는 나올테니까...?
    if(((str.find(id, first_id + 1 + id.length())) != string::npos) && ((str.find("if")) != string::npos)) flag = true;
    if((str.find("mk-rec")) != string::npos) flag = false;
    
    return flag;

}

/*
{with { [id] {mk-rec [value]} [body]}}
{with {fib {mk-rec {     fib  {fun {n} {if {or {= n 0} {= n 1}} 1 {+ {fib {- n 1}} {fib {- n 2}}}}}}} {fib 10}}}
                   ^fun {   }   
*/
string Parser::desugar(string id, string value, string body){
    string desugard_rfae = "{with {mk-rec {fun {body-proc} {with {fX {fun {fY} {with {f {fun {x} {{fY fY} x}}} {body-proc f}}}} {fX fX}}}} ";
    desugard_rfae += "{with {";
    desugard_rfae += id;
    desugard_rfae += " {mk-rec {fun {";
    desugard_rfae += id;
    desugard_rfae += "}";
    desugard_rfae += value;
    desugard_rfae += "}} ";
    desugard_rfae += body;
    desugard_rfae += "}}";
    #ifdef DEBUG
    cout << "[desugar] " << desugard_rfae << endl;
    #endif
    cout << "[desugar] " << desugard_rfae << endl;
    return desugard_rfae;
    
}

