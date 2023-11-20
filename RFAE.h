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
#define MULT 9
#define IF 10
#define OR 11
#define EQUL 12

/* (define-type RFAE
    [num (n number?)]
    [add (lhs RFAE?) (rhs RFAE?)]
    [sub (lhs RFAE?) (rhs RFAE?)]

    [mult (lhs RFAE?) (rhs RFAE?)]
    [if (cond RFAE?) (lhs RFAE?) (rhs RFAE?)]
    [or (lhs RFAE?) (rhs RFAE?)]
    [equl (lhs RFAE?) (rhs RFAE?)]

    [id (name symbol?)]
    [fun (param symbol?) (body RFAE?)]
    [app (fun-expr RFAE?) (arg-expr RFAE?)])
*/

class RFAE{
    public:
        int type;

        string num = "0";//for num
        RFAE *lhs;//for AE
        RFAE *rhs;
        RFAE *cond; //for if
        string name;//for id

        string param;//for fun
        RFAE* body;

        RFAE* fun_expr;//for app
        RFAE* arg_expr;

        void createNum(string);
        void createAE(RFAE*, RFAE*);
        void createIf(RFAE* , RFAE *, RFAE *);
        void createID(string);
        void createFun(string, RFAE*);
        void createApp(RFAE*, RFAE*);

        string getRFAECode();
};

//save AE and cond expression 
void RFAE::createNum(string stringNum) {
    num = stringNum;
}

//save lhs and rhs for add
void RFAE::createAE(RFAE *RFAElhs, RFAE *RFAErhs) {
    lhs = RFAElhs;
    rhs = RFAErhs;
}

//save if 
void RFAE::createIf(RFAE* RFAECond, RFAE *RFAElhs, RFAE *RFAErhs) {
    cond = RFAECond;
    lhs = RFAElhs;
    rhs = RFAErhs;
}

//save id
void RFAE::createID(string stringSymbol) {
    name = stringSymbol;
}

//save fun
void RFAE::createFun(string input_param, RFAE* input_body){
    param = input_param;
    body = input_body;
}

//save app
void RFAE::createApp(RFAE* input_fun_expr, RFAE* input_arg_expr){
    fun_expr = input_fun_expr;
    arg_expr = input_arg_expr;
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
//make parsed code
string RFAE::getRFAECode() {
    string RFAECode = "";
    string numCode = "(num ";
    string addCode = "(add ";
    string subCode = "(sub ";
    string multCode = "(mult ";
    string ifCode = "(if ";
    string orCode = "(or ";
    string equlCode = "(equl ";
    string idCode = "(id ";
    string funCode = "(fun ";
    string appCode = "(app ";

    switch(type){
        case NUM:
        RFAECode += numCode;
        RFAECode += num;
        RFAECode += ")";
        return RFAECode;

        case ADD:
        RFAECode += addCode;
        RFAECode += lhs->getRFAECode();
        RFAECode += " ";
        RFAECode += rhs->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case SUB:
        RFAECode += subCode;
        RFAECode += lhs->getRFAECode();
        RFAECode += " ";
        RFAECode += rhs->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case MULT:
        RFAECode += multCode;
        RFAECode += lhs->getRFAECode();
        RFAECode += " ";
        RFAECode += rhs->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case IF:
        RFAECode += ifCode;
        RFAECode += cond->getRFAECode();
        RFAECode += " ";
        RFAECode += lhs->getRFAECode();
        RFAECode += " ";
        RFAECode += rhs->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case OR:
        RFAECode += orCode;
        RFAECode += lhs->getRFAECode();
        RFAECode += " ";
        RFAECode += rhs->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case EQUL:
        RFAECode += equlCode;
        RFAECode += lhs->getRFAECode();
        RFAECode += " ";
        RFAECode += rhs->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case ID:
        RFAECode += idCode;
        RFAECode += name;
        RFAECode += ")";
        return RFAECode;

        case FUN:
        //[(list 'fun (list p) b)                 (fun p (parse b))]
        RFAECode += funCode;
        RFAECode += param;
        RFAECode += " ";
        RFAECode += body->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        case APP:
        RFAECode += appCode;
        RFAECode += fun_expr->getRFAECode();
        RFAECode += " ";
        RFAECode += arg_expr->getRFAECode();
        RFAECode += ")";
        return RFAECode;

        default:
        printf("invalid syntax\n");

    }
    return RFAECode;
}


// ----------------------------------------------------
/*(define-type DefrdSub
  [mtSub]
  [aSub   (name symbol?) (value RFAE-Value?) 
          (ds DefrdSub?)])
*/
/*(define-type RFAE-Value
  [numV      (n number?)]
  [closureV (param symbol?)
                  (body RFAE?)
                  (ds DefrdSub?)])
*/
class RFAE_Value{
    public:
        int type;

        int numV;
        string param;
        RFAE* body;
        vector<pair<string, RFAE_Value*>> ds = {}; //defrdSub (list of pair of id, value)

        void createNumV(string);
        void createClosureV(string, RFAE*, vector<pair<string, RFAE_Value*>>);
        string getRFAEVCode();
        string printDs(vector<pair<string, RFAE_Value*>>);

};

//save numV
void RFAE_Value::createNumV(string inputNum){
    type = NUMV;
    numV = stoi(inputNum);
}

// save closureV
void RFAE_Value::createClosureV(string inputParam, RFAE* inputBody, vector<pair<string, RFAE_Value*>> inputDs){
    type = CLOSUREV;
    param = inputParam;
    body = inputBody;
    ds = inputDs;
}

string RFAE_Value::getRFAEVCode(){
    string RFAEVCode = "";
    string numVCode = "(numV ";
    string closureVCode = "(closureV ";

    if(type == NUMV){
        RFAEVCode += numVCode;
        RFAEVCode += to_string(numV);
        RFAEVCode += ")";
        return RFAEVCode;
    }

    if(type == CLOSUREV){
        RFAEVCode += closureVCode;
        RFAEVCode += param;
        RFAEVCode += " ";
        RFAEVCode += body->getRFAECode();
        RFAEVCode += " ";
        RFAEVCode += printDs(ds);
        RFAEVCode += ")";
        return RFAEVCode;
    }

}

string RFAE_Value::printDs(vector<pair<string, RFAE_Value*>> dss){
    
    if(dss.size() == 0){
        return "(mtSub)";
    }else{
        vector<pair<string, RFAE_Value*>> rest;
        rest.resize((int)(dss.size() -1));
        copy(dss.begin()+1, dss.end(), rest.begin());
        return "(aSub \'" + dss.front().first + " " + (dss.front().second)->getRFAEVCode() + " " + printDs(rest) + ")";
    }

}
