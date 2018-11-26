#include <stdio.h>

int main(){

  unsigned int cafebabe = 0xCAFEBABE;
  int widths[] = {3, 4, 4, 3, 3, 4, 4, 3, 2, 2};
  unsigned int temp = 0;
  int wsf = 0; //width so far
  
  for(int i = 0; i < 10; i++){
    temp = cafebabe << wsf;
     wsf = wsf +  widths[i];
     temp = temp >> (32 - widths[i]);
    printf("%u", temp);
     if(i == 9){
       printf("\n");
       break;
     }
    printf(" ");
  }    
}
