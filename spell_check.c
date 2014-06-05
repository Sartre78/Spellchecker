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
// declare hash function
int hash (char* string);

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

        // print check statement - to be removed TODO
        printf("%s.\n", v);

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

    // "For" loop to print out each array spot
    for (int i = 25; i >= 0; i--) 
    {
        // temp node that equals the beginning of the "i"th spot in the table                             
        node* current = table[i];

        // "While" loop until "word" equals NULL
        while (current->word != NULL) 
        {
            printf("%s.\n", current->word); // print word 
            current = current->next; // update pointer 
        }         
    }
}
  
/* basic hash function - takes the first letter of each string and returns a 
   numerical value (a = 0, b = 1, c = 2, etc.) */      
    
int hash (char* string) {int h = toupper(string[0]) - 'A'; return h % 26;}
