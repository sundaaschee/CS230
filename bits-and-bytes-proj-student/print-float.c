#include <stdio.h>

int main(){
  float x = 10.000000;
  float y = 13.000000;
  printf("x = %8.6f\n", x);
  printf("y = %8.6f\n", y);

  printf("size of single float in bytes is %d.\n", sizeof(x));
  printf("size of single float in bits is %d.\n", sizeof(x)*8);

  float z = x + y;

  printf("%8.6f + %8.6f = %8.6f.\n", x, y, z);
  printf("%8.6f + %8.6f = %2.0f.\n", x, y, z);
}
