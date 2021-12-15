#include "../../include/config/project_macros.h"

#ifdef DEBUG
void __error__(void){
    for(;;);
}
#endif