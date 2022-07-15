// Error handling  with global error indicator in algr.

// Errro handling is done like this:
// // do something which could cause a library error 
// if(algr_errno != algr_NoError){
//        // handle error
          // for example algr_throw() defined in errors.h
// }
// //  no error occured 


#pragma once
#include<stdio.h> // for fprintf(...)
#include<stdlib.h> // for abort(...)

#include"errno.h" // for algr_errno_map, algr_errno

// Public:
void algr_nonlocalized_throw(void);
// Internal:
// (None)



// Print a textual representation (as defined by algr_errno_map) of algr_errno to the stderr
// then abort.
void algr_nonlocalized_throw(void){
        fprintf(stderr,"Error: %s.",algr_errno_map[algr_errno]);
        abort();
}








