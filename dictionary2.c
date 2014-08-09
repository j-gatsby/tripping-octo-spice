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
#define SEED 5381

// create data structures
    typedef struct node
    {
    	char entry[LENGTH + 1];
    	struct node *next;
    }node;

 
FILE* dptr = NULL;
unsigned int wordCount;
char *table[][LENGTH +1];

 // Swap arguments' values.
 void swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

unsigned int hash(const char* word)
{

    unsigned int hashValue = SEED;

    for(; *word != '\0'; word++) 
    {
    	hashValue = *word + (hashValue << 5)  - hashValue;
    } 

	unsigned int hashResult = hashValue % wordCount;

    return hashResult; 
}

bool check(const char* word)
{
	unsigned int len = strlen(word);
	char dest[len];
	
	strcpy(dest, word);
	for (int i = 0; i < len; i++)
	{
		dest[i] = tolower(dest[i]);
	}
	unsigned int hashResult = hash(dest);
	if(hashResult > wordCount ) return false;
	node *currentNode = hashTable->first[hashResult];

    while (currentNode != NULL)
    {
    	// strcmp entry and word
    	if((strcmp(dest, currentNode->entry)== 0)) return true;
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
    
    char buffer[(sizeof(dptr))];
    int chars_read = 0;
    // check number of words in dptr
     // until the end of the file is reached...
    while (feof(dptr) == 0)	
    { 	
			// read card to buffer, one byte at a time
			fread(&buffer[chars_read], sizeof(char), 1, dptr);
			if (buffer[isupper(chars_read)]) 
			{
				buffer[tolower(chars_read)];
			}
			// if the end of a word is found
			else if (buffer[chars_read] == '\n')
			{
				buffer[chars_read] = '\0';
				wordCount++;
				memset(buffer, '\0', sizeof(chars_read));
				chars_read = 0;
			}
			// if the end of the file is found
			else if (buffer[chars_read] == EOF)
			{
				printf("The End of the File has been reached.\n ");
    		}
    }
    
	hash each word in table array and swap its position current to hash

   // load words from dictionary into hashTable
   for (int i = 0; i < wordCount; i++)
    {
    	node *new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    		return false;
    	}

		unsigned int index = 0;

    	// read in each dictionary entry
  		for (int c = fgetc(dptr); c != EOF; c = fgetc(dptr))
    	{
            	// append character to word
            new_node->entry[index] = tolower(c);
           
	            // terminate current word
		       if (new_node->entry[index] == '\n')
				{
					new_node->entry[index] = '\0';
    	        	break;
    	      	}
    	     	else if (new_node->entry[index]== EOF)
				{
					free(new_node->entry);
					fclose(dptr);
					return true;
				}
				else index++;
			}
		new_node->next = NULL;
	
		unsigned int hashResult = hash (new_node->entry);
	
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
    for (int i = 0; i < wordCount; i++)
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
