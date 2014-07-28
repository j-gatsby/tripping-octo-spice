/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "dictionary.h"

// create data structures
    typedef struct node
    {
    	char entry[LENGTH + 1];
    	struct node *next;
    }node;
    
node *hashtable = NULL;
node *cursor = NULL;
node *ptr_node = NULL;
unsigned int hashResult;
unsigned int hashCheck;
unsigned int count = 0;

unsigned int hash(const char* word)
{
	unsigned int hashValue = 0;
	
	for(; *word != '\0'; word++) 
	{
		hashValue = *word + (hashValue << 5) - hashValue;
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
    hashCheck = hash(word);
    
    node *cursor;
    cursor = &hashtable[hashCheck];
    cursor = ptr_node;
    
    while(cursor != NULL)
    {
    	// strcmp string and word
    	char str1[LENGTH + 1];
    	char str2[LENGTH + 1];
    	int sameString;
    	
    	strcpy(str1, word);
    	strcpy(str2, cursor->entry);
    	
    	sameString = strcmp(str1, str2);
    	
    	if(sameString == 0)
    	{
    		return true;
    	}
    	else
    	{
    		cursor = cursor->next;
    	}
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
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
    	node* new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    		return false;
    	}
    	
    	// use fscanf to read in one word at a time 
    	fscanf(dptr, "%s", new_node->entry);
    	
    	// convert word tolower
    	//new_node->word = tolower(new_node->word);
    	
    	// hash new_node->entry
    	hashResult = hash(new_node->entry);
    	
    	// insert node into hash table
    	if(ptr_node == NULL)
    	{
    		// no linked list exists
    		new_node->next = ptr_node;
    		ptr_node = new_node; 
    		new_node->next = NULL;
    	}
    	else
    	{
    		// linked list exists, so add to beginning of list
    		new_node->next = ptr_node;
    		ptr_node = new_node;
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
    int nodesFree=0;
    
    for(int i = 0; i < count; i++)
    {
    	node *cursor = &hashtable[i];
    	cursor = ptr_node;
    	
    	while(cursor != NULL)
    	{
    		node *temp = cursor;
    		cursor = cursor->next;
    		free(temp);
    		nodesFree++;
    	}
    }
  
  	if (nodesFree == count)
  	{
  		return true;
  	}  
  	else
  	{
    	return false;
    }
}
