#include "helper.h"

unsigned umax(unsigned l, unsigned r) {
   return l < r ? r : l;
}

/* comparator function for qsort() */
int comp(const void *p1, const void *p2) {
   return *((int*)p1) - *((int*)p2);
}
