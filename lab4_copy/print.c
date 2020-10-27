#include <stdio.h>



void print(char *str){

  do{
    putchar(*str);
      str++;
  } while (*str != '\0');
}


int main(){

  print("hej");
  return 0;
}


