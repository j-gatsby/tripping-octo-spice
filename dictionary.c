/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>

#include "dictionary.h"

unsigned int count;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // TODO
    // create data structures
    
    // open dictionary file
    FILE* dptr = fopen("DICTIONARY", "r");
    
    // ensure it worked
    if (dptr == NULL)
    {
    	fclose(dptr);
    	printf("Unable to open DICTIONARY file\n");
    	return 2;
    }
    
    // iterate over dictionary file
    
    	// malloc new node* n for each word
    	
    	// use fscanf to read in one word at a time 
    
    	// hash n->word
    	
    	// insert node into hash table
    	
    	// counter for words inserted
    	count++;
    	
    // fclose dictionary file
    return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return count;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // TODO
    return false;
}
