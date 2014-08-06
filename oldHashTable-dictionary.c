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
int collisionCount = 0;
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
     hashResult = hashValue % wordCount;
    return hashResult;
}

bool check(const char* word)
{
	unsigned int hashResult = hash(word);
	
	node *currentNode = NULL;
	node *ptrNode = NULL;
	ptrNode->next = hashTable->table[hashResult];
	currentNode = ptrNode->next;
	
	printf("%s is the currentNode->entry\n", currentNode->entry);
    for(int loopCount = 0; currentNode->entry != NULL; currentNode = currentNode->next)
    {
    	// strcmp entry and word
    	int comp = strcmp(word, currentNode->entry);
    	//printf("strcmp result = %i\n", comp);
    	if (comp == 0)  
    	{
    		return true;
    	}
    	else if(comp == 1)
    	{
    		loopCount++;
    	}
    	if (loopCount > 1)
    	{
    		return false;
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
   // printf("Reset file position indicator to start of file, and reset EOF flag.\n");
    
    // index dictionary and store in memory
    //char *dict[wordCount];
    //printf("Initialized dictionary array.\n");
    int wordsEntered = 0;
   
    //initialize hashtable
    hashTable = createHashTable(wordCount);
//    printf("Creating hashtable...\n");
     
   // load words from dictionary into hashTable
   for (int i = 0; i < wordCount; i++)
    {
    	 // malloc new node* n for each word
    	// printf("Attempting to allocate memory for new node...");
    	node *new_node = malloc(sizeof(node));
    	if (new_node == NULL)
    	{
    	  	printf("new_node was NULL\n");							// for debugging
    		return false;
    	}
    //	printf("done.\n");
   	
    	//char  temp[LENGTH + 1];
    	//char* fmt = "%[^\n]%*c";
    	//memset(temp, '\0', sizeof(temp));
    	//fgets(temp, sizeof(temp), dptr);
    	//fscanf(dptr,  fmt, temp);
 		//printf("fgets next word from dptr... \n");
		char word[LENGTH+1];
		int index = 0;
		int endWord = 0;
    	// spell-check each word in text
    for (int c = fgetc(dptr); endWord == 0 && c != EOF; c = fgetc(dptr))
    {
        // allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // append character to word
            word[index] = c;
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
            word[index] = '\0';

            // update counter
            wordsEntered++;

            // prepare for next word
            index = 0;
            endWord = 1;
        }
    }
		/*int j = strlen(temp);
		for (int i = 0; i < j; i++)
		{
			if (temp[i] == '\n')
			{
				temp[i]  = '\0';
			}
		}*/
		
		new_node->entry = word;
		new_node->next = NULL;
		
		//printf("%s = ", temp);						// for debugging
    
		hashResult = hash (new_node->entry);
		//hash(temp);
		//printf("%s hashes to %i\n", temp, hashResult);
		printf("%s = %i\n", new_node->entry, hashResult);
		node *head = hashTable->table[hashResult];
		if (head == NULL)
		{
			new_node->next = head;
			hashTable->table[hashResult] = new_node;
			//printf("%d\n", hashResult);
			insertCount++;
			//printf("Successful entry!\n");
		}
	//	printf("%s\n",hashTable->table[hashResult].entry);
	else if (head != NULL) 
		{
				new_node->next = head;
				hashTable->table[hashResult] = new_node;
				//printf("%d\n", hashResult);
				insertCount++;
				collisionCount++;
				//printf("Successful entry!\n");
		}
	}
    
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
    
    for(int i = 0; i < hashTable->size; i++)
    {
    	currentNode = hashTable->table[i];
    	while (currentNode != NULL)
    	{
    		previousNode = currentNode;
    		currentNode = currentNode->next;
    		//free(previousNode->entry);	//  causes program to crash 'Error in ./speller:  free(): invalid pointer: 0xbfa24fea'
    		free(previousNode);
    	}
    }
    free(hashTable->table);
    free(hashTable);
    
    printf("Words counted: %i\n", wordCount);
    printf("Words inserted: %i\n", insertCount);
    printf("Hash collisions: %i\n", collisionCount);
    
  	return true;  
}
