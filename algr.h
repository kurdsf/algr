#ifndef ALGR_H
#define ALGR_H

#include<stdio.h>
#include<stdlib.h>

#define unreachable() __unreachable(__FILE__,__LINE__);
    
_Noreturn void  __unreachable(char* file, int line);

#endif /* ALGR_H */


