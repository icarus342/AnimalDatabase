/* 
 * Justin Arnett
 * jarnett@uw.edu
 * TCSS 333
 * Homework 5
 */


/*
- Extra Credit -
Added option 6 to the main menu. This will open the default
web browser and google search the species of an animal. The
animal is chosen based off of input of a valid id from the
user.

Figuring out how to pull this off took more effort than I
initially thought it would. I experimented with different
system commands for opening a webpage, but they were very
dependant on the system being set up a certain way. I finally
came across xdg-open that will open the default program of
the system. Next, I had to do string manipulations to be able
to do a system call that involved a variable. Finally, xdg-open
was plagueing the terminal output with useless information 
while the webpage was open, hijacking the terminal from the
original program. I managed to figure out (from a previous lab)
to redirect the stdout of xdg-open to a garbage file. 

I also tried making the interface as easy to follow as possible.
*/



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "animals.h"

#define DATA_FILE "./animals.dat"
#define OUTPUT_CSV "animals.csv"



// Gets the main menu input option from the user.
// Post: The user's input option.
int getUserChoice() {
   int temp;
   int test;

   // The main menu items.
   printf("\n\n+-------------------+");
   printf("\n|     Main Menu     |");
   printf("\n+-------------------+");
   printf("\n(1) Add an animal.");
   printf("\n(2) Delete an animal.");
   printf("\n(3) Update animal's info.");
   printf("\n(4) Search for an animal.");
   printf("\n(5) Output database to \"animals.csv\".");
   printf("\n(6) Open species webpage.");
   printf("\n(0) quit.");
   printf("\n\nPlease enter option number: ");
   test = scanf("%d", &temp);

   // Error handling.
   while (test == 0) {
      printf("Please enter an integer.  ");
      scanf("%*s");
      test = scanf("%d", &temp);
   }
   return temp;
} // end of getUserChoice()




// Asks the user to input the ID of the animal.
// Post: The ID number of the animal.
int userInputID() {
   int temp, test;

   printf("\n\n  Please enter ID of animal: ");
   test = scanf("%d", &temp);
   // Error handling.
   while (test == 0) {
      printf("Please enter an integer.  ");
      scanf("%*s");
      test = scanf("%d", &temp);
   }
   return temp;

} // end of userInputID()




// Main driver of program. Opens data file and handles
// user's option choices.
int main(void) {
   
   FILE *inFile;
   struct animal myAnimal;
   int userChoice = -1;

   inFile = fopen(DATA_FILE, "rb+");
   if (inFile == NULL) {
      printf("\nERROR Opening file %s\n", DATA_FILE);
      exit(0);
   }


   while (userChoice != 0) {
      userChoice = getUserChoice();
      struct animal myAnimal;
      
      switch(userChoice) {
         case 0 :
            printf("\nexiting...\n");
            exit(0);
            break;
         case 1 :
            addAnimal(inFile);
            break;
         case 2 :
            deleteAnimal(inFile, userInputID());
            break;
         case 3 :
            updateAnimal(inFile, userInputID());
            break;
         case 4 :
            printAnimal(inFile, userInputID());
            break;
         case 5 :
            outputDataToCSV(inFile, OUTPUT_CSV);
            break;
         case 6 :
            openSpeciesWebpage(inFile, userInputID());
            break;
         default :
            printf("\nNot a listed option.");
      }
   }

   
   fclose(inFile);

} // end of main()


