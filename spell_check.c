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


// define node.  Arbitrarily setting max word length to 50 char.
typedef struct node {char word[50]; struct node* next;} node;

// initialize table array
node* table[26];
// declare hash and print all functions
int hash (char* string);
void print_all (void); 

// main 
int main (void)
{
    // Open dictionary    
    FILE* file = fopen("large_dictionary.txt", "r");

    /* begin loop to read words from file.  "143091" is being hard coded in as 
       the known value of total words in the file. */
    for (int i = 0; i < 143091; i++)
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
        printf("\nWould you like to print the dictionary?\n"); 
        printf("\n 1 - Yes \n 2 - No\n"); int option = GetInt();

        switch (option)
        {
            // quit
            case 1: print_all(); return 0;

            // print dictionary
            case 2: printf("Goodbye!\n"); return 0;

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


  
/* basic hash function - takes the first letter of each string and returns a 
   numerical value (a = 0, b = 1, c = 2, etc.) */      
    
int hash (char* string) {int h = toupper(string[0]) - 'A'; return h % 26;}
