#include <stdio.h>

int main(){
  unsigned int i1 = 1835098984u;
  unsigned int i2 = 1768842611u;

  char c1 = (i1 >> 24);
  char c2 = (i1 >> 16);
  char c3 = (i1 >> 8);
  char c4 = (i1 >> 0);
  char c5 = (i2 >> 24);
  char c6 = (i2 >> 16);
  char c7 = (i2 >> 8);
  char c8 = (i2 >> 0);

  printf("%c%c%c%c%c%c%c%c\n", c1, c2, c3, c4, c5, c6, c7, c8);

}
