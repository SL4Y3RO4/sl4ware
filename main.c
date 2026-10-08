#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum type_c {
	INT,
	STR, 
	DFs
} Type;


int is_numeric(char *s) {
	return *s >= '0' && *s <= '9';
}

int is_letter(char *s) {
	return *s >= 'a' && *s <= 'z';
}

int is_operator(char *s) {
	return *s == '<' || *s == '>';
}

char lines[500];
int numvarname = 0;
char* varnames[50];
int numstr = 0;
int nums[200];
int numbers = 0;
Type type[50];
int numtype = 0;
char *strings[500];

int find_var(char *varname) {
	for(int i = 0; i < numvarname; i++) {
		if(strcmp(varname, varnames[i]) == 0) return i;
	}
	return -1;
}

char *conc(char *a, char *b) {
	char *str = malloc((strlen(a) + strlen(b)) + 1);
	int k = 0;
	
	while(*a) {
		str[k++] = *a;
		a++;
	}
	
	while(*b) {
		str[k++] = *b;
		b++;
	}
	
	str[k] = '\0';
	return str;
}


void exec_line(FILE *f, char* s) {
	
	//int statement
	if(strncmp(s, "int", 3) == 0) {
		type[numtype++] = INT;
		s = s + 3;
		char *varname = malloc(strlen(s) + 1);
		char *intBuff = malloc(strlen(s) + 1);
		int i = 0;
		while(*s == ' ') s++;
		
		while(*s != '=') {
			if(is_numeric(s) || is_letter(s)) {
				varname[i++] = *s;
			}
			s++;
		}
		varname[i] = '\0';
		varnames[numvarname++] = varname;
		i = 0;
		
		  if(*s == '=') {
			 s++;
			 while(*s != ';') {
			 	if(is_numeric(s)) {
			 		intBuff[i++] = *s;
				 }
			 	s++;
			 }
			 intBuff[i] = '\0';
			
		  }
		  nums[numbers++] = atoi(intBuff);
		 // free(intBuff);
	   //string statement
	} else if(strncmp(s, "string", 6) == 0) {
	   type[numtype++] = STR;
	   s = s + 6;
	   char *varname = malloc(strlen(s) + 1);
	   char *string = malloc(strlen(s) + 1);
	   int i = 0;

	   while(*s == ' ') s++;
	
	   while(*s != '=') {
			if(is_numeric(s) || is_letter(s)) {
				varname[i++] = *s;
			}
			s++;
		}
		varname[i] = '\0';
		varnames[numvarname++] = varname;
		i = 0;
		s++;
		while(*s != ';') {	
		    string[i++] = *s;
			s++;
		} 
		string[i] = '\0';
	    strings[numstr++] = string;
	    
	  //syscall statement  
    } else if(strncmp(s, "syscall", 7) == 0) {
      s = s + 7;
	  char *varname = malloc(strlen(s) + 1);
	  int i = 0;
	  
	  while(*s == ' ') s++;
	  
	  while(*s != ';') {
	  	 varname[i++] = *s;
	  	 s++;
	  }
	  varname[i] = '\0';
//	  int check = 0;
	  
	  for(int i = 0; i < numvarname; i++) {
	  	  if(strcmp(varname, varnames[i]) == 0) {
	  	  	   system(strings[i]);
		  } else {
		  	   system(varname);
		  	  // free(varname);
		  	   break;
		  }
	  }	  
	  //print statement
	} else if(strncmp(s, "print", 5) == 0) {
		s = s + 5;
		char *varname = malloc(strlen(s) + 1);
		int i = 0;
		
		while(*s == ' ') s++;
		
		while(*s != ';') {
			if(is_numeric(s) || is_letter(s)) {
				varname[i++] = *s;
			}
			s++;
		}
		varname[i] = '\0';
		
		for(int i = 0; i < numvarname; i++) {
			if(strcmp(varname, varnames[i]) == 0) {
				switch(type[i]) {
					case INT: printf("%d", nums[i - 1]); break;
					case STR: printf("%s", strings[i - 1]); break;
				}
			
			}
		}
		//add statement 
	} else if(strncmp(s, "add", 3) == 0) {
		s = s + 3;
		char *vname = malloc(strlen(s) + 1);
		char *integer = malloc(strlen(s) + 1);
		int vi = 0;
		int i = 0;
		
		while(*s != ' ') s++;
		
		//take variable name
		while(*s != ',') {
			if(is_numeric(s) || is_letter(s)) {
				vname[vi++] = *s;
			}
			s++;
		}
		vname[vi] = '\0';
	  
	      //take int value
	  	  while(*s != ';') {
	    	if(is_numeric(s)) {
	    	   integer[i++] = *s;	
			}
	    	s++;
	      }
		//increase current variable
		for(int i = 0; i < numvarname; i++) {
		   if(strcmp(vname, varnames[i]) == 0) {
		   	  nums[i] = nums[i] + atoi(integer);
		   }	   
		}
	
	   //mv statement
	} else if(strncmp(s, "mv", 2) == 0) {
		s = s + 2;
		char *vname = malloc(strlen(s) + 1);
		char *vname2 = malloc(strlen(s) + 1);
		int vi = 0;
		int k = 0;
		
		while(*s == ' ') s++;

        //take variable name1	
        while(*s != ',') {
        	if(is_numeric(s) || is_letter(s)) {
        	 	vname[vi++] = *s;
			}
        	s++;
		}
		vname[vi++] = '\0';
		s++;
		vi = 0;
		//take variable name2	
        while(*s != ';') {
        	if(is_numeric(s) || is_letter(s)) {
        	 	vname2[vi++] = *s;
			}
        	s++;
		}
		vname2[vi++] = '\0';
		
		char *take_int = malloc(strlen(strings[0]) + 1);
		
		int str_check1 = 0;
		int str_check2 = 0;
		int idx_str = -1;
		int idx_int = -1;
		
	    for(int i = 0; i < numvarname; i++) {
	    	if(strcmp(vname, varnames[i]) == 0 && type[i] == STR) {
	    		str_check1 = 1;
	    		idx_str = i;
			}
		}
		
		for(int i = 0; i < numvarname; i++) {
	    	if(strcmp(vname2, varnames[i]) == 0 && type[i] == INT) {
	    		sprintf(take_int, "%d", nums[i]);
	    		str_check2 = 1;
	    		idx_int = i;
			}
			k++;
		}
		take_int[k] = '\0';
		
	
	    if(str_check1 && str_check2) {
	    	// tronca la stringa all'ultimo '.' (mantenendolo)
	    	char *dot = strrchr(strings[idx_str], '.');
	    	if(dot != NULL) {
	    		*(dot + 1) = '\0';
	    	}
	    	
	    	// ricostruisci la stringa con il nuovo numero
	    	char *tmp = conc(strings[idx_str], take_int);
	    	free(strings[idx_str]);
	    	strings[idx_str] = tmp;
	    	free(take_int);
		}
	    
		
	  //var statement
	} else if(strncmp(s, varnames[numvarname-1], strlen(varnames[numvarname-1])) == 0) {
	  char *p = s;
	  char vname[200];
	  int i = 0;
	  int vi = 0;
	  int check = 0;
	  
	  while(*p == ' ' || *p == '\t') p++;
	  
	  while(*p && (is_letter(p) || is_numeric(p) || *p == '_')) {
	        vname[vi++] = *p;  
			p++; 
	  }
	  vname[vi] = '\0';
	  
	   int idx = find_var(vname);
	   if (idx < 0) {
	  	  return;
	   }
	  
	  s = p;  
	  char *intBuffer = malloc(strlen(p) + 1);
	  char *varname = malloc(strlen(p) + 1);
	  
	  while(*s == ' ') s++;
	  
	  //++ 
	  if(strncmp(s, "++", 2) == 0 && type[idx] == INT) { 
	  	 	nums[idx]++; 
	  }
	  //--
	  if(strncmp(s, "--", 2) == 0 && type[idx] == INT) {
	  	 	nums[idx]--; 	 
      }
	  
	  if(*s == '=' && type[idx] == INT) {
	  	 s++;
	  	 while(*s != ';') {
	  	 	if(is_numeric(s)) {
	  	 		intBuffer[i++] = *s;
			}	
	  	 	s++;
		   }
		   intBuffer[i] = '\0';
		   
		   nums[idx] = atoi(intBuffer);
	  }
	  i = 0;
	  if(*s == ',' && type[idx] == STR) {
	  	 s++;
	  	 while(*s != ';') {
	  	 	if(is_numeric(s) || is_letter(s)) {
	  	 		varname[i++] = *s;
			}
	  	 	s++;
		 }
	  }
	  varname[i] = '\0';
	  
	   if (type[idx] == STR && varname[0] != '\0') {
	  	  int other = find_var(varname);
	  	  if (other >= 0 && type[other] == INT) {

	  	  	  char tmp[600];
	  	  	  snprintf(tmp, sizeof(tmp), "%s%d", strings[idx], nums[other]);
	  	  	  free(strings[idx]);
	  	  	  strings[idx] = strdup(tmp);
	  	  }
	  }
	  
	  
	} //EOF var

}


int main(int argc, char *argv[]) {
	
	char *filename = malloc(strlen(argv[1]) + 4 + 1);	
	sprintf(filename, "%s.sl4w2", argv[1]);
	FILE* file = fopen(filename, "r");
	
	free(filename);
	
	while(fgets(lines, 500, file) != NULL) {
		
		lines[strcspn(lines, "\n")] = '\0';
			          
	    char *s = lines;		          
		while(*s == ' ' || *s == '\t' || *s == '\n' || *s == '=' || *s == '\0') s++;
		
		//for statement
		if(strncmp(s, "for", 3) == 0) {
			s = s + 3;
			int *intBuffer = malloc(strlen(s) + 1);
			int i = 0;
			char op = '\0';
			while(*s == ' ') s++;
			
			while(*s != '{') {
				
				if(is_numeric(s)) {
					intBuffer[i++] = atoi(s);				
				}
				s++;
			}
			long init_smt = ftell(file);
				
			for(int i = intBuffer[0]; i < intBuffer[1]; i++) {
				
				//place pointer here and repeat instruction
				fseek(file, init_smt, SEEK_SET);
				
				//shift all!
				while(fgets(lines, 500, file) != NULL) {
					
					lines[strcspn(lines, "\n")] = '\0';
					
					char *p = lines;
					
					while(*p == ' ' || *p == '\t') p++;
					
					if(*p == '}') break;
					 	 
						  exec_line(file, p);
				     	  
				}
			}
			
				
		} //EOF for
		 exec_line(file, s);
		   
	}
	fclose(file);

}



