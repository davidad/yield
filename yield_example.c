#include <stdio.h>
#include "yield.h"

void loop_range(yield_t* y, int begin, int end) {
  int i;
  for(i=begin; i<end; i++) {
    YIELD(y,&i)
  }
  TERMINATE(y)
}

void loop_powers(yield_t* y, int base, int exp) {
  int i, n=1;
  for(i=0; i<exp; i++) {
    n*=base;
    YIELD(y,&n)
  }
  TERMINATE(y)
}

int main(void) {
  /*INVOKE: first argument   - unique name for loop
   *        second argument  - type of loop variable
   *        third argument   - name of loop variable
   *        fourth argument  - loop generator function
   *        fifth argument   - number of arguments to pass
   *        sixth argument   - additional arguments after yield_t
   */
  INVOKE(outer,int,i,loop_range,3, 0,10)
    printf("i=%d\n", i);
    INVOKE(inner,int,k,loop_powers,3, i,10)
      if(k>100000) {END_EARLY(inner);}
      printf(" k=%d\n", k);
    CLOSE(inner)
  CLOSE(outer)
  return 0;
}
