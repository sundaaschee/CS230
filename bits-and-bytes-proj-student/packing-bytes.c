#include <stdio.h>

int main(){
  unsigned char b3 = 202;
  unsigned char b2 = 254;
  unsigned char b1 = 186;
  unsigned char b0 = 190;

  unsigned int u = 0;
  u = u | (b0 << 0) | (b1 << 8) | (b2 << 16) | (b3 << 24);

  printf("%x\n", u);
  
}
