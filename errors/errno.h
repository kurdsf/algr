// Here the global error indicator and the types of errors are declared.
#pragma once

// List of error codes
enum algr_ErrorCodes {
        algr_NoError,
        algr_MallocFail,
        // ...
};



// Global variable which is always set if something got wrong.
int algr_errno = algr_NoError;

// for example algr_errno_map[NoError] = algr_errno_map[0] = "NoError".
const char* algr_errno_map[] = {"NoError"};




