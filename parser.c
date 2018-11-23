#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


void main(void){

char array[] = "add 16 17 18"; 
char delimiters[] = ' ';

char formArray[] = progScanner(array);

char* op = token[0];
char rs = token[1];
char rt = token[2];

struct inst instr;

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
		inst.opcode=mult
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
	   inst.opcode=mult
		}
	}
else return NULL;
}
break;
case 'l':
{
if(op[1]=='w'){
  inst.opcode=mult
	}
else return NULL;
}
break;
}


if(inst.opcode == lw || inst.opcode == sw){
instr.rt = regNumberConverter(token[1]);
instr.immediate = atoi(token[2]);
instr.rs = regNumberConverter(token[3]);

}
else if(inst.opcode == beq){
instr.rs = regNumberConverter(token[1]);
instr.rt = regNumberConverter(token[2]);
instr.immediate = atoi(token[3]);
}
else if(inst.opcode == addi){
instr.rt = regNumberConverter(token[1]);
instr.rs = regNumberConverter(token[2]);
instr.immediate = atoi(token[3]);
}
else{
instr.rd = regNumberConverter(token[1]);
instr.rs = regNumberConverter(token[2]);
instr.rt = regNumberConverter(token[3]); 

}

return instr;

}
