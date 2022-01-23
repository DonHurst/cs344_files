#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PREFIX "movies_"
#define EXTENSION ".csv"
#define ONID "hurstd"
#define MOVIES "movies"

// struct for movie information
struct movie {
    char* title;
    int year;
    char* languages;
    double rating_value;
    struct movie *next;
};

/* -------------------------------------------------------------------------------------------------------------------------
    This function creates a movie structure. It takes the current line of data from the file as an argument
    The function allocates a new movie structure, then uses a pointer and strtok_r to parse the lines for information.
    Since the data is comma separated and in the same order, the code stores the data from the line into the structure
    as follows: title, year, languages, rating. It also takes the string values for the year/rating and converts them
    to int/double respectively. Finally, the function points to NULL and returns the newly created movie struct.
---------------------------------------------------------------------------------------------------------------------------- */
struct movie *createMovie(char *currLine) {
    // Citation for the following function:
    // Date: 12/08/2022
    // Copied/Adapted from 
    // Source: main.c (OSU CS 344 student sample)
    
    // Allocate the memory for the movie
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r (extracts token from strings)
    char* saveptr;

    // First token is the title of the movie
    char* token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // Next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    int tokenInt = atoi(token);
    currMovie->year = tokenInt;

    // Next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // Next token is the rating value
    token = strtok_r(NULL,"\n", &saveptr);
    double tokenDouble = atof(token);
    currMovie->rating_value = tokenDouble;

    // Set the next node to null
    currMovie->next = NULL;

    // Return our movie
    return currMovie;
}

/* -------------------------------------------------------------------------------------------------------------------------
    This function processes the file, reading line-by-line and creating movie nodes with the data. It takes the filepath
    as an argument and uses that to open the file. With the file open, it uses getline to read line-by-line, calling the 
    create movie function on each line (excluding the header) to create new movie nodes from the data. It then checks to see
    if it's the first node. If it is, it'll set the head and tail to itself. If not, it'll set the new node as the tail.
    Finally, it will return the head of the linked list containing each of the movie nodes.
---------------------------------------------------------------------------------------------------------------------------- */
struct movie *processFile(char *filePath) {
    // Citation for the following function:
    // Date: 12/08/2022
    // Copied/Adapted from 
    // Source: main.c (OSU CS 344 student sample)

    // Open the specified file for reading
    FILE *movieFile = fopen(filePath, "r");

    char * currLine = NULL;
    size_t len = 0;
    ssize_t nread;
    int line_count = 0;

    // Set head and tail of the linked list
    struct movie* head = NULL;
    struct movie* tail = NULL;

    // Read the file line by line
    while((nread = getline(&currLine, &len, movieFile)) != -1) {
        
        // if the first line in the file (header line), move to next line
        if (line_count == 0) {
            line_count += 1;
            continue;
        }

        // Get a new movie node on the current line
        struct movie *newNode = createMovie(currLine);

        // Check if this is the first node in linked list
        if (head == NULL) {
            // If so, set head and tail of linked list to current node
            head = newNode;
            tail = newNode;
        }
        else {
            // If not, add node to the list and advance the tail
            tail->next = newNode;
            tail = newNode;
        }
        line_count += 1;        
    }
    // Free memory, close the file, and return the head of the linked list
    free(currLine);
    fclose(movieFile);
    return head;
}

/* -------------------------------------------------------------------------------------------------------------------------
This function takes a list of movies and the name of a directory as parameters. It steps through each value in
the list and writes the title to a corresponding year file (If no file exists, it creates one).
---------------------------------------------------------------------------------------------------------------------------- */
void createYearFiles(struct movie *list, char *dirName) {
    // Instantiate a file descriptor
    int file_descriptor;

    // Step through all values in the linked list
    while (list != NULL) {

        // Create string for the new file path
        char newFilePath[255];
        sprintf(newFilePath, "./%s/%d.txt", dirName, list->year);

        // Create string for the title to be written to the file
        char titleToWrite[256];
        sprintf(titleToWrite, "%s\n", list->title);

        // Create or open file for the year and write title to it
        file_descriptor = open(newFilePath, O_RDWR | O_CREAT | O_APPEND, 0640);
        write(file_descriptor, titleToWrite, strlen(titleToWrite));
        
        // Close the file
        close(file_descriptor);

        // Move to next node in the linked list
        list = list->next;
    }
}

/*------------------------------------------------------------------
This function takes no parameters and returns nothing. It's used to 
print out menu options for the user
-------------------------------------------------------------------*/
void printMenu () {
    printf("\n1. Select file to process\n");
    printf("2. Exit from the program\n");
    printf("Enter a choice 1 or 2: ");
}

/*-------------------------------------------------------------------
This function takes no parameters and returns nothing. It's used to 
print out menu options for the user
-------------------------------------------------------------------*/
void printFileMenu() {
    printf("\nWhich file do you want to process?\n");
    printf("Enter 1 to pick the largest file\n");
    printf("Enter 2 to pick the smallest file\n");
    printf("Enter 3 to specify the name of a file\n");
    printf("\nEnter a choice from 1 to 3: ");
}


/*-------------------------------------------------------------------
This function takes no parameters and returns a random integer.
This code built with the help of the man pages provided in the course
-------------------------------------------------------------------*/
int generateRandom() {
    time_t t;
    srandom((unsigned) time(&t));
    return random() % 9999;
}

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
