/* 
 * Justin Arnett
 * jarnett@uw.edu
 * TCSS 333
 * Homework 5
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "animals.h"

#define AGE_ADDRESS 58

/* MOVED TO HEADER
# pragma pack(2)
struct animal {
   short int id;
   char name[20];
   char species[35];
   short int age;
   double weight;
};
# pragma pack()
*/



// Finds the amount of possible records already in
// the data file, including deleted records.
//
// Pre: The data file.
// Post: The amount of records in the data file.
int getEntryAmount(/*in*/ FILE *theFile) {
   fseek(theFile, 0, SEEK_END);
   return (ftell(theFile) - sizeof(int)) / sizeof(struct animal);

} // end of getEntryAmount()





// Checks if the ID is a valid ID in the data file.
// An ID is considered valid if it is not outside the
// bounds of the total amount of records, and it is
// not an entry with a negative age value (deleted).
//
// Pre: The data file and the ID key.
// Post: returns 1 if ID is valid, -1 otherwise.
int isValidID(/*in*/ FILE *theFile, /*in*/ int theID) {
  
   short int age;

   // If ID is out of bounds.
   if (theID > getEntryAmount(theFile)) {
      return -1;
   }   

   // Change file pointer to where animal.age is of theID.
   fseek(theFile, 
         sizeof(int) + ((theID-1) * sizeof(struct animal)) + AGE_ADDRESS,
         SEEK_SET);
   fread(&age, sizeof(short int), 1, theFile);

   // If age is a negative, indicated a deleted record.
   if (age < 0) {
      return -1;
   }
   return 1;

} // end isValidID()





// Gets the animal structure from the data file with
// the provided ID key.
//
// Pre: The data file and the ID key.
// Post: The animal structure found in the data file.
struct animal getAnimal(/*in*/ FILE *theFile,/*in*/ int theID) {

   struct animal temp;

   fseek(theFile, sizeof(int) + (sizeof(struct animal) * (theID-1)), SEEK_SET);
   fread(&temp, sizeof(struct animal), 1, theFile);
   
   return temp;

} // end getAnimal()





// Prints the animal's info to the console.
// 
// Pre: The data file and the ID key.
// Post: Returns a 1 if ID was valid, -1 otherwise.
int printAnimal(/*in*/ FILE *theFile, /*in*/ int theID) {

   if (isValidID(theFile, theID) < 1) {
      printf("\n-- ID is not valid --");
      return -1;
   }

   struct animal myAnimal = getAnimal(theFile, theID);

   printf("\n+-----------------------------+");
   printf("\n| ID:       %hd", myAnimal.id);
   printf("\n| Name:     %s", myAnimal.name);
   printf("\n| Species:  %s", myAnimal.species);
   printf("\n| Age:      %hd", myAnimal.age);
   printf("\n| Weight:   %.1lf", myAnimal.weight);
   printf("\n+-----------------------------+\n");

   return 1;
} // end of printAnimal()





// Deletes an animal record by setting the age
// value to -1.
//
// Pre: The data file and the ID key.
// Post: Returns a 1 if ID was valid, -1 otherwise.
int deleteAnimal(/*in*/ FILE *theFile,/*in*/ int theID) {

   struct animal myAnimal = getAnimal(theFile, theID);
   int holes;

   if (isValidID(theFile, theID) < 1) {
      printf("\n-- ID is not valid --");
      return -1;
   }

   myAnimal.age = -1;

   fseek(theFile, sizeof(int) + (sizeof(struct animal) * (theID-1)), SEEK_SET);
   fwrite(&myAnimal, sizeof(struct animal), 1, theFile);

   printf("\n  ** Deleted %hd (%s) **\n", myAnimal.id, myAnimal.name);

   // increments the amount of holes in the file.
   fseek(theFile, 0, SEEK_SET);
   fread(&holes, sizeof(int), 1, theFile);
   holes = holes + 1;
   fseek(theFile, 0, SEEK_SET);
   fwrite(&holes, sizeof(int), 1, theFile);

   //printf("\n  ** DEBUG ** Holes: %d", holes);

   return 1;
   
} // end of deleteAnimal()





// Creates a new animal. Asks the user for the name,
// species, age and weight. ID will be provided.
//
// Pre: The ID of the animal.
// Post: Newly created animal structure.
struct animal createAnimal(/*in*/ short int newID) {
   struct animal myAnimal;

   printf("\nID number is %hd.", newID);
   myAnimal.id = newID;

   printf("\nEnter the name:     ");
   scanf(" %[^\n]s", myAnimal.name);
   
   printf("Enter the species:  ");
   scanf(" %[^\n]s", myAnimal.species);

   printf("Enter the age:      ");
   scanf("%hd", &myAnimal.age);

   printf("Enter the weight:   ");
   scanf("%lf", &myAnimal.weight);

   return myAnimal;

} // end of createAnimal()





// Adds a new animal structure to the data file. The
// ID key is chosen based on availability in the data
// file. If the first integer of the data file is a 
// zero, there are no deleted records left in the data 
// structure, a new one will be appended to the end of 
// the data file. If the first integer is instead 
// greater than zero, the first found deleted record 
// will be replaced with the new animal structure.
//
// Pre: The data file.
void addAnimal(/*in*/ FILE *theFile) {

   int i = 0; 
   int holes;
   short int id = -1;
   struct animal myAnimal;

   // Gets the first integer of the data file.
   fseek(theFile, 0, SEEK_SET);
   fread(&holes, sizeof(int), 1, theFile);

   
   if (holes == 0) {
      // If first integer == 0, append new entry.
      id = getEntryAmount(theFile) + 1;
   } else {
      // else, find first deleted record by finding
      // first invalid ID.
      while (id < 0) {
         i++;
         if (isValidID(theFile, i) < 1) {
            id = i;
         }
      }
      // decrements size of holes then writes to file.
      holes = holes - 1;
      fseek(theFile, 0, SEEK_SET);
      fwrite(&holes, sizeof(int), 1, theFile);
   }

   // writes animal to data file.
   myAnimal = createAnimal(id);
   fseek(theFile, sizeof(int) + (sizeof(struct animal) * (id-1)), SEEK_SET);
   fwrite(&myAnimal, sizeof(struct animal), 1, theFile);

} // end of addAnimal()




// Displays current animal's info then asks user to
// "modify" the current animal's record.
//
// Pre: The data file and the ID key.
// Post: returns a 1 if ID is valid, -1 otherwise.
int updateAnimal(/*in*/ FILE *theFile, /*in*/ int theID) {

   struct animal myAnimal;

   if (isValidID(theFile, theID) < 1) {
      printf("\n-- ID is not valid --");
      return -1;
   }

   printf("\n  ** MODIFYING **");
   printAnimal(theFile, theID);

   // creates animal then writes it to data file.
   myAnimal = createAnimal(theID);
   fseek(theFile, sizeof(int) + (sizeof(struct animal) * (theID-1)), SEEK_SET);
   fwrite(&myAnimal, sizeof(struct animal), 1, theFile);

   return 1;

} // end updateAnimal()





// Outputs all valid records in the data file to a new
// ascii file separated by id,name,species,age,weight.
//
// Pre: The data file and the name of the ascii file.
void outputDataToCSV(/*in*/ FILE *theFile, /*in*/ char *fileName) {

   int size = getEntryAmount(theFile);
   struct animal myAnimal;
   FILE *outFile = fopen(fileName, "w+");
   int i;
   

   for (i = 1; i <= size; i++) {
      // if record is valid, print to ascii file.
      if (isValidID(theFile, i) > 0) {
         myAnimal = getAnimal(theFile, i);
         fprintf(outFile, "%hd,%s,%s,%hd,%.1lf\n",
                          myAnimal.id,
                          myAnimal.name,
                          myAnimal.species,
                          myAnimal.age,
                          myAnimal.weight);
      }
   }

   printf(" ** %s file was created. **", fileName);
   fclose(outFile);
   
} // end of outputDataToCSV()




// Opens a search engine webpage in the default browser 
// and search for the animal's species.
//
// Pre: The data file and the ID key.
// Post: Returns 1 if ID was valid, -1 otherwise.
int openSpeciesWebpage(/*in*/ FILE *theFile, /*in*/ int theID) {

   struct animal myAnimal;
   char command[50] = "xdg-open https://www.google.com/#q=";
   char tail[30] = " > ./garbage 2> ./garbage";
   char *cmdline;
   char *strSpecies;
   int i, size;

   // error check for valid ID.
   if (isValidID(theFile, theID) < 1) {
      printf("\n-- ID is not valid --");
      return -1;
   }

   myAnimal = getAnimal(theFile, theID);

   cmdline = malloc(strlen(command) + strlen(myAnimal.species) + strlen(tail) + 1);
   strSpecies = malloc(strlen(myAnimal.species) + 1);

   // replaces whitespace with '+' in species name.
   size = strlen(myAnimal.species);
   for (i = 0; i < (size+1); i++) {
      if (myAnimal.species[i] == ' ') {
         strSpecies[i] = '+';
      } else {
         strSpecies[i] = myAnimal.species[i];
      }
   }
   //printf("\n  ** DEBUG **  theSpecies: %s", myAnimal.species);
   //printf("\n  ** DEBUG **  strSpecies: %s", strSpecies);

   // assemble command line strings.
   strcpy(cmdline, command);
   strcat(cmdline, strSpecies);
   strcat(cmdline, tail);

   printAnimal(theFile, theID);
   printf("  Opening webpage...");
   system(cmdline);
   
   //printf("\n  ** DEBUG **  \"%s\"  ", cmdline);

   free(strSpecies);
   free(cmdline);

   return 1;

} // end openSpeciesWebpage()





