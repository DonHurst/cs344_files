#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char* argv[]) {

    // Declare variable for switch statement choice
    int choice;
    // Create int for next choice
    int fileChoice;

    do {
        // Print menu and take user choice
        printMenu();
        scanf("%d", &choice);
            
        switch(choice) {
            
            // Open submenu
            case 1:
                
                

                // Print menu and take choice
                printFileMenu();
                scanf("%d", &fileChoice);

                do {
                    switch(fileChoice) {

                        case 1:

                            break;

                        case 2:

                            break;

                        case 3:
                            break;

                        default:
                            printf("You entered an invalid choice. Please try again \n\n");
                    }
                } while(fileChoice != 1 && fileChoice != 2 && fileChoice != 3);


                break;
            // Exit the program
            case 2:
                break;
            default:
                printf("You entered an invalid choice. Please try again. \n\n");
                break;
            }
        
        } while (choice != 2);
    
    return EXIT_SUCCESS;
}
