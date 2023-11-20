#include <iostream>
#include "RFAE.h"
#include "Parser_rfae.cpp"
#include "Interpreter_rfae.cpp"
using namespace std;
#define DEBUG

int main(int argc, char *argv[]){
    string pOption, ConcreteCode;
    RFAE* fae;
    Parser parser;
    Interpreter interpreter;

    #ifdef DEBUG 
    cout << "Let's start!\n";
    #endif

    if(argc >= 3){
        if(strcmp(argv[1], "-p") == 0){
            pOption = argv[1];
            ConcreteCode = argv[2];
        }
        else    
            return 0;
    }
    else if(argc == 2){
        pOption = "-i";
        ConcreteCode = argv[1];    
    }

    if(pOption == "-p"){
        #ifdef DEBUG 
        cout << "input expr: " << ConcreteCode << endl;
        #endif
        fae = parser.parse(ConcreteCode);
        cout << fae->getRFAECode();
        
        return 0;   
    }
    else if(pOption == "-i"){
        #ifdef DEBUG
        cout << "input expr: " << ConcreteCode << endl; 
        #endif
        //TODO 
        fae = parser.parse(ConcreteCode);
        cout << "parsed: " << fae->getRFAECode() << endl;
        cout << "result: " << interpreter.interp(fae, {}).getRFAEVCode() << endl;
        return 0;
    } else cout << "Syntax Error." ; 

    return 0;
}