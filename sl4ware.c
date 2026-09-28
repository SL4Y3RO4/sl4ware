#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

int main() {
     FILE* f = fopen("txt.sl4", "r");
     
     char lines[500];

     int nums[200];
     char *varnames[50];
     int numc = 0;
     int numl = 0;

     while(fgets(lines, 500, f) != NULL) {
             
           lines[strcspn(lines, "\n")] = '\0';

           char *s = lines;

           while(*s == ' ' || *s == '\n' || *s == '\t' || *s == '=' || *s == '\0') *s++;

           if(strncmp(s, "int", 3) == 0) {
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
                    printf("%d", nums[i]);
                  }
              }
           }

     }
     fclose(f);

}