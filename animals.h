#ifndef _ANIMALS_H_
#define _ANIMALS_H_



# pragma pack(2)
struct animal {
   short int id;
   char name[20];
   char species[35];
   short int age;
   double weight;
};
# pragma pack()


int getEntryAmount(FILE *theFile);
int isValidID(FILE *theFile, int theID);
struct animal getAnimal(FILE *theFile, int theID);
int printAnimal(FILE *theFile, int theID);
int deleteAnimal(FILE *theFile, int theID);
struct animal createAnimal(short int newID);
void addAnimal(FILE *theFile);
int updateAnimal(FILE *theFile, int theID);
void outputDataToCSV(FILE *theFile, char *fileName);
int openSpeciesWebpage(FILE *theFile, int theID);

#endif
