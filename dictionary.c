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
//node *hashtable = NULL;
unsigned int wordCount = 0;
node *hashTable;
//node *cursor = NULL;
//node *ptr_node = NULL;
//node *hashPointer;
unsigned int hashResult;
unsigned int hashCheck;

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
/*************************************
 * Returns true if word is in dictionary else false.*
 **************************************/
bool check(const char* word)
{
    // TODO
    //char *temp = NULL;
     //strcpy(temp, word);
    //temp = strlwr(temp);
    hashCheck = hash(word);
    
    for(node *currentNode = &hashTable[hashCheck]; currentNode != NULL; currentNode = currentNode->next)
    {
    	// strcmp entry and word
    	int sameString;
    	sameString = strcmp(word, currentNode->entry);
    //	free(temp);
    	
    	if(sameString == 0)
    	{
    		return true;
    	}
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
    	printf("dptr was NULL\n");							// for debugging
    	return false;
    }
    
    int buffer[LENGTH +1];
    int chars_read = 0;
    int chars_in_word;
    int entry_to_node = 0;
   //node *new_node = NULL;
    
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
    printf("Reset file position indicator to start of file, and reset EOF flag.\n");
    
    // initialize hash table
    node *hashTable[wordCount];
    printf("Initialized hashTable\n");
    
    // set each element in hashtable to NULL
    for (int i = 0; i < wordCount; i++)
    {
    	hashTable[i] = NULL;
    }
    printf("Each element in hashTable set to NULL\n");
    
    /* iterate over dictionary file
    while (feof(dptr) == 0)
	{
	    // malloc new node* n for each word
    	node* new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    	  	printf("new_node was NULL\n");							// for debugging
    		return false;
    	}
    	*/
    	// temporary storage
    	//char *temp = NULL;
    	
    	// use fscanf to read in one word at a time, to temp storage
    	//fgetc(dptr, "%s", new_node->entry);
    	//printf("Reading in one string at a time....\n");
		
		// save copy of word read in from DICTIONARY    	
    	 //strcpy(new_node->entry, temp);
    	   // until the end of the file is reached...
    while (feof(dptr) == 0)	
    {
    	 // malloc new node* n for each word
    	node* new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    	  	printf("new_node was NULL\n");							// for debugging
    		return false;
    	}
    	
		for (chars_read =0;  buffer[chars_read] != '\n'; chars_read++)
		{
			// read card to buffer, one byte at a time
			fread(&buffer[chars_read], sizeof(char), 1, dptr);
			
				for (int i = 0; i < (chars_read); i++)
				{
					new_node->entry[i] = buffer[i];
				}
				new_node->entry[chars_read] = '\0';
				
				entry_to_node++;
				
    	
				// reset chars_in_word counter
				//chars_in_word = 0;
				printf("new_node->entry = %s\n", new_node->entry);
  			   printf("Entry Added to New Node = %d\n", entry_to_node);					// for debugging
    
    	
    			// hash new_node->entry
    			hashResult = hash(new_node->entry);
    			printf("Hashing the new entry...\n");
    	
    			// insert node into hash table
   			 	if(hashTable[hashResult] == NULL)
				{
					printf("Attempting to insert entry into hashTable...\n");
					// no linked list exists
					new_node->next = NULL;
					hashTable[hashResult] = new_node;
					//hashTable[hashResult]->entry = new_node->entry;
					insertCount++;
					printf("Successful entry!\n");
				}
				else
				{	// linked list exists, so add to beginning of list
					printf("Attempting to add entry on to LinkedList...\n"); 
					new_node->next = hashTable[hashResult]->next;
					hashTable[hashResult] = new_node;
					//hashTable[hashResult]->entry= new_node->entry;
					insertCount++;
					printf("Successful addition!\n");
				}
				
			}
		// clear out the buffer
   			 	memset(buffer, '\0', sizeof(buffer));
    }
    printf("Words inserted into hashtable: %i\n", insertCount);
    
    // fclose dictionary file
    fclose(dptr);
      	printf("dptr was closed successfully\n");							// for debugging
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
    int nodesFree=0;
    node *previousNode;
    node *currentNode;
    
    for(int i = 0; i < wordCount; i++)
    {
    	currentNode = &hashTable[i];
    	while (currentNode != NULL)
    	{
    		previousNode = currentNode;
    		currentNode = currentNode->next;
    		free(previousNode->entry);
    		free(previousNode);
    		nodesFree++;
    	}
    		free(&hashTable[i]);
    		nodesFree++;
    }
    free(hashTable);
  	if (hashTable == NULL)
  	{
  		printf("Total nodesFree = %d\n", nodesFree);
  		return true;
  	}  
  	else return false;
}
