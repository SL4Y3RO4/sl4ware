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


void exec_line(FILE *f, char* s) {
	
	//int statement
	if(strncmp(lines, "int", 3) == 0) {
		type[numtype++] = INT;
		s = lines + 3;
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
	   s = lines + 6;
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
      s = lines + 7;
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
	
	} else if(strncmp(s, "print", 5) == 0) {
		s = lines + 5;
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
					case INT: printf("%d", nums[i]); break;
					case STR: printf("%s", strings[i]); break;
				}
			
			}
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
	  	  /* variabile non trovata: non fare nulla */
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
		
		//for
		if(strncmp(s, "for", 3) == 0) {
			s = lines + 3;
			int *intBuffer = malloc(strlen(s) + 1);
			int i = 0;
			char op = '\0';
			while(*s == ' ') s++;
			
			while(*s != '(') {
				
				if(is_numeric(s)) {
					intBuffer[i++] = atoi(s);				
				}
				s++;
			}
			long init_smt = ftell(file);
				
			for(int i = intBuffer[0]; i < intBuffer[1]; i++) {
				
				fseek(file, init_smt, SEEK_SET);
				while(fgets(lines, 500, file) != NULL) {
					
					lines[strcspn(lines, "\n")] = '\0';
					
					char *p = lines;
					
					while(*p == ' ' || *p == '\t') p++;
					
					if(*p == ')') break;
					 	 
						  exec_line(file, p);
				     	  
				}
			}
			
				
		} //EOF for
		 exec_line(file, s);
		   
	}
	fclose(file);

}



