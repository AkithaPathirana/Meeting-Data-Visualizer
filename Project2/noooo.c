#include<stdio.h>
int main(){
  FILE *fp=fopen("meetingData10.csv","r"); // open file in reading mode
  if (fp == NULL){
    printf("Hi");
  }
  else{
    printf("ho");
  }



  return 0;

}

fseek (fp, 0, SEEK_END);
size = ftell(fp);
if (0 == size) {
  continue;
}
else{
  fseek (fp,0,SEEK_SET);
}


if(data_list_number==1){ //this is the first file
  inputs=NULL;
}
