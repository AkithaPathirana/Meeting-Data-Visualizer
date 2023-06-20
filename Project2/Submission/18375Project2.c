#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct data {
  char name[40]; //where the name will be kept
  int no_meetings;//number of meetings taken by a given person
  int participants; //Number of participants for all meetings
  int duration; //duration of all meetings
  int rank; //Unique entries are ranked according to their input position.first input recieved is 1 and last unique input is ranked last
  struct data *next; //reference to the data
} data_t;


//Global variable
data_t *inputs=NULL; // inputs is a reference to data_t
int elements=0;//keep track of the distinct elements




//defined functions
int get_number(char *);//to get the number of output rows
data_t *identity(char *); //this function will check whether a name is already in the user data provided.
int meeting_time(char *); //this functions the duration of a meeting in minutes
void sort_meetings(data_t **,int); //this functions populates an array named top_elements (this array is defined in the middle of the main function) with pointers pointing at highest meeting number elements
void sort_participants(data_t **,int);//this functions populates the top_elements array with pointers pointing at highest participant number elements
void sort_time(data_t **,int); //this functions populates the top_elements array with pointers pointing at longest time duration elements
void sort_top_elements(int,char,data_t **); //this function will sort the array top_elements based on a given parameter in decending order and populate top_element with the pointers
void print_chart(char,int,int,data_t **);// this function will print the graph as given in the sample programme
int funDigits(int);//function to calculate the number of digits in a integer

int main(int argc,char **argv){


  data_t *intermediate;// this is used to temporarily store the adress of the structure created by reading a line
  data_t *identifier;// this is used to identify uf a certain person is already in data_t
  char buff[100];//stores the first 100 lines into buff
  int field_count;
  int data_list_number=0; // this is to keep track of the number of user entered data lists
  int user_argument; //variable to go through user arguments

/*Parameters taken from the user */
  char user_request='n'; //requested parameter to be printed by the user 'm'-no_of_meetings  'p'-number of participants 't'-duration.initiallized to 'n' to indicate that no argumment had been taken so far
  int user_output_rows=10; //requested number of output rows by the user.if no argument is given 10 is taken as the default value
  int user_scale=0;//whether user wants scaled rows or not


/*parameters used for error checking*/
 int error_index;//this will store the index at which the user input is not compatible
 int error_message=0;//this will store the type of the error message. if no error is found it will remain at 0.
 int return_value;//used to store return value of a function temporarily
 int row_value_index;//used to store the index at which the user entered the number of rows to output
 long size;//return value of ftell()

/*IMPORTANT- A Local variable data_t *top_elements[user_output_rows]; is defined at the 104th line of the code*/

/*Error messages*/
/*  -1 =Cannot plot multiple parameters in same graph.*/ /*ex if error_message=-1,this will be printed*/
/*  -2 =Invalid options for [-l]*/
/*  -3 =Not enough options for [-l]*/
/*  -4 =Invalid option [-input] */
/*  -5 =Invalid option [--input]*/
/*  -6 =No input files were given*/
/*  -7=Invalid option(negative) for [-l]*/


  /*handling arguments given by the user*/
  for(user_argument=1;user_argument<argc;++user_argument){
    if(argv[user_argument][0]=='-' && argv[user_argument][1]!='-'){
      if(argv[user_argument][1]=='m' && argv[user_argument][2]=='\0'){
        if(user_request=='n'||user_request=='m'){
          user_request='m'; //output paramater
        }
        else{
          error_index=user_argument;
          error_message=-1; //can't enter two different output parameters
          break;
        }
      }
      else if(argv[user_argument][1]=='p' && argv[user_argument][2]=='\0'){
        if(user_request=='n'||user_request=='p'){
          user_request='p'; //output paramater
        }
        else{
          error_index=user_argument;
          error_message=-1; //can't enter two different output parameters
          break;
        }
      }
      else if(argv[user_argument][1]=='t' && argv[user_argument][2]=='\0'){
        if(user_request=='n'||user_request=='t'){
          user_request='t'; //output paramater
        }
        else{
          error_index=user_argument;
          error_message=-1; //can't enter two different output parameters
          break;
        }
      }
      else if(argv[user_argument][1]=='l' && argv[user_argument][2]=='\0'){
        if((user_argument+1)<argc){
          return_value=get_number(argv[user_argument+1]);
          if(return_value<0){ //argument following -l is not an integer or positive
            error_index=user_argument;
            error_message=return_value;
            break;
          }
          else{
            user_output_rows=return_value; //number of output rows to be displayed
            user_argument++;
            row_value_index=user_argument; //storing the index at which the number of output rows were given.this is useful at the file handling stage
          }
        }
        else{ //no argument is present after -l
          error_index=user_argument;
          error_message=-3;
          break;
        }
      }
      else{ //argument entered following - is not compatible
        error_index=user_argument;
        error_message=-4;
        break;
      }
    }
    if(argv[user_argument][0]=='-'&& argv[user_argument][1]=='-'){
      return_value=strcmp(argv[user_argument],"--scaled");
      if(return_value==0){
        user_scale=1;//output rows should be scaled
      }
      else{//argument entered following -- is not compatible
        error_index=user_argument;
        error_message=-5;
        break;
      }
    }
  }
  if(argc==1){
    error_message=-6; //no files have been given
  }



/*Handling the errors found in the previous section*/
  if(error_message<0){
    if(error_message==-1){
      printf("Cannot plot multiple parameters in same graph.\n");
    }
    if(error_message==-2){
      printf("Invalid options for [-l]\n");
    }
    if(error_message==-3){
      printf("Not enough options for [-l]\n");
    }
    if(error_message==-4){
      printf("Invalid option [%s]\n",argv[error_index]);
    }
    if(error_message==-5){
      printf("Invalid option [%s]\n",argv[error_index]);
    }
    if(error_message==-6){
      printf("No input files were given\n");
    }
    if(error_message==-7){
      printf("Invalid option(negative) for [-l]\n");
    }
    printf("usage: %s [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..\n",argv[0]);

  }
  else{
    if(user_request=='n'){
      user_request='m'; //if the output parameter is not given by the user set it to default which is no of meetings
    }
  }



/*file handiling*/

  if(error_message==0){ //if there was no error in the arguments upto now continue to file handling

    /*first check if files contain errors. is so quit the program*/

    for(data_list_number=1;data_list_number<argc;++data_list_number){
      if(argv[data_list_number][0]=='-'||row_value_index==data_list_number){ //to escape parameter arguments given (-m,--scaled and so on) and the argument given after -l
        continue;
      }
      if(strstr(argv[data_list_number],".csv")==NULL){ //files other than .csv are given
        printf("Only .csv files should be given as inputs.\n");
        return 0;
      }
      FILE *fp=fopen(argv[data_list_number],"r"); // open file in reading mode

      if(fp==NULL){//csv files contain some errors
        printf("Cannot open one or more given files\n");
        return 0;
      }
    }

    /*now we know .csv files don't have errors,we can proceed ahead*/
    for(data_list_number=1;data_list_number<argc;++data_list_number){
      if(argv[data_list_number][0]=='-'||data_list_number==row_value_index){ //to escape parameter arguments given (-m,--scaled and so on) and the argument given after -l
        continue;
      }

      FILE *fp=fopen(argv[data_list_number],"r"); // open file in reading mode

      if(fp!=NULL){
        fseek (fp, 0, SEEK_END);
        size = ftell(fp);
        if (0 == size) {
          continue; //file contains no files
        }
        else{
          fseek (fp,0,SEEK_SET);
        }


        while(fgets(buff,100,fp)){

          intermediate=(data_t *)malloc(sizeof(data_t));

          field_count=0;
          char *field=strtok(buff,",");
          while (field!=NULL){

            if(field_count==0){ //retated to the name
              strcpy(intermediate->name,field);
              identifier=identity(intermediate->name);
            }
            if(field_count==1){ //related to the number of participants
              intermediate->participants=atoi(field);
            }
            if(field_count==2){ //related to meeting duration
              intermediate->duration=meeting_time(field);
            }
            field=strtok(NULL,",");
            field_count++;
          }
          if (identifier==NULL){ //this person is not in the current data
            intermediate->no_meetings=1;
            intermediate->next=inputs; //current data_t instance is linked with the address in inputs
            inputs=intermediate; //address of the current instance is stored in inputs
            elements++;//keeping track of the number of distincts elemets
            intermediate->rank=elements; //assigning a rank to the unique entry
          }
          else{ //this person is already recorded once.his or her data is updated
            identifier->no_meetings=identifier->no_meetings+1;
            identifier->participants=identifier->participants+intermediate->participants;
            identifier->duration=identifier->duration+intermediate->duration;
          }

        }
      }
      fclose(fp);
    }
    if(inputs==NULL){
      printf("No data to process\n");
      return 0;
    }

    /*variable*/
    data_t *top_elements[user_output_rows];//array of pointers to keep track of the top inputs

    if(user_request=='m'){
      sort_meetings(top_elements,user_output_rows);
    }
    if(user_request=='p'){
      sort_participants(top_elements,user_output_rows);
    }
    if(user_request=='t'){
      sort_time(top_elements,user_output_rows);
    }
    print_chart(user_request,user_output_rows,user_scale,top_elements); //printing the infomation in the relevant format

  }


  return 0;

}




//Function
//get the number of output rows requested by the user
//takes a pointer to a string which the user entered as the argument
int get_number(char *user_string){
  int i;//iterable
  int number;
  for(i=0;user_string[i]!='\0';++i){
    if((i==0)&&(user_string[0]=='-'||user_string[0]=='+')){
      continue;
    }
    if((user_string[i]<48)||(user_string[i]>57)) {
      return -2;
    }
  }
  number=atoi(user_string);
  if(number<0){
    return -7;
  }
  else{
    return number;

  }
}





//FUNCTION
//this function will check whether a name is already in the user data provided.
//if it is the address of that data_t variable will be retuned
//if it is not found NULL will be retuned

data_t *identity(char *name){

  data_t *intermediate;// this is used to temporarily store the adress of the structure
  for(intermediate=inputs;intermediate!=NULL;intermediate=intermediate->next){
    if(strcmp(name,intermediate->name)==0){
      return intermediate; // name found,relevant address returned
      break;
    }
  }
  return NULL;//name not found,NULL retuned
}


//FUNCTION
//this functions the duration of a meeting in minutes

int meeting_time(char *time_string){
  int field_count=0;
  int time_minutes=0;
  char *field=strtok(time_string,":");
  while(field!=NULL){
    if(field_count==0){
      time_minutes=time_minutes+atoi(field)*60; //converting hours into minutes and storing in time
    }
    if(field_count==1){
      time_minutes=time_minutes+atoi(field); //storing minutes in time
    }
    field_count++;
    field=strtok(NULL,",");
  }
  return time_minutes;
}


//FUNCTION
//this functions populates the top_elements array with pointers pointing
//at highest meeting number elements (from highest to lowest)
//it takes as arguments data_t *top_elements
//it takes as arguments user_request_output_rows
//global variable elements which kept track of distinct elements is used
//global variable data_t *inputs;  inputs is a reference to data_t is used

void sort_meetings(data_t **top_elements,int user_output_rows){
  int number;
  int current_position;
  data_t *intermediate;
  number=user_output_rows;

  intermediate=inputs;
  for(current_position=0;current_position<number;++current_position){ //first (number) pointers are added to top_elements
    top_elements[current_position]=intermediate;
    intermediate=intermediate->next;

  }
  sort_top_elements(number,'m',top_elements);
  if(elements>number){
    for(;current_position<elements;++current_position){
      if(top_elements[number-1]->no_meetings <=intermediate->no_meetings){
        top_elements[number-1]=intermediate;
        sort_top_elements(number,'m',top_elements);
      }
      intermediate=intermediate->next;
    }
  }

}






//FUNCTION
//this functions populates the top_elements array with pointers pointing
//at highest participants number elements (from highest to lowest)
//it takes as arguments data_t *top_elements
//it takes as arguments user_request_output_rows
//global variable elements which kept track of distinct elements is used
//global variable data_t *inputs;  inputs is a reference to data_t is used

void sort_participants(data_t **top_elements,int user_output_rows){
  int number;
  int current_position;
  data_t *intermediate;
  number=user_output_rows;

  intermediate=inputs;
  for(current_position=0;current_position<number;++current_position){ //first (number) pointers are added to top_elements
    top_elements[current_position]=intermediate;
    intermediate=intermediate->next;

  }
  sort_top_elements(number,'p',top_elements);
  if(elements>number){
    for(;current_position<elements;++current_position){
      if(top_elements[number-1]->participants<=intermediate->participants){
        top_elements[number-1]=intermediate;
        sort_top_elements(number,'p',top_elements);
      }
      intermediate=intermediate->next;
    }
  }

}






//FUNCTION
//this functions populates the top_elements array with pointers pointing
//at highest participants number elements (from highest to lowest)
//it takes as arguments data_t *top_elements
//it takes as arguments user_request_output_rows
//global variable elements which kept track of distinct elements is used
//global variable data_t *inputs;  inputs is a reference to data_t is used

void sort_time(data_t **top_elements,int user_output_rows){
  int number;
  int current_position;
  data_t *intermediate;
  number=user_output_rows;

  intermediate=inputs;
  for(current_position=0;current_position<number;++current_position){ //first (number) pointers are added to top_elements
    top_elements[current_position]=intermediate;
    intermediate=intermediate->next;

  }
  sort_top_elements(number,'t',top_elements);
  if(elements>number){
    for(;current_position<elements;++current_position){
      if(top_elements[number-1]->duration<=intermediate->duration){
        top_elements[number-1]=intermediate;
        sort_top_elements(number,'t',top_elements);
      }
      intermediate=intermediate->next;
    }
  }

}


//FUNCTION
//this function will sort the array top_elements based on a given parameter
// in decending order and populate top_element with the pointers
//it takes paramater to quantify as an argument
//it also takes top_elements pointer and number which is the number of elements in the top_elemets array as argument s

void sort_top_elements(int number,char parameter,data_t **top_elements){
  int sorting_element;
  int maximum_position;
  data_t *holding_position;
  int i;//iterable


  if(parameter=='m'){
    for(sorting_element=0;sorting_element<number-1;++sorting_element){
      maximum_position=sorting_element; //highest element preceding the sorting_element will be stored here
      for(i=sorting_element+1;i<number;++i){
        if(top_elements[i]->no_meetings >=top_elements[maximum_position]->no_meetings){
          maximum_position=i;
        }
      }

      if(top_elements[maximum_position]->no_meetings==top_elements[sorting_element]->no_meetings){ //if two elements are equal on the given paramater,lowest rank will get the maximum position
        maximum_position=sorting_element; //lowest rank element preceding the sorting_element will be stored here
        for(i=sorting_element+1;i<number;i++){
          if((top_elements[i]->rank<top_elements[maximum_position]->rank)&&(top_elements[maximum_position]->no_meetings==top_elements[i]->no_meetings)){
            maximum_position=i;
          }
        }
      }

      holding_position=top_elements[sorting_element];
      top_elements[sorting_element]=top_elements[maximum_position];
      top_elements[maximum_position]=holding_position;
    }

  }
  else if(parameter=='p'){
    for(sorting_element=0;sorting_element<number-1;++sorting_element){
      maximum_position=sorting_element; //highst element preceding the sorting_element will be noted here
      for(i=sorting_element+1;i<number;++i){
        if(top_elements[i]->participants >=top_elements[maximum_position]->participants){
          maximum_position=i;
        }
      }

      if(top_elements[maximum_position]->participants==top_elements[sorting_element]->participants){ //if two elements are equal on the given paramater,lowest rank will get the maximum position
        maximum_position=sorting_element; //lowest rank element preceding the sorting_element will be stored here
        for(i=sorting_element+1;i<number;i++){
          if((top_elements[i]->rank<top_elements[maximum_position]->rank)&&(top_elements[maximum_position]->participants==top_elements[i]->participants)){
            maximum_position=i;
          }
        }
      }

      holding_position=top_elements[sorting_element];
      top_elements[sorting_element]=top_elements[maximum_position];
      top_elements[maximum_position]=holding_position;
    }
  }
  else{
    for(sorting_element=0;sorting_element<number-1;++sorting_element){
      maximum_position=sorting_element; //highst element preceding the sorting_element will be noted here
      for(i=sorting_element+1;i<number;++i){
        if(top_elements[i]->duration >=top_elements[maximum_position]->duration){
          maximum_position=i;
        }
      }

      if(top_elements[maximum_position]->duration==top_elements[sorting_element]->duration){ //if two elements are equal on the given paramater,lowest rank will get the maximum position
        maximum_position=sorting_element; //lowest rank element preceding the sorting_element will be stored here
        for(i=sorting_element+1;i<number;i++){
          if((top_elements[i]->rank<top_elements[maximum_position]->rank)&&(top_elements[maximum_position]->duration==top_elements[i]->duration)){
            maximum_position=i;
          }
        }
      }

      holding_position=top_elements[sorting_element];
      top_elements[sorting_element]=top_elements[maximum_position];
      top_elements[maximum_position]=holding_position;
    }

  }



}


//FUNCTION
// this function will print the graph as given in the sample programme
//the following will be taken as arguments
//requested parameter to be printed by the user 'm'-no_of_meetings  'p'-number of participants 't'-duration
//requested number of output rows by the user
//whether user wants scaled rows or not.0 means don't scale, 1 means scale
//data_t **top_elements will also be taken
//global variavles *inputs and elements will also be used


void print_chart(char user_request,int user_output_rows,int user_scale,data_t **top_elements){

  data_t *intermediate;//this will be used to go through the elements
  int max_name_length;// length of the longest name in top_elements
  int i;//iterable
  int bar_printing_area; //length of the maximum printing area for bar for an element
  int bar_printing_total_area;// printing area including bar and digit length
  int bar_length;//bar length for individual element
  int total_element_sum=0;//sum of all the element of the user requested parameter
  int j;//iterable used to print the pattern in the given format
  int cursor_position;//where the cursor is currently printing
  int divider;
  int digits;//calculate number of digits in a integer;

  max_name_length=strlen(top_elements[0]->name);

  for(i=0;i<user_output_rows;++i){
    if(strlen(top_elements[i]->name)>max_name_length){
      max_name_length=strlen(top_elements[i]->name);
    }
  }

  bar_printing_total_area=80-max_name_length-3;

  if(user_request=='m'){
    for(intermediate=inputs;intermediate!=NULL;intermediate=intermediate->next){
      total_element_sum=total_element_sum+(intermediate->no_meetings);
    }

    if(user_scale==1){ //print scaled version
      divider=top_elements[0]->no_meetings;
    }
    else{ //print non scaled version
      divider=total_element_sum;
    }
    printf("\n");
    for(i=0;i<user_output_rows;++i){ //printing each element
      digits=funDigits(top_elements[i]->no_meetings);
      bar_printing_area=bar_printing_total_area-digits;
      bar_length=(((float)top_elements[i]->no_meetings/divider)*bar_printing_area);
      for(j=0;j<3;++j){

        if(j==0){
          for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("\n");
        }

        if(j==1){
          printf(" ");//printing spaces
          printf("%s",top_elements[i]->name);
          for(cursor_position=strlen(top_elements[i]->name)+1;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("%i",top_elements[i]->no_meetings);
          printf("\n");
        }

        if(j==2){
          for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("\n");
        }
      }
      for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
        printf(" ");//printing spaces
      }
      printf("\u2502");
      printf("\n");

    }

  }

  if(user_request=='p'){
    for(intermediate=inputs;intermediate!=NULL;intermediate=intermediate->next){
      total_element_sum=total_element_sum+(intermediate->participants);
    }

    if(user_scale==1){ //print scaled version
      divider=top_elements[0]->participants;
    }
    else{ //print non scaled version
      divider=total_element_sum;
    }
    printf("\n");
    for(i=0;i<user_output_rows;++i){ //printing each element
      digits=funDigits(top_elements[i]->participants);
      bar_printing_area=bar_printing_total_area-digits;
      bar_length=(((float)top_elements[i]->participants/divider)*bar_printing_area);
      for(j=0;j<3;++j){

        if(j==0){
          for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("\n");
        }

        if(j==1){
          printf(" ");//printing spaces
          printf("%s",top_elements[i]->name);
          for(cursor_position=strlen(top_elements[i]->name)+1;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("%i",top_elements[i]->participants);
          printf("\n");
        }

        if(j==2){
          for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("\n");
        }
      }
      for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
        printf(" ");//printing spaces
      }
      printf("\u2502");
      printf("\n");

    }

  }

  if(user_request=='t'){
    for(intermediate=inputs;intermediate!=NULL;intermediate=intermediate->next){
      total_element_sum=total_element_sum+(intermediate->duration);
    }

    if(user_scale==1){ //print scaled version
      divider=top_elements[0]->duration;
    }
    else{ //print non scaled version
      divider=total_element_sum;
    }
    printf("\n");
    for(i=0;i<user_output_rows;++i){ //printing each element
      digits=funDigits(top_elements[i]->duration);
      bar_printing_area=bar_printing_total_area-digits;
      bar_length=(((float)top_elements[i]->duration/divider)*bar_printing_area);
      for(j=0;j<3;++j){

        if(j==0){
          for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("\n");
        }

        if(j==1){
          printf(" ");//printing spaces
          printf("%s",top_elements[i]->name);
          for(cursor_position=strlen(top_elements[i]->name)+1;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("%i",top_elements[i]->duration);
          printf("\n");
        }

        if(j==2){
          for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
            printf(" ");//printing spaces
          }
          printf("\u2502");
          for(cursor_position=0;cursor_position<bar_length;++cursor_position){
            printf("\u2591");
          }
          printf("\n");
        }
      }
      for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
        printf(" ");//printing spaces
      }
      printf("\u2502");
      printf("\n");

    }

  }
  for(cursor_position=0;cursor_position<(max_name_length+2);++cursor_position){
    printf(" ");//printing spaces
  }
  printf("\u2514");
  for(cursor_position=cursor_position+1;cursor_position<80;++cursor_position){
    printf("\u2500");
  }
  printf("\n");

}

//function to calculate the number of digits in a integer

int funDigits(int number){
  int elements=0; //number of digits in the integer
  if(number>0){
    elements=1;
  }
  else{
    elements=0;
    return elements;
  }
  while(number!=0){
    number=number/10;
    if(number>0){
      elements++;
    }
  }

  return elements;
}
