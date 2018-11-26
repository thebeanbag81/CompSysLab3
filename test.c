#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

void main(){
	char c[] = "lw $s0, 8($t0)";
	

        char delimiters[] = {',',' ','\n','\r'}; //delimiter array
        char **token;
        token=(char**)malloc(10*sizeof(char*));        
        char formStr[20];
        
        //finds opcode
        int i = 0;
	token[0]=(char*)malloc(256*sizeof(char));
        token[i] = strtok(c,delimiters);
        

        //checks too see if there is an opcode
        if(token[0] == "") return formStr;
        
        //finds rest of instruction
        while(token[i]!=NULL){
            token[++i]=(char*)malloc(256*sizeof(char));   
	    token[i]=strtok(NULL,delimiters);
            
        }
        //if there are more than 4 parts of the instruction it is invalid
        if(i>4)return formStr;

        //checks to see if opcode is lw or sw
        //then checks to see if parentheses are correct
        //if they are, removes them from character array and returns 
        //if not returns ERROR
        if((strcmp(token[0],"lw")==0) || (strcmp(token[0],"sw")==0)){
           char* paren = token[2];
           int j = 0;
           int leftBracket = 0;
           int rightBracket = 0;
           char* foundL;
           char* foundR;
           while(paren[j]!=NULL){
	    if(paren[j]=='(') leftBracket++;
	    if(paren[j]==')') rightBracket++;
		j++;
           }

           if(leftBracket!=rightBracket) {return NULL;} 
           else{
               int k = 0;
               while(paren[k]!=NULL){
                   if((paren[k]==')') || (paren[k]=='(')) {paren[k]=' ';}
		   k++;
               }
           }
           

        }

        //pull string together
/*
        int l = 0;
        while(l<i*2){
        formStr[l] = &token[l];
        formStr[++l]=" ";
        l++;
        }
*/

	int l=0;
        while(token[l]!=NULL){
	strcat(formStr,token[l]);
	strcat(formStr," ");
	l++;
	}
	printf("%s",formStr);
        free(token);
	putchar('\n');
	}
