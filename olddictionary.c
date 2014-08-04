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

// default dictionary
#define DICTIONARY "home/cs50/pset6/dictionaries/large"

// create data structures
    typedef struct node
    {
    	char entry[LENGTH + 1];
    	struct node *next;
    }node;
FILE* dptr = NULL;
node *hashtable = NULL;
node *cursor = NULL;
node *ptr_node = NULL;
unsigned int hashResult;
unsigned int hashCheck;
unsigned int wordCount = 0;
unsigned int insertCount = 0;

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
    	hashResult = hashValue % wordCount;
    	
    	printf("%s = %i\n", word, hashResult);			// for debugging
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
    	printf("dptr was NULL\n");							// for debugging
    	return false;
    }
    
    int buffer[LENGTH +1];
    int chars_read = 0;
    int chars_in_word;
    
    // check number of words in dptr
     // until the end of the file is reached...
    while (feof(dptr) == 0)	
    {
    	// clear out the buffer
    	memset(buffer, '\0', sizeof(buffer));
    	
		for (chars_read =0;  chars_read < LENGTH + 1; chars_read++)
		{
			// read card to buffer, one byte at a time
			fread(&buffer[chars_read], sizeof(char), 1, dptr);
			
			// if the end of a word is found
			if (buffer[chars_read] == '\n')
			{
				// increment counter for words in Dictionary
				wordCount++;
				// reset chars_in_word counter
				chars_in_word = 0;
			}
			// if the end of the file is found
			else if (buffer[chars_read] == EOF)
			{
				printf("The End of the File has been reached.\n ");
    		}
    	}
    }
    printf("wordCount is %d\n", wordCount);					// for debugging
    
    // reset file position indicator to beginning of dptr
    fseek(dptr, 0, SEEK_SET);
    
    // initialize hash table
    node *hashtable[wordCount];
    
    // set each element in hashtable to NULL
    for (int i = 0; i < wordCount; i++)
    {
    	hashtable[i] = NULL;
    }
    
    // iterate over dictionary file
    while (feof(dptr) == 0)
	{
	    // malloc new node* n for each word
    	node* new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    	  	printf("new_node was NULL\n");							// for debugging
    		return false;
    	}
    	
    	// use fscanf to read in one word at a time 
    	fscanf(dptr, "%s", new_node->entry);
    	
    	// convert word tolower
    	//new_node->word = tolower(new_node->word);
    	
    	// hash new_node->entry
    	hash(new_node->entry);
    	
    	// insert node into hash table
    	if(hashtable[hashResult] == NULL)
    	{
    		// no linked list exists
    		new_node->next = NULL;
    		hashtable[hashResult] = new_node; 
    		insertCount++;
    	}
    	else
    	{	// linked list exists, so add to beginning of list
    		new_node->next = hashtable[hashResult];
    		hashtable[hashResult] = new_node;
    		insertCount++;
    	}
    	/* original code to insert into hash table
    	// insert node into hash table
    	if(ptr_node == NULL)
    	{
    		// no linked list exists
    		new_node->next = ptr_node;
    		ptr_node = new_node; 
    		new_node->next = NULL;
    		insertCount++;
    	}
    	else
    		// linked list exists, so add to beginning of list
    		new_node->next = ptr_node;
    		ptr_node = new_node;
    	insertCount++;
    	*/
    }
    
    printf("Words inserted into hashtable: %i\n", insertCount);
    
    // fclose dictionary file
    fclose(dptr);
      	printf("dptr was closed successfully\n");							// for debugging
    // set bool to true
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return wordCount;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    int nodesFree=0;
    
    for(int i = 0; i < wordCount; i++)
    {
    		free(hashtable);
    		nodesFree++;
    }
  
  	if (nodesFree >= wordCount)
  	{
  		return true;
  	}  
  	else return false;
}
