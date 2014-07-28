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

unsigned int count = 0;
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
    typedef struct node
    {
    	char word[LENGTH + 1];
    	struct node *next;
    }node;
    
    // open dictionary file
    FILE* dptr = fopen("DICTIONARY", "r");
    
    // ensure it worked
    if (dptr == NULL)
    {
    	return false;
    }
    
    // check number of words in dptr
    while (!feof(dptr))
    {
    	char temp = fgetc(dptr);
    	if (temp == '\n')
    	{
    		count++;
    	}
    }
    
    // reset file position indicator and eof flag
    fseek(dptr, 0, SEEK_SET);
    
    // initialize hash table
    node *hashtable[count];
    
    // set each element in hashtable to NULL
    for (int i = 0; i < count; i++)
    {
    	hashtable[i] = NULL;
    }
    
    // iterate over dictionary file
    while (!feof(dptr))
	{
	    // malloc new node* n for each word
    	
    	// use fscanf to read in one word at a time 
    
    	// hash n->word
    	
    	// insert node into hash table
    	
    }
    
    // fclose dictionary file
    fclose(dptr);
    
    // set bool to true
    return true;
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
