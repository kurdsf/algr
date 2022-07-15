#pragma once
/*
  You can optionally set errno_linenum and errno_linepos for further
  information to the information of the error.
  They default to -1.
*/
#include<stdio.h> // for fprintf(...)
#include<stdlib.h> // for abort(...)


#include"errno.h" // for algr_errno_map,algr_errno


// Public:
void algr_throw_with_location_infos(void);
char algr_location_infos_available(void);
// Internal:
// (None)

 


int algr_errno_linenum = -1; 
int algr_errno_linepos = -1;


void algr_throw_with_location_infos(void){
        fprintf(stderr,
                "Error: %s.\n"
                "\t(At line %d:%d)\n",
                algr_errno_map[algr_errno],
                algr_errno_linenum,
                algr_errno_linepos);

        abort();
}


char algr_location_infos_available(void){
        return (algr_errno_linenum != -1) && (algr_errno_linepos != -1);
}




