#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;
#define NUM 1
#define ADD 2
#define SUB 3
#define ID 4
#define FUN 5
#define APP 6
#define NUMV 7
#define CLOSUREV 8

/* (define-type FAE
    [num (n number?)]
    [add (lhs FAE?) (rhs FAE?)]
    [sub (lhs FAE?) (rhs FAE?)]
    [id (name symbol?)]
    [fun (param symbol?) (body FAE?)]
    [app (fun-expr FAE?) (arg-expr FAE?)])
*/
class FAE{
    public:
        int type;

        string num = "0";//for num
        FAE *lhs;//for AE
        FAE *rhs;
        string name;//for id

        string param;//for fun
        FAE* body;

        FAE* fun_expr;//for app
        FAE* arg_expr;

        void createNum(string);
        void createAdd(FAE*, FAE*);
        void createSub(FAE*, FAE*);
        void createID(string);
        void createFun(string, FAE*);
        void createApp(FAE*, FAE*);

        string getFAECode();
};

//save num 
void FAE::createNum(string stringNum) {
    num = stringNum;
}

//save lhs and rhs for add
void FAE::createAdd(FAE *FAElhs, FAE *FAErhs) {
    lhs = FAElhs;
    rhs = FAErhs;
}

//save lhs and rhs for add
void FAE::createSub(FAE *FAElhs, FAE *FAErhs) {
    lhs = FAElhs;
    rhs = FAErhs;
}

//save id
void FAE::createID(string stringSymbol) {
    name = stringSymbol;
}

//save fun
void FAE::createFun(string input_param, FAE* input_body){
    param = input_param;
    body = input_body;
}

//save app
void FAE::createApp(FAE* input_fun_expr, FAE* input_arg_expr){
    fun_expr = input_fun_expr;
    arg_expr = input_arg_expr;
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
//make parsed code
string FAE::getFAECode() {
    string FAECode = "";
    string numCode = "(num ";
    string addCode = "(add ";
    string subCode = "(sub ";
    string idCode = "(id ";
    string funCode = "(fun ";
    string appCode = "(app ";

    switch(type){
        case NUM:
        FAECode += numCode;
        FAECode += num;
        FAECode += ")";
        return FAECode;

        case ADD:
        FAECode += addCode;
        FAECode += lhs->getFAECode();
        FAECode += " ";
        FAECode += rhs->getFAECode();
        FAECode += ")";
        return FAECode;

        case SUB:
        FAECode += subCode;
        FAECode += lhs->getFAECode();
        FAECode += " ";
        FAECode += rhs->getFAECode();
        FAECode += ")";
        return FAECode;

        case ID:
        FAECode += idCode;
        FAECode += name;
        FAECode += ")";
        return FAECode;

        case FUN:
        //[(list 'fun (list p) b)                 (fun p (parse b))]
        FAECode += funCode;
        FAECode += param;
        FAECode += " ";
        FAECode += body->getFAECode();
        FAECode += ")";
        return FAECode;

        case APP:
        FAECode += appCode;
        FAECode += fun_expr->getFAECode();
        FAECode += " ";
        FAECode += arg_expr->getFAECode();
        FAECode += ")";
        return FAECode;

        default:
        printf("invalid syntax\n");

    }
    return FAECode;
}


// ----------------------------------------------------
/*(define-type DefrdSub
  [mtSub]
  [aSub   (name symbol?) (value FAE-Value?) 
          (ds DefrdSub?)])
*/
/*(define-type FAE-Value
  [numV      (n number?)]
  [closureV (param symbol?)
                  (body FAE?)
                  (ds DefrdSub?)])
*/
class FAE_Value{
    public:
        int type;

        int numV;
        string param;
        FAE* body;
        vector<pair<string, FAE_Value*>> ds = {}; //defrdSub (list of pair of id, value)

        void createNumV(string);
        void createClosureV(string, FAE*, vector<pair<string, FAE_Value*>>);
        string getFAEVCode();
        string printDs(vector<pair<string, FAE_Value*>>);

};

//save numV
void FAE_Value::createNumV(string inputNum){
    type = NUMV;
    numV = stoi(inputNum);
}

// save closureV
void FAE_Value::createClosureV(string inputParam, FAE* inputBody, vector<pair<string, FAE_Value*>> inputDs){
    type = CLOSUREV;
    param = inputParam;
    body = inputBody;
    ds = inputDs;
}

string FAE_Value::getFAEVCode(){
    string FAEVCode = "";
    string numVCode = "(numV ";
    string closureVCode = "(closureV ";

    if(type == NUMV){
        FAEVCode += numVCode;
        FAEVCode += to_string(numV);
        FAEVCode += ")";
        return FAEVCode;
    }

    if(type == CLOSUREV){
        FAEVCode += closureVCode;
        FAEVCode += param;
        FAEVCode += " ";
        FAEVCode += body->getFAECode();
        FAEVCode += " ";
        FAEVCode += printDs(ds);
        FAEVCode += ")";
        return FAEVCode;
    }

}

string FAE_Value::printDs(vector<pair<string, FAE_Value*>> dss){
    
    if(dss.size() == 0){
        return "(mtSub)";
    }else{
        vector<pair<string, FAE_Value*>> rest;
        rest.resize((int)(dss.size() -1));
        copy(dss.begin()+1, dss.end(), rest.begin());
        return "(aSub \'" + dss.front().first + " " + (dss.front().second)->getFAEVCode() + " " + printDs(rest) + ")";
    }

}
