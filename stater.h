//Bash made by Vansh Marda
//2021101089
//IIIT H - CSE

#ifndef UNTITLED_UTIL_H
#define UNTITLED_UTIL_H
int max(int a, int b);
int min(int a, int b) ;
void absolute_relativer_addressing(char *new_address, char *cd_location, const char *curr_dir, const char *home_dir);
void clearScreen();
void printInitials(char* shellName , char* showDir);
char* username();
char* remove_whitespace(char* line);
void process_adding(char *tokens[], int num, int bg);
#endif //UNTITLED_UTIL_H
