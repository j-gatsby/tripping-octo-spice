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
 
 	typedef struct hashtable
 	{
 		unsigned int size;
 		node **first;
 	}hashtable;
 
FILE* dptr = NULL;
unsigned int wordCount;
hashtable *hashTable;

hashtable *createHashTable(unsigned int size)
{
    hashtable *new_table;
    
    if (size<1) return NULL; /* invalid size for table */

    /* Attempt to allocate memory for the table structure */
    if ((new_table = malloc(sizeof(node))) == NULL) {
        return NULL;
    }
    
    /* Attempt to allocate memory for the table itself */
    if ((new_table->first = malloc(sizeof(node *) * size)) == NULL) {
        return NULL;
    }

    /* Initialize the elements of the table */
    for(unsigned int i=0; i<size; i++) new_table->first[i] = NULL;
    
    /* Set the table's size */
    new_table->size = size;

    return new_table;
}

unsigned int hash(const char* word)
{
	//unsigned int wordTotal = wordCount;
    unsigned int hashValue = 101;

    for(; *word != '\0'; word++) 
    {
    	hashValue = *word + (hashValue << 6)  - hashValue;
    } 

	unsigned int hashResult = hashValue % wordCount;

    return hashResult; 
}

bool check(const char* word)
{
	unsigned int len = strlen(word);
	char dest[len];
	//unsigned int wordTotal = wordCount;
	
	strcpy(dest, word);
	for (unsigned int i = 0; i < len; i++)
	{
		// bitwise op instead of tolower
		dest[i] = dest[i] &~(1<<5);

	}
	unsigned int hashResult = hash(dest);
	
	node *currentNode = hashTable->first[hashResult];

    while (currentNode != NULL)
    {
    	// strcmp entry and word
    	if( (strcmp(dest, currentNode->entry)== 0)) return true;
    	else currentNode = currentNode->next;
    } 
    return false;
}

/******************************************************
 * Loads dictionary into memory.  Returns true if successful else false.*
 ******************************************************/
bool load(const char* dictionary)
{
    // open dictionary file
    FILE* dptr = fopen("DICTIONARY", "r");
    
    // ensure it worked
    if (dptr == NULL)
    {
    	return false;
    }
    
    char buffer[sizeof(LENGTH + 1)];
    int chars_read = 0;
    //unsigned int dptrWords = 0;
    
    // check number of words in dptr
     // until the end of the file is reached...
    while (feof(dptr) == 0)	
    { 	
		//for (chars_read =0;  chars_read < LENGTH + 1; chars_read++)
		//{
			// read card to buffer, one byte at a time
			fread(&buffer[chars_read], sizeof(char), 1, dptr);
			 
			// if the end of a word is found
			if (buffer[chars_read] == '\n')
			{
				// increment counter for words in Dictionary
				wordCount++;
				// clear out the buffer
    			memset(buffer, '\0', sizeof(buffer));
			}
	}
    
    // reset file position indicator to beginning of dptr
    fseek(dptr, 0, SEEK_SET);
  
    //initialize hashtable
    hashTable = createHashTable(wordCount);
	unsigned int wordTotal = wordCount;
   // load words from dictionary into hashTable
   for (unsigned int i = 0; i < wordTotal; i++)
    {
    	node *new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    		return false;
    	}

		unsigned int index = 0;

    	// read in each dictionary entry
  		for (unsigned int c = fgetc(dptr); c != EOF; c = fgetc(dptr))
    	{
            	// bitwise op instead of tolower
            new_node->entry[index] =  c =(c &~(1<<5));
           
	            // terminate current word
		       if (new_node->entry[index] == '\n')
				{
					new_node->entry[index] = '\0';
    	        	break;
    	      	}
				else index++;
		}
	
		unsigned int hashResult = hash(new_node->entry);
	
		if (hashTable->first[hashResult] == NULL)
		{
			hashTable->first[hashResult]= new_node;
			new_node->next = NULL;
		}
		
		else if (hashTable->first[hashResult] != NULL) 
		{
			new_node->next = hashTable->first[hashResult];
			hashTable->first[hashResult] = new_node;
		}
	}
	
    // fclose dictionary file
    fclose(dptr);

    // set bool to true
    return true;
}

/*********************************************************
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.*
 *********************************************************/
unsigned int size(void)
{
    return wordCount;
}

/*******************************************************
 * Unloads dictionary from memory.  Returns true if successful else false.
 *******************************************************/
bool unload(void)
{
	unsigned int wordTotal = wordCount;
    for (unsigned int i = 0; i < wordTotal; i++)
    {
    	node *currentNode = hashTable->first[i];

			while (currentNode != NULL)
			{
					node *previousNode = currentNode;
					currentNode = currentNode->next;
					free(previousNode);
			}
	}
	free(hashTable->first);
   free(hashTable);
    
  	return true;  
}
