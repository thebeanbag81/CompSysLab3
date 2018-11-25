#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>




void main(void){


enum opcode {add,addi,sub,mult,beq,lw,sw,haltSim};

struct instr{
		//rtype example
		//add $s1 $s2 $s2 adds contents of register $s1 to $s2

		//itype example
		//lw $t1 4($s0) 
		//addi $a0 $t2 33 

		enum opcode opcode; //what function
		int rs; //source data 
		int rt; //source data
		int rd; //destination for result
		int func; //function 
		int immediate; //
};


int x;
int y;
int z;
int a;

char array[] = "addi $s0 $s1 8"; 

//char formArray[] = progScanner(array);

char delimiters[] = {',',' ','\n','\r'}; //delimiter array
        char **token;
        token=(char**)malloc(10*sizeof(char*));        
        char formStr[20];
        
        //finds opcode
        int i = 0;
	token[0]=(char*)malloc(256*sizeof(char));
        token[i] = strtok(array,delimiters);
        

        //checks too see if there is an opcode
        if(token[0] == "") return formStr;
        
        //finds rest of instruction
        while(token[i]!=NULL){
            token[++i]=(char*)malloc(256*sizeof(char));   
	    token[i]=strtok(NULL,delimiters);
            
        }


char* op = token[0];
char* rs = token[1];
char* rt = token[2];

struct instr inst;

switch(op[0]){

case 'a':
{
if(op[1]=='d'){
  	if(op[2]=='d'){
	   if(op[3]=='i'){
		inst.opcode=addi;}
		else inst.opcode=add;
			
		}
	}
else return NULL;
}
break;
case 's':
{
if(op[1]=='u'){
  	if(op[2]=='b'){
	   inst.opcode=sub;
		}
	}
else if(op[1]=='w'){
   inst.opcode=sw;
}
else return NULL;
}
break;
case 'm':
{
if(op[1]=='u'){
  	if(op[2]=='l'){
	   if(op[3]=='t'){
		inst.opcode=mult;
			}
		}
	}
else return NULL;
}
break;
case 'b':
{
if(op[1]=='e'){
  	if(op[2]=='q'){
	   inst.opcode=mult;
		}
	}
else return NULL;
}
break;
case 'l':
{
if(op[1]=='w'){
  inst.opcode=mult;
	}
else return NULL;
}
break;
}


if(inst.opcode == lw || inst.opcode == sw){
//instr.rt = regNumberConverter(token[1]);
inst.immediate = atoi(token[2]);
//instr.rs = regNumberConverter(token[3]);
x++;
}
else if(inst.opcode == beq){
//instr.rs = regNumberConverter(token[1]);
//instr.rt = regNumberConverter(token[2]);
inst.immediate = atoi(token[3]);
y++;
}
else if(inst.opcode == addi){
//instr.rt = regNumberConverter(token[1]);
//instr.rs = regNumberConverter(token[2]);
inst.immediate = atoi(token[3]);
z++;
}
else{
//instr.rd = regNumberConverter(token[1]);
//instr.rs = regNumberConverter(token[2]);
//instr.rt = regNumberConverter(token[3]); 
a++;
}

//return instr;


}
