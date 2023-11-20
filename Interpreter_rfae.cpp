#include <iostream>
#include <regex>
#include <string>
#include "RFAE.h"
// #define DEBUG
string E[16] = {"NULL", "NUM", "ADD", "SUB", "ID", "FUN", "APP", "NUMV", "CLOSUREV", "MULT", "IF", "OR", "EQUL"};


class Interpreter{
    public:
    vector<pair<string, RFAE_Value*>> ds;

    RFAE_Value interp(RFAE*, vector<pair<string, RFAE_Value*>>); //change return value to RFAE_Value
    RFAE_Value lookup(string, vector<pair<string, RFAE_Value*>>);
    RFAE_Value addNumV(RFAE_Value, RFAE_Value);
    RFAE_Value subNumV(RFAE_Value, RFAE_Value);
    RFAE_Value multNumV(RFAE_Value, RFAE_Value);
    RFAE_Value ifNumV(RFAE_Value, RFAE_Value, RFAE_Value);
    RFAE_Value orNumV(RFAE_Value, RFAE_Value);
    RFAE_Value equlNumV(RFAE_Value, RFAE_Value);

};

RFAE_Value Interpreter::interp(RFAE* RFAE, vector<pair<string, RFAE_Value*>> ds){
    RFAE_Value* val = new RFAE_Value();
    RFAE_Value fun_val;
    pair<string, RFAE_Value*> newVal;

    #ifdef DEBUG
    cout << "[Interp] :" << E[RFAE->type] << ", ds size is " << ds.size() << endl;
    #endif

    switch (RFAE->type) {

    case NUM:
    val->type = NUMV;
    val->numV = stoi(RFAE->num);
    
    return *val;
    
    case ADD:
    return addNumV(interp(RFAE->lhs, ds), interp(RFAE->rhs, ds));

    case SUB:
    return subNumV(interp(RFAE->lhs, ds), interp(RFAE->rhs, ds));

    case MULT:
    return multNumV(interp(RFAE->lhs, ds), interp(RFAE->rhs, ds));

    case IF:
    return ifNumV(interp(RFAE->cond, ds), interp(RFAE->lhs, ds), interp(RFAE->rhs, ds));

    case OR:
    return orNumV(interp(RFAE->lhs, ds), interp(RFAE->rhs, ds));

    case EQUL:
    return equlNumV(interp(RFAE->lhs, ds), interp(RFAE->rhs, ds));
    
    case ID:
    return lookup(RFAE->name, ds);
    
    case FUN:
    val->createClosureV(RFAE->param, RFAE->body, ds);
    return *val;

    case APP:
    //fun-expr ->interp하면 param, body가 나옴 
    //                  -> param이랑 arg-expr의 interp을 ds로 줘야함

    fun_val = interp(RFAE->fun_expr, ds);
    // if(fun_val.type != FUN){
    //     cout << "[Interp] contract violation error\n";
    //     exit(1);
    // }

    newVal.first = (fun_val.param); 
    newVal.second = new RFAE_Value(interp(RFAE->arg_expr, ds));
    ds.push_back(newVal);
    #ifdef DEBUG
    cout << "[Interp] ds added: " << ds.back().first << ", now size of ds is " << ds.size() << endl;
    #endif

    return interp(fun_val.body, ds);

    default:
        cout << "[Interp] invalid expression\n";
        break;
    }

}

//‘{with {x {with {y 5} x}} y}’

// '(with (x (with (y 5) x)) y)
RFAE_Value Interpreter::lookup(string id, vector<pair<string, RFAE_Value*>> dss){

    #ifdef DEBUG
    cout << "num of ds: " << dss.size()<< endl;
    #endif
    for(auto it = dss.begin(); it < dss.end(); it++){
        //cout << "[lookup] compair id " << (it->first).c_str() << " and " << id.c_str() << ": "<< strcmp((it->first).c_str(), id.c_str()) << endl;
        if (strcmp((it->first).c_str(), id.c_str()) == 0){
            return *(it->second);
        }
    }
    cout << "[lookup] no binding for identifier: " << id << endl;
    exit(1);

}

//add+
RFAE_Value Interpreter::addNumV(RFAE_Value a, RFAE_Value b){
    RFAE_Value result;
    result.type = NUMV;
    result.numV = a.numV + b.numV;
    return result;

}

//sub+
RFAE_Value Interpreter::subNumV(RFAE_Value a, RFAE_Value b){
    RFAE_Value result;
    result.type = NUMV;
    result.numV = a.numV - b.numV;
    return result;
}

//mult+
RFAE_Value Interpreter::multNumV(RFAE_Value a, RFAE_Value b){
    RFAE_Value result;
    result.type = NUMV;
    result.numV = a.numV * b.numV;
    return result;
}

//if+
RFAE_Value Interpreter::ifNumV(RFAE_Value cond, RFAE_Value a, RFAE_Value b){
    #ifdef DEBUG
    cout << "[ifNUmV] " << cond.getRFAEVCode() << ", " << a.getRFAEVCode() << ", " << b.getRFAEVCode() << endl;
    #endif
    if(cond.numV) return a;
    else return b;
}

//or+
RFAE_Value Interpreter::orNumV(RFAE_Value a, RFAE_Value b){
    RFAE_Value result;
    result.type = NUMV;
    if(a.numV || b.numV) result.numV = 1;
    else result.numV = 0;
    return result;
}

//equl+
RFAE_Value Interpreter::equlNumV(RFAE_Value a, RFAE_Value b){
    RFAE_Value result;
    result.type = NUMV;
    if(a.numV == b.numV) result.numV = 1;
    else result.numV = 0;
    return result;
}