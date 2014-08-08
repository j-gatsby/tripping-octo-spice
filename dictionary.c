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
 
 	typedef struct hashtable
 	{
 		unsigned int size;
 		node **first;
 	}hashtable;
 
FILE* dptr = NULL;

unsigned int wordCount = 0;
unsigned int insertCount;
unsigned int hashCheck;
unsigned int hashResult;
unsigned int collisionCount = 0;
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
    for(int i=0; i<size; i++) new_table->first[i] = NULL;
    
    /* Set the table's size */
    new_table->size = size;

    return new_table;
}

unsigned int hash(const char* word)
{

    unsigned int hashValue = SEED;

    for(; *word != '\0'; word++) 
    {
    	hashValue = *word + (hashValue << 5)  - hashValue;
    } 

	hashResult = hashValue % wordCount;

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
	hashResult = hash(dest);
	if(hashResult > wordCount ) return false;
	node *currentNode = hashTable->first[hashResult];

    while (currentNode != NULL)
    {
    	// strcmp entry and word
    	if(strcmp(dest, currentNode->entry) == 0) return true;
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
    
    char buffer[LENGTH + 1];
    int chars_read = 0;
    
    // check number of words in dptr
     // until the end of the file is reached...
    while (feof(dptr) == 0)	
    { 	
		for (chars_read =0;  chars_read < LENGTH + 1; chars_read++)
		{
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
			// if the end of the file is found
			else if (buffer[chars_read] == EOF)
			{
				printf("The End of the File has been reached.\n ");
    		}
    	}
    }
    
    // reset file position indicator to beginning of dptr
    fseek(dptr, 0, SEEK_SET);
    
    // index dictionary and store in memory
    int wordsEntered = 0;
  
    //initialize hashtable
    hashTable = createHashTable(wordCount);

   // load words from dictionary into hashTable
   for (int i = 0; i < wordCount; i++)
    {
    	node *new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    	  	printf("new_node was NULL\n");							// for debugging
    		return false;
    	}

		unsigned int index = 0;

    	// spell-check each word in text
    for (int c = fgetc(dptr); c != EOF; c = fgetc(dptr))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            new_node->entry[index] = tolower(c);
            index++;

            // ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // consume remainder of alphabetical string
                while ((c = fgetc(dptr)) != EOF && isalpha(c));

                // prepare for new word
                index = 0;
                
            }
        }

        // ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // consume remainder of alphanumeric string
            while ((c = fgetc(dptr)) != EOF && isalnum(c));

            // prepare for new word
            index = 0;
        }

        // we must have found a whole word
        else if (index > 0)
        {
            // terminate current word
             new_node->entry[index] = '\0';

            // update counter
            wordsEntered++;

            // prepare for next word
            index = 0;
            break;
          }
         else if (new_node->entry[index]== EOF)
		{
			free(new_node->entry);
			fclose(dptr);
			return true;
		}
	}
		new_node->next = NULL;

		hashResult = hash (new_node->entry);

		if (hashTable->first[hashResult] == NULL)
		{
			hashTable->first[hashResult]= new_node;
			new_node->next = NULL;
			insertCount++;
		}
	
	else if (hashTable->first[hashResult] != NULL) 
		{
				new_node->next = hashTable->first[hashResult];
				hashTable->first[hashResult] = new_node;
				insertCount++;
				collisionCount++;
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
    
   printf("Words counted: %i\n", wordCount);
   printf("Words inserted: %i\n", insertCount);
   printf("Hash collisions: %i\n", collisionCount);
    
  	return true;  
}
