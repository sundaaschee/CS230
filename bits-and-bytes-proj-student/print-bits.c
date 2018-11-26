#include <stdio.h>

int main(){
  unsigned char a = 181;
  signed char b = -75;

  for(int i = 7; i > -1; i--){
    printf("%d", (a >> i) & 1);
  }

  printf("\n");

  for(int j = 7; j > -1; j--){
    printf("%d", (b >> j) & 1);
  }

  printf("\n");
  
}
