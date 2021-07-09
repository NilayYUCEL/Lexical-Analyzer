#include <stdio.h>
#include <ctype.h>
#include <windows.h>	
#include <stdbool.h>


//TOKENS
#define LETTER 0
#define DIGIT 1
#define OPERATOR 2
#define INT_LIT 3
#define IDENT 4
#define UNDERSCORE 5
#define LEFTPAR 6
#define STAR 7
#define RIGHTPAR 8
#define UNKNOWN -1
#define FINISHED -2
#define ERROR 9
#define QUOTE 14
#define COMMENTERROR 15


//to check number of parenthesis
int par_control=0;
int leftpar_control =0;

#define LEFTSQUARE 10
#define RIGHTSQUARE 11
int sq_control=0;
int leftsq_control =0;

#define LEFTCURLY 12
#define RIGHTCURLY 13
int curly_control=0;
int leftcurly_control =0;




char line[100]; // to read by line line
char input[10000]; // to read all file without spaces and new lines

// for writing output to output file
char output[100]; 
char end_output[1000];

// to read input char by char
int inputIndex=0;

// to find tokens
int token=-1;

// to separate the comment line and rightpar
bool flag=true;


// to find tokens
int charClass;
char lexeme[50];
char nextChar;
char currentChar;
int lexLen;

int error=0;


// to find and register the desired token (especially for ident, string constant  and comments )
void addChar()
{
    lexeme[lexLen++] =nextChar;
    
}

//to find out what happened in the next character
char getNextChar()
{
	if(input[inputIndex] !='\0')
	{
		return input[inputIndex++];
	}
	else return NULL;
}

// to find out which token is the next character(charClass)
void getChar()
{
    if((nextChar=getNextChar())!=NULL)
    {
        if (nextChar=='+' || nextChar=='-'  || nextChar=='*' || nextChar=='/' || nextChar==':' ||nextChar=='='){
            charClass = OPERATOR; //2
        }
        else if(nextChar=='_'){
        	charClass=UNDERSCORE; //5
		}
		else if(nextChar=='('){
			charClass=LEFTPAR; //6
		}
		else if(nextChar==')'){
			charClass=RIGHTPAR;//8
		}
		else if(nextChar=='['){
			charClass=LEFTSQUARE; //10
		}
		else if(nextChar==']'){
			charClass=RIGHTSQUARE;//11
		}
		else if(nextChar=='{'){
			charClass=LEFTCURLY; //12
		}
		else if(nextChar=='}'){
			charClass=RIGHTCURLY;//13
		}
		else if((int)nextChar==34){
			charClass=QUOTE; //14
		}
		else
        {
            if (isalpha(nextChar))
            {
                charClass=LETTER; //0
            }
            else
            {
                if (isdigit(nextChar))
                    charClass = DIGIT; //1
                else
                    charClass = UNKNOWN; //-1
            }
        }
    }

    else
        charClass = FINISHED; //-2
}

// for current character operations
int lex()
{
    int i;
    static int first = 1;
    for (i=0; i<lexLen; i++)
        lexeme[i]='\0'; 
    lexLen=0;
    
    // just for first char
    if (first)
    {
        getChar(); //charclass belirlendi
        first=0; 
    }
    
    //for another chars
    switch(charClass)
    {
        case LETTER:
            addChar(); //adding new char to lexeme arrays
            getChar(); //to find charclass for nextchar 
            
            
			// to find identy, keywords
            while (charClass == LETTER || charClass == DIGIT || charClass == UNDERSCORE)
            {
                addChar(); //adding new char to lexeme arrays
	            getChar(); //charclass belirleniyo nextchar için
    
            }
            
            // equality operator check
			if(currentChar!=':' && nextChar=='='){
            	printf("Error Message: \"=\" and \":\" operators must use together!\n"); // print to terminal screen
            	strcpy(output,"Error Message: \"=\" and \":\" operators must use together!\n"); //to write to output file
				write_file();  // writing output file
				strcpy(output,"");// clean current token 
            	return ERROR;
            	break;
			}

            return IDENT; //4
            break;
            
        case DIGIT:
            addChar();  //adding new char to lexeme arrays
            currentChar = nextChar;// to register current char
            getChar(); //to find charclass for nextchar 
            
            //Identifiers cannot start with a number
            if(charClass==LETTER){
            		printf("Error Message: Indentifier can not start with an digit!!\n");// print to terminal screen
            		strcpy(output,"Error Message: Indentifier can not start with an digit!!\n"); //to write to output file
					write_file(); // write output file
					strcpy(output,""); // clean current token 
            		return ERROR;
            		break;
			}
            
            // to find Integer constants
            while (charClass == LETTER || charClass == DIGIT)
            {
                addChar(); //adding new char to lexeme arrays
                getChar(); //charclass belirleniyo nextchar için
            }
        	
        	// equality operator check
        	if(currentChar!=':' && nextChar=='='){
            	printf("Error Message: \"=\" and \":\" operators must use together!\n");// print to terminal screen
            	strcpy(output,"Error Message: \"=\" and \":\" operators must use together!\n"); //to write to output file
				write_file(); // write output file
				strcpy(output,""); // clean current token 
            	return ERROR;
            	break;
			}

            return INT_LIT; //3
            break;
            
        case OPERATOR:
            addChar(); //adding new char to lexeme arrays
            currentChar = nextChar; // to register current char
            getChar(); //to find charclass for nextchar 
            
            //control for invalid operators 
            if(charClass==OPERATOR){
            	if(!((currentChar==':' && nextChar=='=') || (currentChar=='+' && nextChar=='+') || (currentChar=='-' && nextChar=='-'))){
            		printf("Error Message: Invalid operator usage.\n"); // print to terminal screen
	            	strcpy(output,"Error Message: Invalid operator usage.\n"); //to write to output file
					write_file(); // write output file
					strcpy(output,""); // clean current token 
            		return ERROR;
            		break;
				}
							
			} 
			
			// find valid operators
            while (charClass == OPERATOR && ((currentChar==':' && nextChar=='=') || (currentChar=='+' && nextChar=='+') ||(currentChar=='-' && nextChar=='-')  ))
            {
            	addChar(); //adding new char to lexeme arrays
            	currentChar = nextChar;  // to register current char
                getChar(); //to find charclass for nextchar 
                	
                // control for  negative integer 	
				if(currentChar == '=' && nextChar == '-'){
					printf("Error Message: Negative values are not supported."); // print to terminal screen
					strcpy(output,"Error Message:  Negative values are not supported.\n");//to write to output file
					write_file(); // write output file
					strcpy(output,""); // clean current token 
					return ERROR; //2
            		break;
				}
				                
            }
            
            	// equality operator check
            if((currentChar!=':' && nextChar=='=') || (currentChar==':' && nextChar!='=')){
            	printf("Error Message: \"=\" and \":\" operators must use together!\n"); // print to terminal screen
            	strcpy(output,"Error Message: \"=\" and \":\" operators must use together!\n"); //to write to output file
				write_file(); // write output file
				strcpy(output,""); // clean current token 
            	return ERROR;
            	break;
			}
            return OPERATOR; //2
            break;
            
        // for String constants
        case QUOTE:
            do {
	            addChar(); //adding new char to lexeme arrays
	            getChar(); //to find charclass for nextchar 
	            
	            //string constant control for end of file 
	            if(nextChar==NULL){
	            	printf("Error Message: Double quotes error!\n");// print to terminal screen
	            	strcpy(output,"Error Message: Double quotes error\n"); //to write to output file
					write_file(); // write output file
					strcpy(output,"");  // clean current token 
                    return ERROR;
                }
            }
			while(charClass != QUOTE );
			
			// for finding nextchar to continue
            addChar(); //lexeme arrayine nextcharý ekliyo
            getChar(); //to find charclass for nextchar 
            
			return QUOTE;
			break;         		
        	
        // for control commentline and - Brackets	
        case LEFTPAR:
        	addChar(); //adding new char to lexeme arrays
        	currentChar = nextChar; // to register current char
            getChar(); //to find charclass for nextchar 
            
            // control for comment line
            if(nextChar=='*'){
            	addChar();
            	getChar();
            	
            	//control for end of comment line 
            	while (nextChar != '*' ){
	                addChar();
	                getChar();
	                
	                // control for end of file 
	            	if(nextChar==NULL){
	                	return COMMENTERROR;
					}
            	}
            	addChar();
                getChar();
                
                //control for end of comment line 
            	if(charClass == RIGHTPAR){
            		flag=false;
            		addChar();
                	getChar();
            		return RIGHTPAR;
				}
				else{
					printf("Error Message: Comment Error.");
					strcpy(output,"Error Message:  Comment Error.\n");
					write_file();
					strcpy(output,"");					
					return ERROR;//15
					break;
				}
           	}
           	else{
           		//its is not comment line. it is leftpar control
           		leftpar_control=1;
           		par_control++;
           		return LEFTPAR;
			   }
        	break;
        	
		// for - Brackets
		case RIGHTPAR:
			
			//control for number of parentheses
			if(leftpar_control && par_control>=0){
				addChar();
            	getChar();
           	 	par_control--;
            	return RIGHTPAR;
			}
			else{
					printf("Error Message: Parentheses missing.");
					strcpy(output,"Error Message:  Parentheses missing.\n");
					write_file();
					strcpy(output,"");
					return ERROR;			
			}
            break;
        
        // Brackets
		case LEFTSQUARE:
			addChar();
            getChar();
            //control for number of parentheses
           	leftsq_control=1;
           	sq_control++;
           	return LEFTSQUARE;
        	break;   	
        //for Brackets	
        case RIGHTSQUARE:
        	//control for number of parentheses
			if(leftsq_control && sq_control>=0){
				addChar();
            	getChar();
           	 	sq_control--;
            	return RIGHTSQUARE;
            }
			else{
					printf("Error Message: Parentheses missing.");
					strcpy(output,"Error Message:  Parentheses missing.\n");
					write_file();
					strcpy(output,"");
					return ERROR;			
			}
            break;
 		//for Brackets
		case LEFTCURLY:
			addChar();
            getChar();
			//control for number of parentheses
           	leftcurly_control=1;
           	curly_control++;
           	return LEFTCURLY;
        	break;   	
        //for Brackets
        case RIGHTCURLY:
        	//control for number of parentheses
			if(leftcurly_control && curly_control>=0){
				addChar();
            	getChar();
           	 	curly_control--;
            	return RIGHTCURLY;
            }
			else{
					printf("Error Message: Parentheses missing.");
					strcpy(output,"Error Message:  Parentheses missing.\n");
					write_file();
					strcpy(output,"");
					return ERROR;			
			}
            break;

        case FINISHED:
            return FINISHED; //-2
            break;
            
        // to control ident     
        case UNDERSCORE:
			printf("Error Message: Indentifier must start with a letter!!");
			strcpy(output,"Error Message: Indentifier must start with a letter!!\n");
			write_file();
			strcpy(output,"");
			return ERROR;
            break;
			           	
        case UNKNOWN:
            addChar();
            getChar();
            return UNKNOWN; //-1
            break;
    }
}

// to remove spaces in input
void remove_spaces(char* input) {
    const char* d = input;
    int cnt=0; // control spaces in string constant
    do {
    	if((int)*d!=34 && cnt%2==0){ 
	        while (*d == ' ') {
	            ++d;
	        }
		}
        else if ((int)*d==34){
        	cnt++;
       }
    } while (*input++ = *d++);
    
  
}

//to make all characters smaller
void case_sensitive(){
	int i=0;
	for(i=0;i<=strlen(input);i++){
      if(input[i]>=65&&input[i]<=90)
         input[i]=input[i]+32;
   }
}

//Reading File
void read_file(){

 FILE *sourcefile = fopen("code.Ceng", "r");
 
 while (fgets(line, sizeof line, sourcefile) != NULL) {
	  if ( line[strlen(line)-1] == '\n') {
	    line[strlen(line)-1] = '\0';
	  }
	 	strncat(input, &line, strlen(line)); // record lines
	 
	}	     
	fclose(sourcefile);
	remove_spaces(input); // to remove spaces in input
	case_sensitive(); //to make all characters smaller
}

//Writing File
void write_file(){
	
  FILE *endfile = fopen("code.lex", "w");
  strncat(end_output, &output, strlen(output));// combined current token and last output file
  fprintf(endfile,end_output);
  fclose(endfile);
	
}

// MAIN
int main()
{ 

    read_file();
    //to fint current token
    while((token = lex())!=FINISHED){
    	
    	// control end of line operator
    	if(!strcmp(lexeme,";")){
    		strcpy(output,"EndOfLine\n"); //to write to output file
			write_file();// writing output file
			strcpy(output,""); // clean current token 
    		printf("EndOfLine\n"); // print to terminal screen
		}
		// to  Identifiers
		else if(token==IDENT ){
			// control Identifiers size
			if(lexLen>20){
				printf("Error Message: String type must be smaller than 20 char!\n");
				strcpy(output,"Error Message: String type must be smaller than 20 char!\n");
				write_file();
				strcpy(output,"");
			//	break;
			}
			
			//control for keywords
			else if(!strcmp(lexeme,"break") || !strcmp(lexeme,"case") || !strcmp(lexeme,"char") || !strcmp(lexeme,"const") || !strcmp(lexeme,"continue") || !strcmp(lexeme,"do") || !strcmp(lexeme,"else") || !strcmp(lexeme,"enum") || !strcmp(lexeme,"float") || !strcmp(lexeme,"for") || !strcmp(lexeme,"goto") || !strcmp(lexeme,"if") || !strcmp(lexeme,"int")|| !strcmp(lexeme,"long") || !strcmp(lexeme,"record") || !strcmp(lexeme,"return") || !strcmp(lexeme,"static") ||!strcmp(lexeme,"while")){
				printf("Keyword(%s)\n",lexeme);
				strcpy(output,"Keyword(");
				strncat(output, &lexeme, strlen(lexeme));
				strcat(output,")\n");	
				write_file();
				strcpy(output,"");
			}
			else{
				printf("Identifier(%s)\n",lexeme);
				strcpy(output,"Identifier(");
				strncat(output, &lexeme, strlen(lexeme));
				strcat(output,")\n");				
				write_file();
				strcpy(output,"");
			}	
		}
		// to operators
		else if(token==OPERATOR){
			printf("Operator(%s)\n",lexeme);
			strcpy(output,"Operator(");
			strncat(output, &lexeme, strlen(lexeme));
			strcat(output,")\n");
			write_file();
			strcpy(output,"");
		}
		// to Integer constants
		else if(token==INT_LIT  ){
			// control integer constants size
			if(lexLen>10){
				printf("Error Message: Maximum integer size is 10 digits!\n");
				strcpy(output,"Error Message: Maximum integer size is 10 digits!\n");
				write_file();
				strcpy(output,"");
			//	break;
			}
			else{
				printf("IntConst(%s)\n",lexeme);
				strcpy(output,"IntConst(");
				strncat(output, &lexeme, strlen(lexeme));
				strcat(output,")\n");
				write_file();
				strcpy(output,"");
			}
		}
		//to String Constant
		else if(token==QUOTE){	
			printf("String Constants(%s)\n",lexeme);		
			strcpy(output,"String Constants(");
            strncat(output, &lexeme, strlen(lexeme));
            strcat(output,")\n");
            write_file();
            strcpy(output,"");
		}
		
		//////////// TO BRACKETS ///////////
		else if(token==LEFTPAR){
    		printf("LeftPar\n");
			strcpy(output,"LeftPar\n");
			write_file();
			strcpy(output,"");
		}
		else if(token==RIGHTPAR){
			
			if(flag){// to define, it is commnet line or just bracket
	    		printf("RightPar\n");
				strcpy(output,"RightPar\n");
				write_file();
				strcpy(output,"");
    		}
    		else{
    			flag=true;
			}
		}
		else if(token==LEFTSQUARE){
    		printf("LeftSquareBracket\n");
			strcpy(output,"LeftSquareBracket\n");
			write_file();
			strcpy(output,"");
		}
		else if(token==RIGHTSQUARE){
    		printf("RightCurlyBracket\n");
			strcpy(output,"RightCurlyBracket\n");
			write_file();
			strcpy(output,"");
		}
		else if(token==LEFTCURLY){
    		printf("LeftCurlyBracket\n");
			strcpy(output,"LeftCurlyBracket\n");
			write_file();
			strcpy(output,"");
		}
		else if(token==RIGHTCURLY){
    		printf("RightCurlyBracket\n");
			strcpy(output,"RightCurlyBracket\n");
			write_file();
			strcpy(output,"");
		}
		
		// to commentline errors
		else if(token == COMMENTERROR){
			printf("Error Message: Comment error\n");
			strcpy(output,"Error Message: Comment is wrong! \n");
			write_file();
			strcpy(output,"");
    	//	break;
		}
		else if(token==ERROR){
		//	break;
		}
	
	}
	
	//control for number of brackets
	if(par_control!=0 || sq_control!=0 || curly_control!=0){
			printf("\nError Message: Missing operator! Please check brackets.\n");
			strcpy(output,"\nError Message: Missing operator! Please check brackets.\n");
			write_file();
			strcpy(output,"");
	}

    system("PAUSE");    
    return 0;
} 




//REFERENCES: https://beginnersbook.com/2015/02/c-program-to-convert-uppercase-string-to-lowercase-string/
//https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
