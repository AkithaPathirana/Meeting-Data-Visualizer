#include<stdio.h>

int non_recursive(int num1,int num2);

int main(){
  int num1,num2;
  printf("Enter numbers\n");
  scanf("%d %d",&num1,&num2);
  printf("The GCD of %d and %d is %d ", num1,num2,non_recursive(num1,num2));
}

int non_recursive(int num1,int num2){
 int intermediate,remain;
 if(num1<num2){
   intermediate=num1;
   num1=num2;
   num2=intermediate;
 }
 while(num2!=0){
   remain=num1%num2;
   num1=num2;
   num2=remain;
 }
 return num1;
}
