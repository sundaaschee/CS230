#include <stdio.h>

int main(){

  unsigned int whatever =  17512807u;

  int masking = ((1 << (21 - 18 + 1)) - 1) << 18;
  int shift = (8 << 18) & masking; // right shift
  int remove = whatever & (~ masking); //remove the range
  whatever = shift | remove; //update the range
  
  masking = ((1 << (14 - 10 + 1)) - 1) << 10;
  shift = (17 << 10) & masking;
  remove = whatever & (~ masking);
  whatever = shift | remove;

  printf("%08x\n", whatever);

}
