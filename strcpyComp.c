/**************************************************************************
*strcpyComp.c
*
*Implements a timed comparision of different versions of strcpy.
****************************************************************************/
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#undef calculate
#undef getrusage

#define LENGTH 255
#define ITERATIONS 1000000

// prototype from HarvardCS/cs50/pset6/speller.c/byDavidMalan
double calculate(const struct rusage* b, const struct rusage* a);

// prototypes from StanfordCS/EducationLibrary/EssentialC/byNickParlante
void strcpy1(char dest[], const char source[]);
void strcpy2(char dest[], const char source[]);
void strcpy3(char dest[], const char source[]);
void strcpy4(char dest[], const char source[]);

int main(int argc, char* argv[])
{
    // check for correct number of args
    if (argc != 2)
    {
        printf("Usage: strcpyComp string\n");
        return 1;
    }
    
    int len = strlen(argv[1]);
    
    // check for correct length of string
    if (len > LENGTH)
    {
    	printf("String cannot be longer than %i letters.\n", LENGTH);
    	return 2;
    }
    
	 // set source and dest variables
    char source[len];
    char dest[len];
    
    // check string contains letters only & set source variable
    for (int i = 0; i < len; i++)
    {
    	int c = argv[1][i];
    	if (isalpha(c))
    	{
    		source[i] = c;
    		i++;
    	}
    	else if (c == '\0')
    	{
    		source[i] = '\0';
    		i++;
    	}
    	else
    	{
    		printf("String must contain letters only.\n");
    		return 3;
    	}
    }
  
    // structs for timing data
    struct rusage before;
    struct rusage after;

    // benchmarks
    double strcpy_time = 0.0, strcpy1_time = 0.0, strcpy2_time = 0.0, strcpy3_time = 0.0, strcpy4_time = 0.0;
    
     // execute standard version of strcpy from string.h
    getrusage(RUSAGE_SELF, &before);
    for (double i = 0; i < ITERATIONS; i++)
    {
    	strcpy(dest, source);
    }
    getrusage(RUSAGE_SELF, &after);
    
    // calculate time to execute strcpy
    strcpy_time = calculate(&before, &after);

    // execute strcpy1
    getrusage(RUSAGE_SELF, &before);
    for (double i = 0; i < ITERATIONS; i++)
    {
    	strcpy1(dest, source);
    }
    getrusage(RUSAGE_SELF, &after);

    // calculate time to execute strcpy1
    strcpy1_time = calculate(&before, &after);
    
    // execute strcpy2
    getrusage(RUSAGE_SELF, &before);
    for (double i = 0; i < ITERATIONS; i++)
    {
    	strcpy2(dest, source);
    }
    getrusage(RUSAGE_SELF, &after);

    // calculate time to execute strcpy2
    strcpy2_time = calculate(&before, &after);

	// execute strcpy3
    getrusage(RUSAGE_SELF, &before);
    for (double i = 0; i < ITERATIONS; i++)
    {
    	strcpy3(dest, source);
    }
    getrusage(RUSAGE_SELF, &after);

    // calculate time to execute strcpy3
    strcpy3_time = calculate(&before, &after);

    // execute strcpy4
    getrusage(RUSAGE_SELF, &before);
    for (double i = 0; i < ITERATIONS; i++)
    {
    	strcpy4(dest, source);
    }
    getrusage(RUSAGE_SELF, &after);

    // calculate time to execute strcpy4
    strcpy4_time = calculate(&before, &after);
    
	 printf("TIME IN strcpy1:	%.3f\n", strcpy1_time);
    printf("TIME IN strcpy2:	%.3f\n", strcpy2_time);
    printf("TIME IN strcpy3:	%.3f\n", strcpy3_time);
    printf("TIME IN strcpy4:	%.3f\n", strcpy4_time);
    printf("TIME IN strcpy:		%.3f\n", strcpy_time);
    
    // that's all folks
    return 0;
}
/**
 * Returns number of seconds between b and a.
 */
double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 888888888.8888888888;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                 (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                 (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}

// Unfortunately, a straight while or for loop won't work.
// The best we can do is use a while (1) with the test
// in the middle of the loop.
void strcpy1(char dest[], const char source[])
{	
	int i = 0;
	
	while (1)
	{
		dest[i] = source[i];
		if (dest[i] == '\0') break;		// we're done
		i++;
	}
}

// Move the assignment into the test
void strcpy2(char dest[], const char source[])
{
	int i = 0;
	
	while ((dest[i] = source[i]) != '\0')
	{
		i++;
	}
}

// 
void strcpy3(char dest[], const char source[])
{
	while ((*dest++ = *source++) != '\0');
}

// 
void strcpy4(char dest[], const char source[])
{
	while ((*dest++ = *source++));
}


