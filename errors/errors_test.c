// Because testing that the correct erros are thrown in C is very hard
// We will just print the output and DO NOT DO ANY KIND OF CHECKS.
// The human has to verify that the output is correct.
#include"errors.h" // for algr_errno,algr_throw(...),algr_linenum,algr_linepos

int main(){
        algr_errno_linenum = 0;
        algr_errno_linepos = 0;
        algr_throw();
        return 0;
}
