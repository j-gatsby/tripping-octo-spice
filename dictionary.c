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
    	char *entry;
    	struct node *next;
    }node;
 
 	typedef struct hashtable
 	{
 		int size;
 		node **table;
 	}hashtable;
 
FILE* dptr = NULL;

unsigned int wordCount = 0;
int insertCount;
int hashCheck;
int hashResult;
hashtable *hashTable;

hashtable *createHashTable(int size)
{
    hashtable *new_table;
    
    if (size<1) return NULL; /* invalid size for table */

    /* Attempt to allocate memory for the table structure */
    if ((new_table = malloc(sizeof(node))) == NULL) {
        return NULL;
    }
    
    /* Attempt to allocate memory for the table itself */
    if ((new_table->table = malloc(sizeof(node *) * size)) == NULL) {
        return NULL;
    }

    /* Initialize the elements of the table */
    for(int i=0; i<size; i++) new_table->table[i] = NULL;

    /* Set the table's size */
    new_table->size = size;

    return new_table;
}

unsigned int hash(const char* word)
{
    unsigned int hashValue;
    
    /* we start our hash out at 0 */
    hashValue = 0;

    /* for each character, we multiply the old hash by 31 and add the current
     * character.  Remember that shifting a number left is equivalent to 
     * multiplying it by 2 raised to the number of places shifted.  So we 
     * are in effect multiplying hashval by 32 and then subtracting hashval.  
     * Why do we do this?  Because shifting and subtraction are much more 
     * efficient operations than multiplication.
     */
    for(; *word != '\0'; word++) 
    {
    	hashValue = *word + (hashValue << 5) - hashValue;
    }

    /* we then return the hash value mod the hashtable size so that it will
     * fit into the necessary range
     */
    return hashValue % hashTable->size;
}

bool check(const char* word)
{
	hashCheck = hash(word);
	if (hashTable->table[hashCheck] == NULL)
	{
		return false;
	}
	node *currentNode = hashTable->table[hashCheck];
    for(; currentNode != NULL; currentNode = currentNode->next)
    {
    	// strcmp entry and word
    	int sameString;
    	sameString = strcmp(word, currentNode->entry);
    	
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
    printf("wordCount is %d\n", wordCount);					// for debugging
    
    // reset file position indicator to beginning of dptr
    fseek(dptr, 0, SEEK_SET);
    printf("Reset file position indicator to start of file, and reset EOF flag.\n");
    
    // index dictionary and store in memory
    char *dict[wordCount];
    printf("Initialized dictionary array.\n");
    int wordsEntered = 0;
    
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
				buffer[chars_read] = '\0';
				dict[wordsEntered] = buffer;
				// increment counter for words entered in dictionary[]
				wordsEntered++;
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
    printf("%d words entered into dictionary[]\n", wordsEntered);
    
    //initialize hashtable
    hashTable = createHashTable(wordCount);
    printf("Creating hashtable...\n");
     
   // load words from dictionary into hashTable
   for (int i = 0; i < wordCount; i++)
    {
    	 // malloc new node* n for each word
    	node* new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    	  	printf("new_node was NULL\n");							// for debugging
    		return false;
    	}
    	
		strcpy(new_node->entry, dict[i]);
		
		printf("%s = ", new_node->entry);							// for debugging
    
		// hash new_node->entry
		hashResult = hash(new_node->entry);
		new_node->next = hashTable->table[hashResult];
		hashTable->table[hashResult] = new_node;
		printf("%d\n", hashResult);
		insertCount++;
		printf("Successful entry!\n");
		
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
	if (hashTable == NULL)  return false;
	
    node *previousNode;
    node *currentNode;
    
    for(int i = 0; i < wordCount; i++)
    {
    	currentNode = hashTable->table[i];
    	while (currentNode != NULL)
    	{
    		previousNode = currentNode;
    		currentNode = currentNode->next;
    		free(previousNode->entry);
    		free(previousNode);
    	}
    }
    free(hashTable->table);
    free(hashTable);
  	return true;  
}
