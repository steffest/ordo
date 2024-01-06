#include <stdlib.h>

int random(int max) {
    return rand() / (RAND_MAX/max);
}