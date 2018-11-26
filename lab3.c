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

	//opcode
	//struct instr InstructionMem[512]; //long = 32 bits = 4 bytes = 1 word, IM = 512 words, 2k bytes
	long DataMem[512]; //long = 32 bits = 4 bytes = 1 word, IM = 512 words, 2k bytes


enum opcode {add,addi,sub,mul,beq,lw,sw,haltSim};

int registers[32];

void initReg(void){
    registers[0]=0;
}

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
    int immediate; //
};

struct latch{
    struct instr instruction;
    int data;
    int read;
    int write;
};

struct instr *InstructionMem;

struct latch IFID;
struct latch IDEX;
struct latch EXMEM;
struct latch MEMWB;

void latchinit(void){
    IFID.write = 1;
    IFID.read = 0;
    IFID.data = 0;

    IDEX.write = 1;
    IDEX.read = 0;
    IDEX.data = 0;

    EXMEM.write = 1;
    EXMEM.read = 0;
    EXMEM.data = 0;

    MEMWB.write = 1;
    MEMWB.read = 0;
    MEMWB.data = 0;
}

struct instr parser(char *input);

main (int argc, char *argv[]){
	
	latchinit();

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
   
   /* to count number of lines in the file */
    char c;
    int count;

    for (c = getc(input); c != EOF; c = getc(input)) {
        if (c == '\n'){ // Increment count if this character is newline 
            count = count + 1;} 
    }
	/********************************************************************************/
    InstructionMem = (struct inst*)malloc(count*sizeof(struct instr));
    char *lines;
    lines=malloc(250*sizeof(char));
    rewind(input);

    int i = 0;
    while(fgets(lines,250,input)){
        InstructionMem[i]=parser(lines);
        i++;
    }

	//add the following code to the end of the simulation, 
        //to output statistics in batch mode

	double ifUtil, idUtil, exUtil, memUtil, wbUtil;		
	
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

  /********************************************************************************/

}

    //to check if the character string is a number within the allowed range
    int isImmOperand(char* s){
        long immediate = (int) *s;
        if((immediate>=-32768) && (immediate <= 32767)){ //if it's a valid immediate number
            return immediate; //return it back
        }
        else{ //if it isn't
            return -1;
        }
    }

    //to check if the number(register) format is followed for a load or store instruction.
    int memAccessFormat(char *s){

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
        if(token[0] == "") printf("Illegal Opcode detected"); exit(0);
        
        //finds rest of instruction
        while(token[i]!=NULL){
            token[++i]=(char*)malloc(256*sizeof(char));   
	    token[i]=strtok(NULL,delimiters);
            
        }
        //if there are more than 4 parts of the instruction it is invalid
        if(i>4) printf("Incorrect Instruction format"); exit(0);

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

           if(leftBracket!=rightBracket) {printf("Mismatched or Missing Parenthesis"); exit(0);} 
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
            printf("Illegal Register Name"); exit(0);
        }
        else if((prog[0]!='$') || (strlen(inst)>5)){
            //ERROR
            printf("Missing $"); exit(0);
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
        printf("Illegal Register Name"); exit(0);
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
  struct instr parser(char *input){
    
         //char array[] = "addi $s0 $s1 8"; 
        char *formArray;
        formArray = progScanner(input);

        char delimiters[] = {' '}; //delimiter array
        char **token;
        token=(char**)malloc(10*sizeof(char*));        
        
        int i = 0;
	    token[0]=(char*)malloc(256*sizeof(char));
        token[i] = strtok(formArray,delimiters);
    
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
else printf("Illegal Opcode"); exit(0);
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
else printf("Illegal Opcode"); exit(0);
}
break;
case 'm':
{
if(op[1]=='u'){
  	if(op[2]=='l'){
		inst.opcode=mul;	
		}
	}
else printf("Illegal Opcode"); exit(0);
}
break;
case 'b':
{
if(op[1]=='e'){
  	if(op[2]=='q'){
	   inst.opcode=beq;
		}
	}
else printf("Illegal Opcode"); exit(0);
}
break;
case 'l':
{
if(op[1]=='w'){
  inst.opcode=lw;
	}
else printf("Illegal Opcode"); exit(0);
}
break;
}


if(inst.opcode == lw || inst.opcode == sw){
inst.rt = regNumberConverter(token[1]);
if(isImmOperand(token[2])>=0) inst.immediate = atoi(token[2]);
else printf("Immediate field too large"); exit(0);
inst.rs = regNumberConverter(token[3]);
}

else if(inst.opcode == beq){
inst.rs = regNumberConverter(token[1]);
inst.rt = regNumberConverter(token[2]);
if(isImmOperand(token[3])>=0) inst.immediate = atoi(token[3]);
else printf("Immediate field too large"); exit(0);
}

else if(inst.opcode == addi){
inst.rt = regNumberConverter(token[1]);
inst.rs = regNumberConverter(token[2]);
if(isImmOperand(token[3])>=0) inst.immediate = atoi(token[3]);
else printf("Immediate field too large"); exit(0);
}

else{
inst.rd = regNumberConverter(token[1]);
inst.rs = regNumberConverter(token[2]);
inst.rt = regNumberConverter(token[3]); 
}

return inst;

}

	int IF(){
         IFID.write = 1;
    IFID.read = 0;
    if (sim_cycle%c != 0) { //check to see if the program cycle and the latch cycle match: if not sends an error.
        return NULL;
    }
    else { //for everything else, load the latch with the instruction from memory
        IFID.instruction = InstructionMem[pgm_c/4];
    }
	}

	int ID(){
        IFID.read = 1;
    IFID.write = 0;
    IDEX.read = 0;
    IDEX.write = 1;
    if ((IFID.instruction.opcode == haltSim)) { //first check if the instruction opcode is a simulation stop. Pass it through if it is.
        IDEX.instruction = IFID.instruction;
        IDEX.read = IFID.read;
        IDEX.write = IFID.write;
        IDEX.data = IFID.data;
        //IDEX.clock = IFID.clock;
    }
        //NEED TO HAVE A FLAG-TYPE STOP HERE
    else if ((IFID.instruction.opcode == add) || (IFID.instruction.opcode == sub)  || (IFID.instruction.opcode == mul)) { //add, sub, and mult opcodes will follow this statement
        IDEX.instruction.opcode = IFID.instruction.opcode;
        IDEX.instruction.rd = IFID.instruction.rd;
        IDEX.instruction.rs = IFID.instruction.rs;
        IDEX.instruction.rt = IFID.instruction.rt;
    }
    else if ((IFID.instruction.opcode == lw) || (IFID.instruction.opcode == sw)) { //lw and sw opcodes will follow this statement
        IDEX.instruction.opcode = IFID.instruction.opcode;
        IDEX.instruction.immediate = IFID.instruction.immediate;
        IDEX.instruction.rt = IFID.instruction.rt;
        IDEX.instruction.rs = IFID.instruction.rs;
    }
    else if ((IFID.instruction.opcode == addi)) { //addi will follow this statement
        IDEX.instruction.opcode = IFID.instruction.opcode;
        IDEX.instruction.immediate = IFID.instruction.immediate;
        IDEX.instruction.rt = IFID.instruction.rt;
        IDEX.instruction.rs = IFID.instruction.rs;
    }
    else if ((IFID.instruction.opcode == beq)) { //beq will follow this statement
        IDEX.instruction.opcode = IFID.instruction.opcode;
        IDEX.instruction.immediate = IFID.instruction.immediate;
        IDEX.instruction.rt = IFID.instruction.rt;
        IDEX.instruction.rs = IFID.instruction.rs;
    }
    else { //any stall that needs to happen will be done here: it will subtract 0 so that no numbers change.
        IDEX.instruction.opcode = sub;
        IDEX.instruction.immediate = 0;
        IDEX.instruction.rt = 0;
        IDEX.instruction.rs = 0;
        IDEX.instruction.rd = 0;
    }
	}

	int EX(struct latch *inL, struct latch *outL){
        if(((sim_cycle%m) == 0) && (IDEX.instruction.opcode == mul) || ((sim_cycle%n) == 0) && (IDEX.instruction.opcode != mul)) {

        if(IDEX.instruction.opcode == mul){EX_counter+=m;}
        else{EX_counter+=n;}

        switch (IDEX.instruction.opcode) {

            case add :
                EXMEM.instruction = IDEX.instruction;

                EXMEM.data = mips_reg[IDEX.instruction.rs] +
                             mips_reg[IDEX.instruction.rt]; //data is the addition of the values
                break;

            case sub :
                EXMEM.instruction = IDEX.instruction;

                EXMEM.data =
                        mips_reg[IDEX.instruction.rs] -
                        mips_reg[IDEX.instruction.rt]; //data is the addition of the values
                break;

            case addi :
                EXMEM.instruction = IDEX.instruction;
                EXMEM.data =
                        mips_reg[IDEX.instruction.rs] + IDEX.instruction.immediate; //data is the addition of the values
                break;

            case mul :
                EXMEM.instruction = IDEX.instruction;
                EXMEM.data =
                        mips_reg[IDEX.instruction.rs] *
                        mips_reg[IDEX.instruction.rt]; //data is the addition of the values
                break;

            case lw :
                EXMEM.instruction = IDEX.instruction;
                EXMEM.data = EXMEM.instruction.rs +
                            EXMEM.instruction.immediate; //data is the resultant location of word to be loaded
                break;

            case sw :
                EXMEM.instruction = IDEX.instruction;
                EXMEM.data = EXMEM.instruction.rs +
                            EXMEM.instruction.immediate; //data is the resultant location of word to be stored
                break;

            case beq :
                EXMEM.instruction = IDEX.instruction;
                EXMEM.data = mips_reg[IDEX.instruction.rs] -
                            mips_reg[IDEX.instruction.rt]; //data is the difference between the two registers
                break;

        }
  
	}

	void MEM() {
    if ((EXMEM.instruction.opcode != lw)  || (EXMEM.instruction.opcode != sw)) {
        MEMWB.instruction = EXMEM.instruction;
        DataMem[pgm_c/4] = EXMEM.data;
    }
    else if ((EXMEM.instruction.opcode == lw)) { //lw and sw opcodes will follow this statement
        MEMWB.instruction.opcode = EXMEM.instruction.opcode;
        MEMWB.instruction.immediate = EXMEM.instruction.immediate;
        MEMWB.instruction.rt = EXMEM.instruction.rt;
        MEMWB.instruction.rs = EXMEM.instruction.rs;
    }
    else if ((EXMEM.instruction.opcode == sw)) { //addi will follow this statement
        /*    outL->instruction.opcode = inL->instruction.opcode;
            outL->instruction.immediate = inL->instruction.immediate;
            outL->instruction.rt = inL->instruction.rt;
            outL->instruction.rs = inL->instruction.rs; */
    }
}

int WB(struct latch *inL) {

    if ((inL->instruction.opcode != lw) && (inL->instruction.opcode != sw)) {
        mips_reg[inL->instruction.rd] = inL->data;
    } else {
        mips_reg[inL->instruction.rt] = DataMem[inL->data];
    }

    inL->read = 0;
    inL->write = 1;

}

}
