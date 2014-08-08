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

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
  || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#define get16bits(d) (*((const uint16_t *) (d)))
#endif

#if !defined (get16bits)
#define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                       +(uint32_t)(((const uint8_t *)(d))[0]) )
#endif


// create data structures
    typedef struct node
    {
    	char entry[LENGTH + 1];
    	struct node *next;
    }node;
 
 	typedef struct hashtable
 	{
 		int size;
 		node **first;
 	}hashtable;
 
FILE* dptr = NULL;

unsigned int wordCount = 0;
int insertCount;
int hashCheck;
int hashResult;
int collisionCount = 0;
hashtable *hashTable;

uint32_t SuperFastHash (const char * data, int len) 
{
	uint32_t hash = len, tmp;
	int rem;

		if (len <= 0 || data == NULL) return 0;

		rem = len & 3;
		len >>= 2;

		/* Main loop */
		for (;len > 0; len--) {
		    hash  += get16bits (data);
		    tmp    = (get16bits (data+2) << 11) ^ hash;
		    hash   = (hash << 16) ^ tmp;
		    data  += 2*sizeof (uint16_t);
		    hash  += hash >> 11;
		}

		/* Handle end cases */
		switch (rem) {
		    case 3: hash += get16bits (data);
		            hash ^= hash << 16;
		            hash ^= ((signed char)data[sizeof (uint16_t)]) << 18;
		            hash += hash >> 11;
		            break;
		    case 2: hash += get16bits (data);
		            hash ^= hash << 11;
		            hash += hash >> 17;
		            break;
		    case 1: hash += (signed char)*data;
		            hash ^= hash << 10;
		            hash += hash >> 1;
		}

		/* Force "avalanching" of final 127 bits */
		hash ^= hash << 3;
		hash += hash >> 5;
		hash ^= hash << 4;
		hash += hash >> 17;
		hash ^= hash << 25;
		hash += hash >> 6;

		return hash;
}

hashtable *createHashTable(int size)
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
/*
    unsigned int hashValue;
    hashValue = 0;
    for(; *word != '\0'; word++) 
    {
    	hashValue = *word + (hashValue << 5) - hashValue;
    }

	hashResult = hashValue % wordCount;
	*/
	int len = strlen(word);
    hashResult = SuperFastHash(word, len) % wordCount;
    return hashResult; 
}

bool check(const char* word)
{
	//printf("word = %s\n", word);	
	hashResult = hash(word);
	if(hashResult < 0 || hashResult > wordCount ){return false;}
	node *currentNode = hashTable->first[hashResult];
	//printf("%s is the currentNode->entry @ %i: \n", currentNode->entry, hashResult);
    for (int loopEscape = 0; currentNode != NULL; loopEscape++)
    {
    	// strcmp entry and word
    	int comp = strcmp(word, currentNode->entry);
    	//printf("strcmp result = %i\n", comp);
    	if (comp == 0)  
    	{
    		return true;
    	}
    	if(comp == 1 || comp == -1)
    	{
    		currentNode = currentNode->next;
    	}
    	if (loopEscape > 10)
    	{
    		printf("Had to use loopEscape with %s\n", currentNode->entry);
    		return false;
    	}
    	else return false;
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
//    printf("Creating hashtable...\n")

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
		//char word[LENGTH + 1];
		int index = 0;

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
         
        
		/*int j = strlen(temp);
		for (int i = 0; i < j; i++)
		{
			if (temp[i] == '\n')
			{
				temp[i]  = '\0';
			}
		}*/
		
		
		//printf("%s = ", temp);						// for debugging
    	
		hashResult = hash (new_node->entry);
		//hash(temp);
		//printf("%s hashes to %i\n", temp, hashResult);
		//printf("%s = %i\n", new_node->entry, hashResult);
		
		if (hashTable->first[hashResult] == NULL)
		{
			new_node->next =  hashTable->first[hashResult];
			hashTable->first[hashResult] = new_node;
			//printf("%d\n", hashResult);
			insertCount++;
			//printf("Successful entry!\n");
		}
	//	printf("%s\n",hashTable->first[hashResult].entry);
	else if (hashTable->first[hashResult] != NULL) 
		{
				new_node->next =  hashTable->first[hashResult];
				hashTable->first[hashResult] = new_node;
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
	//printf("Checking size...");
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
	//printf("%s is the currentNode->entry @ %i: \n", currentNode->entry, hashResult);
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
