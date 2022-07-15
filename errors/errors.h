// This is the main file for throwing errors.
#pragma once

#include"nonlocalized_errors.h" // for algr_nonlocalized_throw(...).
#include"localized_errors.h" // for algr_throw_with_location_info(...), algr_location_infos_available(...)


// Public:
// algr_throw(...)


void algr_throw(void){
        if(algr_location_infos_available())
                algr_throw_with_location_infos();
        else
                algr_nonlocalized_throw();
        // aborts in both cases. 

}
