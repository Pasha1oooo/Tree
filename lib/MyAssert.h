#ifndef MyAssert_H
#define MyAssert_H

#include "COLORS.h"

#define MyAssert(ARG)                                       \
    if(ARG){                                             \
        printf("%s " WordRED("ERROR") ":" #ARG              \
           " FILE: %s  FUNCTION: %s  LINE: %d\n",           \
            __TIME__, __FILE__, __FUNCTION__,__LINE__);     \
        abort();                                            \
    }

#endif
