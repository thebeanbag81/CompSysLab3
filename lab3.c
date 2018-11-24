// List the full names of ALL group members at the top of your code.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <ctype.h>
//feel free to add here any additional library names you may need 
#define SINGLE 1
#define BATCH 0
#define REG_NUM 32
main (int argc, char *argv[]){
	int sim_mode=0;//mode flag, 1 for single-cycle, 0 for batch
	int c,m,n;
	int i;//for loop counter
	long mips_reg[REG_NUM];
	long pgm_c=0;//program counter
	long sim_cycle=0;//simulation cycle counter
	//define your own counter for the usage of each pipeline stage here
	/***************************************/
	//counters
	int IF_counter = 0;
	int ID_counter = 0;
	int EX_counter = 0;
	int MEM_counter = 0;
	int WB_counter = 0;
	int clock_counter = 0;

    /**************************************/

	//opcode
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
	}

	struct instr InstructionMem[512]; //long = 32 bits = 4 bytes = 1 word, IM = 512 words, 2k bytes
	long DataMem[512]; //long = 32 bits = 4 bytes = 1 word, IM = 512 words, 2k bytes

	struct latch{
		struct instr instruction;
		int data;
		int read;
		int write;
	}
	
	struct latch IFID;
	struct latch IDEX;
	struct latch EXMEM;
	struct latch MEMWB;

	void latchinit(void){
		IFID.write = 1;
		IFID.read = 0
		IFID.data = 0;

		IDEX.write = 1;
		IDEX.read = 0
		IDEX.data = 0;

		EXMEM.write = 1;
		EXMEM.read = 0
		EXMEM.data = 0;

		MEMWB.write = 1;
		MEMWB.read = 0
		MEMWB.data = 0;
	}



	
	int test_counter=0;
	FILE *input=NULL;
	FILE *output=NULL;
	printf("The arguments are:");
	
	for(i=1;i<argc;i++){
		printf("%s ",argv[i]);
	}
	printf("\n");
	if(argc==7){
		if(strcmp("-s",argv[1])==0){
			sim_mode=SINGLE;
		}
		else if(strcmp("-b",argv[1])==0){
			sim_mode=BATCH;
		}
		else{
			printf("Wrong sim mode chosen\n");
			exit(0);
		}
		
		m=atoi(argv[2]);
		n=atoi(argv[3]);
		c=atoi(argv[4]);
		input=fopen(argv[5],"r");
		output=fopen(argv[6],"w");
		
	}
	
	else{
		printf("Usage: ./sim-mips -s m n c input_name output_name (single-sysle mode)\n or \n ./sim-mips -b m n c input_name  output_name(batch mode)\n");
		printf("m,n,c stand for number of cycles needed by multiplication, other operation, and memory access, respectively\n");
		exit(0);
	}
	if(input==NULL){
		printf("Unable to open input or output file\n");
		exit(0);
	}
	if(output==NULL){
		printf("Cannot create output file\n");
		exit(0);
	}
	//initialize registers and program counter
	if(sim_mode==1){
		for (i=0;i<REG_NUM;i++){
			mips_reg[i]=0;
		}
	}
	
	//start your code from here
    
    //reads file into sim   FGETS()
    /************************************/
    void fgets(void){
     char *traceEntry1;
    FILE *ifp;

    traceEntry1 = malloc(200*sizeof(char));
    ifp = fopen("./program.txt", "r");

    while (fgets(traceEntry1, 75, ifp)) 
      printf("String input is %s \n", traceEntry1);
    fclose(ifp);
    }
    /***********************************/
     

    //idk what this does but it was on the course website  STRTOK()
    /*********************************************************************************/
    void strtok(void){
      int i;
    char delimiters[]=",";  // here, we chose a comma as the only delimiter.
                            // you can expand this string with other delimiters
                            //     as well.
    char ** instructionFields;
    char inputString[] = "this is string 1, this is string 2,,,this is string 3";

    instructionFields = (char **)malloc(100*sizeof(char *));
    for (i=0; i<3; i++)
       *(instructionFields+i) = (char *) malloc(20*sizeof(char *));

    printf("inputString[]=%s\n", inputString);
    instructionFields[0] = strtok(inputString, delimiters);
    printf("inputString[]=%s\n", inputString);
    instructionFields[1] = strtok(NULL, delimiters);
    printf("inputString[]=%s\n", inputString);
    instructionFields[2] = strtok(NULL, delimiters);
    printf("inputString[]=%s\n", inputString);
    printf("%s \n%s \n%s \n", 
           instructionFields[0], instructionFields[1], instructionFields[2]);
    }
    /********************************************************************************/

	/********************************************************************************/
	
	//add the following code to the end of the simulation, 
        //to output statistics in batch mode
	if(sim_mode==0){
		fprintf(output,"program name: %s\n",argv[5]);
		fprintf(output,"stage utilization: %f  %f  %f  %f  %f \n",
                             ifUtil, idUtil, exUtil, memUtil, wbUtil);
                     // add the (double) stage_counter/sim_cycle for each 
                     // stage following sequence IF ID EX MEM WB
		
		fprintf(output,"register values ");
		for (i=1;i<REG_NUM;i++){
			fprintf(output,"%d  ",mips_reg[i]);
		}
		fprintf(output,"%d\n",pgm_c);
	
	}
	//close input and output files at the end of the simulation
	fclose(input);
	fclose(output);
	return 0;
}
  /********************************************************************************/

}

	/*********************************************/
	//progScanner
	//reads from file provided by user, containing mips assembly
    //has to be flexible with empty spaces
    //writes what was read into the array representing IM 
    //turns add    $s0, $s1, $s2 into add $s0 $s1 $s2 
    //lw $s0, 8($t0) --> lw $s0 8 $t0 
	/*********************************************/
	char* progScanner(char *c){
    
      // char c[] = "lw $s0, 8($t0)";
	
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

	int l=0;
        while(token[l]!=NULL){
	    strcat(formStr,token[l]);
	    strcat(formStr," ");
	    l++;
	    }
        free(token);
	putchar('\n');
    return formStr;
	}


    //accepts output of progScanner
    //when it sees '$' it strips the dollar sign 
    //then if the register it represented as a number like 5
    //it leaves the 5 as is, if it is something like $t0 then 
    //it turns t0 into the corresponding number(8)
	int regNumberConverter(char *prog){
    int reg;
	//char prog[] = "$ra"; //16
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
           // printf("%d\n",reg);
	    return reg;
        }
	}

	/******************************************************************/
	//parser
	//from the instruction passed to it by regNumberConverter()
    //break it up into its constituent fields
	//(opcode, source registers, immediate field, destination register)
    //detect errors in program 
        //illegal opcode
        //illegal register name
        //reg number out of bounds 
        //immediate field too large
    /******************************************************************/
	struct instruction parser(char *input){
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

	int IF(){

	}

	int ID(){

	}

	int EX(){

	}

	int MEM(){

	}

	int WB(){

	}

	//code from https://en.wikibooks.org/wiki/C_Programming/string.h/
	int my_strcmp (const char * s1, const char * s2){
    for(; *s1 == *s2; ++s1, ++s2)
        if(*s1 == 0)
            return 0;
    return *(unsigned char *)s1 < *(unsigned char *)s2 ? -1 : 1;
	}

	char *my_strcat(char *dest, const char *src){
    size_t i,j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i+j] = src[j];
    dest[i+j] = '\0';
    return dest;
	}





