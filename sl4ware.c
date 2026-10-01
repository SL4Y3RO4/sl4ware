#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum type_c {
     INT,
     STR,
     DFS
} Type;


int slen(char *s) { 
     int i = 0;
     while(*s != '\0') {
      s++;
      i++;
     }
     return i;
}

int is_numeric(char *s) {
      return *s >= '0' && *s <= '9';
}

int is_lowcase(char *s) {
      return *s >= 'a' && *s <= 'z';
}

int is_specialc(char *s) {
      return *s == '<' || *s == '>' || *s == '/' ||
             *s == '@' || *s == '#' || *s == '?' ||
             *s == ' ' || *s == '|' || *s == '!' ||
             *s == '"' || *s == ',' || *s == '.' || 
             *s == '_' || *s == '-';
}

int main(int argc, char* argv[]) {

     char *filename = malloc(slen(argv[1]) + 3 + 1);
     sprintf(filename, "%s.sl4", argv[1]);
     FILE* f = fopen(filename, "r");

     free(filename);
     
     char lines[500];
     int nums[200];
     char *varnames[50];
     char *strvec[50];
     int numc = 0;
     int numl = 0;
     int numstr = 0;
     Type typec[50];
     int numtypes = 0;

     while(fgets(lines, 500, f) != NULL) {
             
           lines[strcspn(lines, "\n")] = '\0';

           char *s = lines;

           while(*s == ' ' || *s == '\n' || *s == '\t' || *s == '=' || *s == '\0') s++;

           if(strncmp(s, "int", 3) == 0) {
              typec[numtypes++] = INT;
              s = lines + 3;
              char *varname = malloc(slen(s) + 1);
              char *intBuffer = malloc(slen(s) + 1);
              
              int i = 0;
              while(*s != '=') {
                    if(is_numeric(s) || is_lowcase(s)) {
                        varname[i++] = *s;
                    } 
                    s++;
              }
              varname[i] = '\0';
              varnames[numl++] = varname;
            
              if(*s == '=') {
                 s++;
                 i = 0; //dealloc
                 while(*s != ';') {
                    if(is_numeric(s)) {
                       intBuffer[i++] = *s;
                    }
                    s++;
                 }
              }
              intBuffer[i] = '\0';
            
             
              nums[numc++] = atoi(intBuffer);
                  
             //print
           } else if(strncmp(s, "print", 5) == 0) {
             s = lines + 5;
             char *varname = malloc(slen(s) + 1);
              
              int i = 0;
              while(*s != ';') {
                    if(is_numeric(s) || is_lowcase(s)) {
                        varname[i++] = *s;
                    } 
                    s++;
              }

              for(int i = 0; i < numl; i++) {
                  if(strcmp(varname, varnames[i]) == 0) {  
                      switch(typec[i]) {
                         case STR: printf("%s", strvec[i]); break;
                         case INT: printf("%d", nums[i]); break;
                      }
                         
                  }
              }
              //system
           } else if(strncmp(s, "system", 6) == 0) {
             s = lines + 6;
             char* string = malloc(slen(s) + 1);
             char* varname = malloc(slen(s) + 1);
             int i = 0;

             while(*s == ' ') s++;

             if(*s != '"') {
                 while(*s != ';') {
                  if(is_lowcase(s) || is_numeric(s)) {
                      varname[i++] = *s;
                  }
                  s++;
             }
             varname[i] = '\0';
             }
            

             if(*s == '"') {        
                s++;
                i = 0;
                while(*s != '"') {
                  string[i++] = *s;  
                  s++;
                }
             } else {
                for(int i = 0; i < numl; i++) {
                    if(strcmp(varname, varnames[i]) == 0) {
                       system(strvec[i]);
                    }
                }  
             }
             string[i] = '\0';
             printf("%s", string);
             system(string);
            
             //string
           } else if(strncmp(s, "string", 6) == 0) {
             typec[numtypes++] = STR;   
             s = lines + 6;
             char *varname = malloc(slen(s) + 1);
             char *string = malloc(slen(s) + 1);
             int i = 0;

             //find varname and store in a vector
             while(*s != '=') {
                 if(is_lowcase(s) || is_numeric(s)) {
                    varname[i++] = *s;
                 }
                 s++;
             }
             varname[i] = '\0';
             varnames[numl++] = varname;

             if(*s == '=') {  
                  s++;
                  i = 0;
                  while(*s != ';') {
                     if(is_lowcase(s) || is_numeric(s) || is_specialc(s)) {
                        string[i++] = *s;     
                     }
                     s++;
                  }
             
            }
             string[i] = '\0';
             strvec[numstr++] = string;     
              
             //variable
           } else if(strncmp(s, varnames[numl-1], slen(varnames[numl-1])) == 0) {
             s = lines + slen(varnames[numl-1]);
             char *string = malloc(slen(varnames[numl-1]) + 1);
             int i = 0;

             while(*s == ' ') s++;

             if(*s == '=') {
                s++;
                while(*s != ';') {
                    string[i++] = *s;
                    s++;
                }
             }
             string[i] = '\0';
             strcpy(strvec[numstr-1], string);

           } else if(strncmp(s, "for", 3) == 0) {
             s = lines + 3;
             while(*s == ' ') s++;

             //TODO

           }

     }                              
     fclose(f);

}