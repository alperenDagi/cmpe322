#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFFER_LEN 1024
#define MAX_HISTORY 15
int history_index = 0; 
char history[15][BUFFER_LEN];
char* changeResult(char* token){ //returns yes if history contains token, no if not
    for(int i = 0; i< history_index % 15 ;i++){
        if(strcmp(history[i], token)==0){
            return "Yes";
            }
        }
    return "No";
}
void customExecute(char* argument_list[]){ //creates a process and executes arguments list
    pid_t childPid;
    childPid = fork();
    if(childPid < 0){
        return;
        }
    if(childPid == 0){
        execvp(argument_list[0], argument_list);
        return;
        }
    else{
        wait(NULL);
        return;
        }
}
void historyUpdate(char input_copy[BUFFER_LEN]){ //updates history after command
    char *addToHistory;
    addToHistory = (char*)calloc(10,100*sizeof(char));
    strcpy(addToHistory,"\"");
    strcat(addToHistory,input_copy);
    strcat(addToHistory,"\""); //put quotas before and after
    strcpy(history[history_index % 15], addToHistory);
    history_index++; //increments history index
    free(addToHistory);
    return;
}

int main (){
    char * username = getenv("USER");
    char input[BUFFER_LEN];
	while(1){	
        char *buffer, *tokens[4], *token;
        char input_copy[BUFFER_LEN];
        int count = 0;
        printf("%s >>> ", username); //prints username

        if(!fgets(input,BUFFER_LEN, stdin)){
            break;  
        }
        int len = strlen(input);
        if((len != 1)&&(input[len-1] == '\n')){ // removes /n if input is empty
            input[len-1] = 0;
        }
        strcpy(input_copy,input); // takes input copy to use after
        token = strtok(input, " "); // tokenize input with strtok() using delimiter " "
         while(token != NULL){
            tokens[count] = token;
            count++;
            token = strtok(NULL, " "); //continues tokenizing from rest of the input
        }
        if(strcmp(tokens[0],"exit") == 0){ // if exit breaks the while loop
            break;
        }
        if(strcmp(tokens[0],"listdir") == 0){ // calls system with ls
            system("ls");
            historyUpdate(input_copy);
        }
        else if(strcmp(tokens[0],"mycomputername") == 0){ // calls system with hostname
            system("hostname");
            historyUpdate(input_copy);

        }
        else if(strcmp(tokens[0],"whatsmyip") == 0){ // calls system with hostname -I
            system("hostname -I");
            historyUpdate(input_copy);

        }
        else if(strcmp(tokens[0],"hellotext") == 0){ // calls system with editor
            system("editor");
            historyUpdate(input_copy);

        }
        else if(strcmp(tokens[0],"dididothat") == 0){ //checks history and calls custom execute according to the result
            char* result;
            result = (char*)malloc(100*sizeof(char));
            char *temp;
            temp = (char*)malloc(100*sizeof(char));
            strcpy(temp, input_copy+11); //copies input copy from +11 address which is rest of input after dididothat
            strcpy(result,changeResult(temp)); //copies output of changeResult to result
            char* argument_list[] = {"echo",result, NULL}; //call custom execute with argument list
            customExecute(argument_list);
            free(result);
        }
        else if((strcmp(tokens[0],"printfile") == 0)){
            if(strcmp(tokens[2],">") == 0){ // if token[2] is > than 
                strcpy(tokens[2]," ");
                char* argument_list[] = {"cp",tokens[1],tokens[3],NULL}; // calls custom execute with cp and 2 files
                customExecute(argument_list);
                historyUpdate(input_copy);
            }else{
                char* argument_list[] = {"more","-1",tokens[1],NULL}; //calls custom execute with more -1 so it reads file line by line 
                customExecute(argument_list);
                historyUpdate(input_copy);
            }
           
        }
        };
     return 0;
    }

