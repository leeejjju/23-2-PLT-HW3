#include <iostream>
#include <regex>
#include <string>
#include "FAE.h"
// #define DEBUG
string E[9] = {"NULL", "NUM", "ADD", "SUB", "ID", "FUN", "APP", "NUMV", "CLOSUREV"};


class Interpreter{
    public:
    vector<pair<string, FAE_Value*>> ds;

    FAE_Value interp(FAE*, vector<pair<string, FAE_Value*>>); //change return value to FAE_Value
    FAE_Value lookup(string, vector<pair<string, FAE_Value*>>);
    FAE_Value addNumV(FAE_Value, FAE_Value);
    FAE_Value subNumV(FAE_Value, FAE_Value);

};

FAE_Value Interpreter::interp(FAE* fae, vector<pair<string, FAE_Value*>> ds){
    FAE_Value* val = new FAE_Value();
    FAE_Value fun_val;
    pair<string, FAE_Value*> newVal;

    #ifdef DEBUG
    cout << "[Interp] :" << E[fae->type] << ", ds size is " << ds.size() << endl;
    #endif

    switch (fae->type) {

    case NUM:
    val->type = NUMV;
    val->numV = stoi(fae->num);
    
    return *val;
    
    case ADD:
    return addNumV(interp(fae->lhs, ds), interp(fae->rhs, ds));

    case SUB:
    return subNumV(interp(fae->lhs, ds), interp(fae->rhs, ds));
    
    case ID:
    return lookup(fae->name, ds);
    
    case FUN:
    val->createClosureV(fae->param, fae->body, ds);
    return *val;

    case APP:
    //fun-expr ->interp하면 param, body가 나옴 
    //                  -> param이랑 arg-expr의 interp을 ds로 줘야함

    fun_val = interp(fae->fun_expr, ds);
    if(fun_val.type != FUN){
        cout << "[Interp] contract violation error\n";
        exit(1);
    }

    newVal.first = (fun_val.param); 
    newVal.second = new FAE_Value(interp(fae->arg_expr, ds));
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
FAE_Value Interpreter::lookup(string id, vector<pair<string, FAE_Value*>> dss){

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
FAE_Value Interpreter::addNumV(FAE_Value a, FAE_Value b){
    FAE_Value result;
    result.type = NUMV;
    result.numV = a.numV + b.numV;
    return result;

}

//sub+
FAE_Value Interpreter::subNumV(FAE_Value a, FAE_Value b){
    FAE_Value result;
    result.type = NUMV;
    result.numV = a.numV - b.numV;
    return result;
}