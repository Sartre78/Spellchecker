/**
 * 06/05/2014  
 *
 * spell_check.c
 *
 * practice for pset6 - CS50x
 *
 * This program loads a text file identical to the large dictionary
 * used in pset6, and then accesses the dictionary to print out the 
 * dictionary.  The dictionary is stored utilizing a hash table with
 * separate chaining (linked lists).
 *
 */
#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define LENGTH 45

// define node.  Arbitrarily setting max word length to 50 char.
typedef struct node {char word[LENGTH+1]; struct node* next;} node;


// initialize table array
node* table[26];
// function declarations
int hash (char* string);
void print_all (void);
void print_i (int i); 
void check(char* w);
void free_table (void);
// main 
int main (void)
{
    // Open dictionary    
    FILE* fp = fopen("large_dictionary1.txt", "r");
    if (fp == NULL) {printf("Could not open dictionary.\n"); return false;}

   // prepare to spell-check
    int index = 0, words = 0;
    char word[LENGTH+1];

    // spell-check each word in text
    for (int c = fgetc(fp); c != EOF; c = fgetc(fp))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c; index++;
            
            // ignore alphabetical strings too long to be words
            // consume remainder of alphabetical string; prepare for new word
            if (index > LENGTH) {while ((c = fgetc(fp)) != EOF && isalpha(c)); index = 0;}
        }
        // ignore words with numbers (like MS Word can)
        // consume remainder of alphanumeric string; prepare for new word
        else if (isdigit(c)) {while ((c = fgetc(fp)) != EOF && isalnum(c)); index = 0;}

        // we must have found a whole word.  terminate word and update counter
        else if (index > 0) {word[index] = '\0'; words++; index = 0;}
    }
    printf("\n\n Total words in dictionary %i. \n\n", words);
    fclose(fp);
    // begin loop to read words from file.  
    
    // Open dictionary    
    FILE* file = fopen("large_dictionary1.txt", "r");
    if (file == NULL) {printf("Could not open dictionary.\n"); return false;}
    
    for (int i = 0; i < words; i++)
    {          
        // allocate new node and check for NULL
        node* new_node = malloc(sizeof(node)); {if (new_node == NULL) return false;}
    
        // scan word from file into the "word" portion of the new node struct
        fscanf(file, "%s", new_node->word);
        
        // create a string (char*) to be hashed 
        char* v = new_node->word;

        // retrieve hash key by hashing the current word
        int k = hash (v);

        // initial insertion scenario (i.e., the spot in the table array is empty)
        if (table[k] == NULL)  {table[k] = new_node;}
    
        /* otherwise, put the new string in the first position and link the
           previous node to the new head of the list */
        else 
        {
            // create a node equal to the current value of the table
            node* curr = table[k];  
            /* point "new_node" (the struct holding the value of the desired
               word to be inserted) to the newly created node that equals what
               to this point was the start of the list */ 
            new_node->next = curr;
            /* set the head pointer of the "k"th spot in the array (i.e., the 
               "head") to be the node containing the desired string to be 
               inserted.  */ 
            table[k] = new_node;
        }        
    }
    printf(".\n\n\n");

    // prompt the user to print dictionary
    
    // switch case
    while (true)
    {
        printf("\nPlease enter a valid option\n"); 
        printf("\n 1 - Quit\n"); 
        printf("\n 2 - Print a letter of the dictionary\n");
        printf("\n 3 - Print the full dictionary\n");
        printf("\n 4 - Check to see if a specific word is in the dictionary\n\n");
        int option = GetInt();

        switch (option)
        {
            // quit
            case 1: free_table(); printf("\nGoodbye!\n\n"); fclose(file); return 0;
            
            // print an individual section ("Letter") of the dictionary
            case 2: printf("\nWhich letter of the dictionary would you like to print?\n\n"); // prompt user
                    char* c = GetString(); // get string from user
                    int alpha = c[0];
                    if (isalpha(alpha) == 0) {printf("\nNot a letter!\n"); break;}
                    int h = hash(c); // determine hash key
                    print_i(h); free(c); break; // print table for letter & free memory

            // print dictionary
            case 3: print_all(); break;
            
            case 4: printf("\nWhich word would you like to check?\n\n"); // prompt user
                    char* w = GetString(); // get string from user
                    // check the first character of the word to avoid seg fault                   
                    if (isalpha(w[0]) == 0) {printf("\nThat's not a word!\n"); free(w); break;}
                    check(w); free(w); break; // check word and free memory   

            default: printf("Not a valid option.\n"); break;
        }   
    }  
} 

/**
 *
 * print the entire Dictionary, along with headers and word
 * counts for each letter, and a total word count
 *
 */
 
void print_all (void) 
{
    int word_counter = 0; // initialize total word counter
    
    printf("\nDictionary - Complete:\n\n"); // print header
    
    for (int i = 0; i <= 25; i++) // for loop to access hash table
    {
        int letter = i + 65; // determine current ASCII letter value
        int letter_counter = 0; // initialize current letter counter
        printf("Dictionary - Letter: %c\n\n", (char) letter); // print header
        node* current = table[i]; // set temporary node to the head of list
        while (current->word != NULL) // while loop until end of list
        {
            printf("%s.\n", current->word); // print word
            current = current->next; // move to next word
            word_counter++; letter_counter++; // update counters
        }
        // print word counter for each letter
        printf("\nTotal Letter %c word count: %i.\n\n", (char) letter, letter_counter);
        free(current);  // free temporary node
    }
    // print total word count for dictionary
    printf("Total Dictionary Word Count: %i.\n\n", word_counter);
}

/**
 *
 * prints all the words of a given letter of the dictionary.
 * also prints out the total number of words for the given letter.
 *
 */
 
void print_i (int i)
{
    // check to see if the dictionary has been loaded and respond accordingly
    if (table[i] == NULL) {printf("Dictionary has not been loaded yet!\n\n");}
    
    int word_counter = 0; // initialize word counter
    
    node* current = table[i]; // set temporary node to head of the list
    
    int letter = i + 65; // obtain ASCII letter (Capitial)
    
    printf("\nDictionary - Letter: %c\n\n", (char) letter); // print header
    
    while (current->word != NULL) // while loop until end of list
    {
        printf("%s.\n", current->word); // print current word
        current = current->next; // move to next word
        word_counter++; // increase counter
    }
    // print final word count 
    printf("\nTotal Letter %c word count: %i.\n\n", (char) letter, word_counter);
    free(current); // free temporary node       
}

void check(char* w)
{
    // allocate memory for a string that will contain the unadulterated word
    char* original = malloc((strlen(w) + 1) * sizeof(char));
    if (original == NULL) {free(w);} // check for NULL
    
    // convert word to lowercase to compare it to the all lower-care dictionary
    for (int i = 0, n = strlen(w); i < n; i++) {original[i] = w[i]; w[i] = tolower(w[i]);}
    
    int h = hash(w); // hash word to determine hash table access key
    
    node* current = table[h]; // set temporary node to head of list
    
    while (current->word != NULL && w != NULL) // NULL check and loop until end of list
    { 
        // Scenario 1:  Word matches current word in list   
        if (strcmp(w,current->word) == 0) {printf("\n%s is in the Dictionary!\n\n", original); break;}
        /* Scenario 2: Word does not match current word.
           Apocryphal Buddhist and/or Confucian proverb:
           "To advace further, look within" */
        else {current = current->next;}
    }
    // Inform user that word was not in the dictionary            
    if (current->word == NULL) {printf("\n%s was not in the Dictionary.\n\n", original);}
    free(original); // free string containing the originally requested word
}

  
/* basic hash function - takes the first letter of each string and returns a 
   numerical value (a = 0, b = 1, c = 2, etc.) */   
/**
 *
 *frees table
 *
 */
 
void free_table (void)
{
    // free every "i"th spot of the table
    for (int i = 0; i <= 25; i++) {free(table[i]);}
}      
    
int hash (char* string) {int h = toupper(string[0]) - 'A'; return h % 26;}
