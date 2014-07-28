/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "dictionary.h"

unsigned int count = 0;

unsigned int hash(const char* word)
{
	unsigned int hashValue = 0;
	char* string = tolower(word);
	
	for(; *string != '\0'; string++) 
	{
		hashval = *string + (hashValue << 5) - hashValue;
	}

    /* we then return the hash value mod the hashtable size so that it will
     * fit into the necessary range
     */
    	hashResult = hashValue % count;
    	return hashResult;
}
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // TODO
    char *string = tolower(word);
    
    if(hashtable[hash(string)] == NULL)
    {
    	return false;
    }
    else
    
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
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
    	node new_node=malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    		return false;
    	}
    	
    	// use fscanf to read in one word at a time 
    	fscanf(dptr, "%s", new_node->word);
    	
    	// convert word tolower
    	new_node->word = tolower(new_node->word);
    	
    	// declare index variable ??? and set to NULL for each pass ???
    	unsigned int hashResult = NULL;
    	
    	// hash new_node->word
    	hashResult = hash(new_node->word);
    	
    	// insert node into hash table
    	if(hashtable[hashResult] == NULL)
    	{
    		// no linked list exists
    		node *ptr_node = &new_node;
    		hashtable[hashResult]->ptr_node;
    		new_node->next = NULL; 
    	}
    	else
    	{
    		// linked list exists, so add to beginning of list
    		new_node->next = &ptr_node;
    		hashtable[hashResult] ->ptr_node;
    		ptr_node = &new_node;
    	}
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
