#include<iostream>
#include<string>
#include<cstring>

#include"presedence.h"
#include"parser.h"
#include"optimizer.h"


int main(){
    AST* ast;
    while(1){
        std::string input;
        std::cout << "algr>";
        std::getline(std::cin, input);

        if(input == "q\n") {
            break;
        }

        if(input[0] == '#'){
            continue;
        }
        // TODO: Fix this workaround, make parse accept const char*.
        char* tmp = new char[input.size() + 1];
        strcpy(tmp, input.c_str());
        tmp[input.size()] = '\0';
        ast = parse(tmp);
        delete[] tmp;
        ast = optimize(ast);
        putsAST(ast);
        std::cout << '\n';
        freeAST(ast);
    }
    return 0;
}
 
