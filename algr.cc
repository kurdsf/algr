#include"algr.h"

_Noreturn void __unreachable(char* file, int line){
    fprintf(stderr,"Internal Error at %s:%d.\n",file,line);
    abort();
}
