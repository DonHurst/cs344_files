#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#include "functions.h"

#define PREFIX "movies_"
#define EXTENSION ".csv"
#define ONID "hurstd"
#define MOVIES "movies"


int main(int argc, char* argv[]) {

    // Declare variable for switch statement choice
    int choice;
    // Create int for next choice
    int fileChoice;

    do {
        // Print menu and take user choice
        printMenu();
        scanf("%d", &choice);

        // Open current directory
        DIR* currDir; 
        struct dirent* aDir;
        currDir = opendir(".");

        // Create variables for use in corresponding choices
        struct stat dirStat;
        struct stat largestDir;
        struct stat smallestDir;
        int matchingFiles = 0;
        char entryName[256];
        struct movie *list;
            
        switch(choice) {    
            
            // Open submenu
            case 1:
                
                do {
                    currDir = opendir(".");
                    // Print menu and take choice
                    printFileMenu();
                    scanf("%d", &fileChoice);

                    // Generate a random value
                    int randNum = generateRandom();

                    // Create a variable for new directory name
                    char newDirName[255];
                    
                    switch(fileChoice) {

                        // Largest file with CSV extension
                        case 1:

                            // Go through all entries in directory
                            while((aDir = readdir(currDir)) != NULL) {

                                // IF file has a matching prefix and extension
                                if((strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) && 
                                (strncmp(EXTENSION, aDir->d_name + strlen(aDir->d_name)-4, strlen(EXTENSION)) == 0))
                                {
                                    // Get the stats for the directory
                                    stat(aDir->d_name, &dirStat);

                                    // If there are no other valid files, set stats for largest dir and copy to name
                                    if (matchingFiles == 0){
                                        stat(aDir->d_name, &largestDir);
                                        strcpy(entryName, aDir->d_name);
                                    }
                                    // If there are other valid files, compare their size and take the larger one
                                    else {
                                        if (largestDir.st_size < dirStat.st_size) {
                                            stat(aDir->d_name, &largestDir);
                                            strcpy(entryName, aDir->d_name);
                                        }
                                    }
                                    // Increase the number of valid files
                                    matchingFiles++;

                                }       

                            }      
                            printf("\nNow processing the chosen file name %s\n", entryName); 

                            // Create new directory and process the list
                            sprintf(newDirName, "hurstd.movies.%d", randNum);
                            mkdir(newDirName, 0750); 
                            printf("Created directory with name %s\n", newDirName);
                            
                            // Create list of movies from the file and use it to create subsequent files
                            list = processFile(entryName); 
                            createYearFiles(list, newDirName);
                            
                            break;

                        // Smallest file with CSV extension
                        case 2:

                            // Go through all entries in directory
                            while((aDir = readdir(currDir)) != NULL) {

                                // If the prefix is correct and the extension is correct
                                if((strncmp(PREFIX, aDir->d_name, strlen(PREFIX)) == 0) && 
                                (strncmp(EXTENSION, aDir->d_name + strlen(aDir->d_name)-4, strlen(EXTENSION)) == 0))
                                {
                                    // Get the stats fir the current file
                                    stat(aDir->d_name, &dirStat);

                                    // If this is the first file, copy this data to largest dir
                                    // Copy file name to entryName string
                                    if (matchingFiles == 0){
                                        stat(aDir->d_name, &smallestDir);
                                        strcpy(entryName, aDir->d_name);
                                    }

                                    // If not the first file
                                    else {
                                        // If the size of the largest dir is bigger than the current
                                        if (smallestDir.st_size > dirStat.st_size) {

                                            // Copy 
                                            stat(aDir->d_name, &smallestDir);
                                            strcpy(entryName, aDir->d_name);
                                        }
                                    }
                                    matchingFiles++;

                                }       

                            }      
                            
                            printf("\nNow processing the chosen file name %s\n", entryName); 
                            
                            // Create new directory and process the list
                            sprintf(newDirName, "hurstd.movies.%d", randNum);
                            mkdir(newDirName, 0750); 
                            printf("Created directory with name %s\n", newDirName);
                            
                            // Create list of movies from the file and use it to create subsequent files
                            list = processFile(entryName); 
                            createYearFiles(list, newDirName);
                            break;

                        // Ask user for file name
                        case 3:

                            printf("Please enter a file name: \n");
                            scanf("%s", entryName);
                            matchingFiles = 0;

                            // Go through all entries in directory
                            while((aDir = readdir(currDir)) != NULL) {
                                if((strcmp(entryName, aDir->d_name) == 0)) {
                                    
                                    matchingFiles += 1;
                                    // Generate a random value
                                    int randNum = generateRandom();

                                    printf("\nNow processing the chosen file name %s\n", entryName); 

                                    // Create new directory and process the list
                                    sprintf(newDirName, "hurstd.movies.%d", randNum);
                                    mkdir(newDirName, 0750); 
                                    printf("Created directory with name %s\n", newDirName);

                                    list = processFile(entryName); 
                                    createYearFiles(list, newDirName);
                                }
                            }

                            // If there are no matching files, print error message
                            if (matchingFiles == 0) {
                                printf("The file %s was not found. Try again.\n", entryName);
                            }


                            break;
                        default:
                            printf("You entered an invalid choice. Please try again \n\n");
                    }
                } while(fileChoice != 1 && fileChoice != 2 && (fileChoice != 3 || matchingFiles == 0));

                break;
            // Exit the program
            case 2:
                break;
            default:
                printf("You entered an invalid choice. Please try again. \n\n");
                break;
            }
            // Close current directory
            closedir(currDir);
        
        } while (choice != 2);

    
    
    return EXIT_SUCCESS;
}
