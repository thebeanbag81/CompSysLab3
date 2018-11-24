#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>


int main(void){
	
	int reg;
	char prog[] = "$ra"; //16
	char delim[] = {'$',' '}; //splits takes input from $ to space
        char *inst = strtok(prog,delim);
	reg = -1;


        if(prog==NULL){
            //ERROR
            return NULL;
        }
        else if((prog[0]!='$') || (strlen(inst)>5)){
            //ERROR
            return NULL;
        }

        else if(isdigit(inst[0])){     
            reg = atoi(inst);
            return reg;
        }
        else{

        switch(inst[0]) {
            
	    case 'z':
            {reg=0;}
            break;
            case 'g': 
            {reg=28;}
            break;
            case 'f': 
            {reg=30;}
            break;
            case 'r': 
            {reg=31;}
            break;
	    
            case 'v':
		switch(inst[1]){
		case '0':
		{reg=2;}
		break;
		case '1':
		{reg=3;}
		break;
		} 
            case 'k':
		switch(inst[1]){
		case '0':
		{reg=26;}
		break;
		case '1':
		{reg=27;}
		break;
		}  

            case 's':
                switch(inst[1]){
                    case 'p':
		    {reg=29;}
		    break;
		    case '0':
                    {reg=16;} 
                    break;
                    case '1':
                    {reg=17;}
                    break;
                    case '2': 
                    {reg=18;}
                    break;
                    case '3': 
                    {reg=19;}
                    break;
                    case '4': 
                    {reg=20;}
                    break;
                    case '5': 
                    {reg=21;}
                    break;
                    case '6': 
                    {reg=22;}
                    break;
                    case '7':
                    {reg=23;}
                    break;
                
		}
		break;
            
            case 'a':
                switch(inst[1]){
                    case 't':
		    {reg=1;}
		    break;
		    case '0':
                    {reg=4;}
                    break;
                    case '1': 
                    {reg=5;}
                    break;
                    case '2':
                    {reg=6;}
                    break;
                    case '3':
                    {reg=7;}
                    break;
                
		}
		break;
            
            case 't':
                 switch(inst[1]){
                    case '0':
                    {reg=8;} 
                    break;
                    case '1':
                    {reg=9;}
                    break;
                    case '2': 
                    {reg=10;}
                    break;
                    case '3': 
                    {reg=11;}
                    break;
                    case '4': 
                    {reg=12;}
                    break;
                    case '5': 
                    {reg=13;}
                    break;
                    case '6': 
                    {reg=14;}
                    break;
                    case '7':
                    {reg=15;}
                    break;
                    case '8':
                    {reg=24;}
                    break;
                    case '9':
                    {reg=25;}
                    break;
                
		}
		break;
        }

        }

        if(reg==-1){
            return NULL;
        }
        else{
            printf("%d\n",reg);
	    return reg;
        }

}
