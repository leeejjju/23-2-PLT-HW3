#include <iostream>
#include <regex>
#include <string>
#include "FAE.h"
#define DEBUG


class interpreter{
    public:
    vector<pair<string, FAE_Value*>> ds;

    FAE_Value interp(FAE*, vector<pair<string, FAE_Value*>>); //change return value to FAE_Value
    FAE_Value lookup(string);
    FAE_Value addNumV(FAE_Value, FAE_Value);
    FAE_Value subNumV(FAE_Value, FAE_Value);

};

FAE_Value interpreter::interp(FAE* expr, vector<pair<string, FAE_Value*>> ds){
    FAE_Value* val = new FAE_Value();
    pair<string, FAE_Value*> newVal;
    switch (expr->type) {

    case NUM:
    val->type = NUMV;
    val->numV = stoi(expr->num);
    
    return *val;
    
    case ADD:
    return addNumV(interp(expr->lhs, ds), interp(expr->rhs, ds));

    case SUB:
    return subNumV(interp(expr->lhs, ds), interp(expr->rhs, ds));
    
    case ID:
    return lookup(expr->name);
    
    case FUN:
    val->type = CLOSUREV;

    val->createClosureV(expr->param, expr->body, ds);
    return *val;

    case APP:
    newVal.first = (interp(expr->fun_expr, ds).param->name);
    newVal.second = new FAE_Value(interp(expr->arg_expr, ds));
    ds.push_back(newVal);

    return interp((interp(expr->fun_expr, ds).body), ds);

    default:
        break;
    }

}

FAE_Value interpreter::lookup(string id){
    auto it = find_if(ds.begin(), ds.end(), [id](const auto& var){
        return var.first == id;
    });
    if(it != ds.end()){
        return *(it->second);
    }else{
        cout << "[Interp] no such id: " << id << endl;
        exit(1);
    }

}

//add+
FAE_Value interpreter::addNumV(FAE_Value a, FAE_Value b){
    FAE_Value result;
    result.type = NUMV;
    result.numV = a.numV + b.numV;
    return result;

}

//sub+
FAE_Value interpreter::subNumV(FAE_Value a, FAE_Value b){
    FAE_Value result;
    result.type = NUMV;
    result.numV = a.numV - b.numV;
    return result;
}