#include <iostream>
#include "FAE.h"
#include "Parser.cpp"
#include "Interpreter.cpp"
using namespace std;
#define DEBUG

int main(int argc, char *argv[]){
    string pOption, ConcreteCode;
    FAE* fae;
    Parser parser;
    interpreter interpreter;

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
        cout << fae->getFAECode();
        
        return 0;   
    }
    else if(pOption == "-i"){
        #ifdef DEBUG
        cout << "input expr: " << ConcreteCode << endl; 
        #endif
        //TODO 
        fae = parser.parse(ConcreteCode);
        cout << "parsed: " << fae->getFAECode() << endl;
        cout << "result: " << interpreter.interp(fae, {}).getFAEVCode() << endl;
        return 0;
    } else cout << "Syntax Error." ; 

    return 0;
}