#ifndef MY_FUNCTIONS_H
#define MY_FUNCTIONS_H

struct movie *createMovie(char *currLine);
struct movie *processFile(char *filePath);
void createYearFiles(struct movie *list, char *dirName);
void printMenu();
void printFileMenu();
int generateRandom();


#endif