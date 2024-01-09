#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

struct Person {
    string name;
    string propositions[5];
    string stringPrefs[5];
    int rejectNum = 0;
    bool onHold = false;
    bool proposedTo = false;

};

Person *read_file(string filename, string type);
int indexOf(Person person, string people[]);
void propose(Person person, Person people[]);
void calcMatches(Person men[], Person women[]);
bool done(Person people[]);
void propose(Person person, int index, Person women[]);
void reject(Person women[], Person men[]);
int getPreference(Person woman);
int indexOfString(string array[], string name);
void print(Person women[]);

/*
 * name:      main
 * purpose:   creates an array of Person struct pointers based on the file provided
 *            and initiates the algorithm by calling calcMatches
 * arguments: an int with number of arguments and an array with the arguments 
 * returns:   an int of whether the program finished successfully 
 * effects:   prints error if client did not provide a filename or if the file does
 *            not exist.
*/
int main(int argc, char *argv[]){

    if (argc < 2) {
        cerr << "ERROR: No filename provided as an argument" << endl;
        exit(EXIT_FAILURE);
    }

    Person *men = read_file(argv[1], "m");
    Person *women = read_file(argv[1], "w");

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            women[i].propositions[j] = "Nobody";
        }
    }

    calcMatches(men, women);

    
    delete [] men;
    delete [] women;

    return 0;
}


/* read_file
* Input:       string containing user-inputed filename and a string which
*              either contains 'w' or 'm' corresponding to whether to initialize
*              male data or female data
*              
* Description: Opens and reads the user provided file and creates and
*              initializes a pointer array of Person structs initialized based
*              on the file
*
* Output:      Returns pointer array with corresponding data from file
*/
Person *read_file(string filename, string type){
    
    ifstream infile(filename);
    if (infile.fail()) {
        cout << "ERROR OPENING FILE: Exiting Program" << endl;
        exit(EXIT_FAILURE);
    }

    Person *personArray = new Person[5];
    string skip;

    infile >> skip >> skip >> skip >> skip;

    if(type == "m"){
    for(int i = 0; i < 5; i++){
        infile >> personArray[i].name;
        for(int j = 0; j < 5; j++){
            infile >> personArray[i].stringPrefs[j];
        }
    }
    }
    if(type == "w"){
    
    for(int i = 0; i < 34; i++){
        infile >> skip;
    }

    for(int i = 0; i < 5; i++){
        infile >> personArray[i].name;
            for(int j = 0; j < 5; j++){
                infile >> personArray[i].stringPrefs[j];
            }
        }
    }

    return personArray;
}


/* calcMatches
* Input:       pointer array of male and female Person structs
*              
* Description: Executes Gale-Shapley algorithm with correct steps
*
* Output:      Does not return anything
*/
void calcMatches(Person men[], Person women[]){
   
    while(!done(women)){
        
        for(int i = 0; i < 5; i++){
            propose(men[i], i, women);
        }
        
        print(women);
        reject(women, men);
        print(women);

    }
    
}



/* indexOf
* Input:       pointer array of Person stucts and a string containing a name of
*              a person specified in the file
*              
* Description: Finds the struct in the array provided with the specified name
*              and returns its index in the array
*
* Output:      Returns index of corresponding struct in people[]
*/
int indexOf(Person people[], string name){

    for(int i = 0 ; i < 5; i++){
        if(people[i].name == name){
            return i;
        }
    }

    return -1;
}


/* done
* Input:       pointer array of Person structs corresponding to the female data
*              
* Description: Determines whether the algorithm has terminated based on the
*              number of women which have been proposed to
*
* Output:      returns boolean corresponding to whether the algortihm has
*              terminated
*/
bool done(Person women[]){
    
    int totalProposed = 0;
    
    for(int i = 0; i < 5; i++){
        if(women[i].proposedTo){
            totalProposed++;
        }
   
    }
    return totalProposed == 5;
}


/* propose
* Input:       male Person struct which is proposing, the man's index
*              in the main male Person struct pointer array and the pointer
*              array of struct Person corresponding with female data
*              
* Description: Executes the proposal step of the GS algorithm: each man proposes
*              to their highest choice which has not yet rejected them
*
* Output:      Does not return anything
*/
void propose(Person person, int index, Person women[]){

   if(!person.onHold && person.rejectNum < 5){ 

        string nameOfPreference = person.stringPrefs[person.rejectNum];
        int indexOfPreference = indexOf(women, nameOfPreference);
        women[indexOfPreference].propositions[index] = person.name;
        women[indexOfPreference].proposedTo = true; 

   }
}



/* reject
* Input:       pointer array of male and female Person structs
*              
* Description: Executes the reject step of the GS algorithm: every female
*              rejects everyone except for their most favorable man which
*              proposed to them
*
* Output:      Does not return anything
*/
void reject(Person women[], Person men[]){
    int indexOfPreffered;
    for(int i = 0; i < 5; i++){

        indexOfPreffered = getPreference(women[i]);

        if(indexOfPreffered != -1){

        men[indexOfPreffered].onHold = true;

        for(int j = 0; j < 5; j++){
            if(j != indexOfPreffered && women[i].propositions[j] != "Nobody"){
                men[j].rejectNum++;
                men[j].onHold = false;
                women[i].propositions[j] = "Nobody";
            }
        }
    }
    }
}


/* getPreference
* Input:       Person struct from the female Person struct pointer array
*              
* Description: Determines which proposal is most favorable for the woman
*              specified and returns the corresponding index of the most
*              favorable man in the woman's propositions[] (which stores the
*              proposals each woman recieved)
*
* Output:      Returns index corresponding to the most favorable proposal
*              in the specified woman's propositions[]
*/
int getPreference(Person woman){
    
    int nobodyCounter = 0;
    for(int i = 0; i < 5; i++){
        if(woman.propositions[i] == "Nobody"){
            nobodyCounter++;
        }
    }

    if(nobodyCounter == 5 || nobodyCounter == 4){
        return -1;
    }

    int index;
    int lowest = 4;

    for(int i = 0; i < 5; i++){

        if(indexOfString(woman.stringPrefs, woman.propositions[i]) < lowest 
            && indexOfString(woman.stringPrefs, woman.propositions[i]) != -1){
            
            index = i;
            lowest = indexOfString(woman.stringPrefs, woman.propositions[i]);
        
        }
    }

    return index;
}

/* indexOfString
* Input:       Array of strings and string
*              
* Description: Determines the index at which a specified string exists in a
*              specified string array
*
* Output:      Returns the index corresponding to the instance of the string
*              in the string array
*/
int indexOfString(string array[], string name){

    for(int i = 0 ; i < 5; i++){
        if(array[i] == name){
            return i;
        }
    }
    return -1;
}

/* print
* Input:       Pointer array of female Person structs
*              
* Description: Prints each couple
*
* Output:      Does not return anything
*/
void print(Person women[]){
cout << "-----------------" << endl;
for(int i = 0; i < 5; i++){
        cout << "\n\n" << women[i].name << ":" << endl;
        for(int j = 0; j < 5; j++){
            if(women[i].propositions[j] != "Nobody"){
                cout << women[i].propositions[j] << endl;
            }
        }
        cout << "\n\n\n";
    }
}
