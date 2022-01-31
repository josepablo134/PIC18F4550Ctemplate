#include "../inc/project_macros.h"

#ifdef __DEBUG
void __error__(void){
    for(;;);
}
#endif