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
data_t *inputs; // inputs is a reference to data_t
int elements=0;//keep track of the distinct elements




//defined functions
data_t *identity(char *); //this function will check whether a name is already in the user data provided.
int meeting_time(char *); //this functions the duration of a meeting in minutes
void sort_meetings(data_t **,int); //this functions populates an array named top_elements (this array is defined in the middle of the main function) with pointers pointing at highest meeting number elements
void sort_participants(data_t **,int);//this functions populates the top_elements array with pointers pointing at highest participant number elements
void sort_time(data_t **,int); //this functions populates the top_elements array with pointers pointing at longest time duration elements
void sort_top_elements(int,char,data_t **); //this function will sort the array top_elements based on a given parameter in decending order and populate top_element with the pointers



int main(int argc,char **argv){

  data_t *intermediate;// this is used to temporarily store the adress of the structure created by reading a line
  data_t *identifier;// this is used to identify uf a certain person is already in data_t
  char buff[100];//stores the first 100 lines into buff
  int field_count;
  int data_list_number=0; // this is to keep track of the number of user entered data lists
  char user_request; //requested parameter to be printed by the user 'm'-no_of_meetings  'p'-number of participants 't'-duration
  int user_output_rows=11; //requested number of output rows by the user

  user_request='p'; //delete this

  for(data_list_number=1;data_list_number<argc;++data_list_number){

    FILE *fp=fopen(argv[data_list_number],"r"); // open file in reading mode

    if(fp!=NULL){

      if(data_list_number==1){ //this is the first file
        inputs=NULL;
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




  for(int i=0;i<user_output_rows;++i){
    printf("%s ",top_elements[i]->name);
    //printf("%i ",top_elements[i]->no_meetings);

    printf("%i ",top_elements[i]->participants);
    //printf("%i",top_elements[i]->duration);
    printf("\n");
  }
  return 0;


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
  int least_rank;//this is used if two elements are equal

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
