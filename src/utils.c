#include <stdio.h>
#include <stdlib.h>

#include "utils.h"


unsigned int get_next_backets_num(unsigned int number) {
    if (number >= 20 && number < 200) {
        return number + 20;
    }
    if (number >= 200 && number < 2000) {
        return number + 200;
    }
    if (number >= 2000 && number < 20000) {
        return number + 2000;
    }
    if (number >= 20000 && number < 200000) {
        return number + 20000;
    }
    return 0;
}


