#include <stdio.h>
#include <stdlib.h>

// frobcmp function
// return values may be 0, 1, or -1
int frobcmp(const char * a, const char * b)
{
  for(; *a == *b; a++, b++)
    { if(*a == ' ') return 0;}
  if((*a^0x2A) > (*b^0x2A) || (*b == ' '))
    return 1;
  else
    return -1;
}

// frobcmpvoid function
// return values may be 0, 1, or -1
int frobcmpvoid(const void* c, const void * d)
{
  const char* a = *(const char**)c;
  const char* b = *(const char**)d;
  return frobcmp(a, b);
}

// mallocerrer function
int mallocerror(void)
{
  fprintf(stderr, "Error in Memory Allocation");
  exit(1);
}

// readerror function
int readerror(void)
{
  if(ferror(stdin))
    {
      fprintf(stderr, "Error while reading file");
      exit(1);
    }
}

// writeerror function
int writeerror(void)
{
  fprintf(stderr, "Error while writing file");
  exit(1);
}

// sfrob function
int sfrob(void)
{
  char** words = malloc(sizeof(char*));
  char* word = malloc(sizeof(char));
  int k = 0, i = 0, sizeword = 0, sizewords = 0;
  char c;
  
  while(c=getchar())
    {
      readerror();
      if(c != EOF && c != ' ' && c != '\n')
	{
	  word[sizeword] = c;
	  sizeword++;
	  char *tmp = word;
	  if((word = realloc(word, (sizeword+1)*sizeof(char))) == NULL)
	    {
	      free(tmp);
	      mallocerror();
	    }
	}
      if((c == ' ' || c == EOF || c == '\n') && sizeword > 0)
	{
	  word[sizeword] = ' ';
	  words[sizewords] = word;
	  sizewords++;
	  if((words = realloc(words, (sizewords+1)*sizeof(char*))) == NULL) mallocerror();
	  sizeword = 0;
	  word = malloc(sizeof(char));
	}
      if(c == EOF)
	break;
    }
  qsort(words, sizewords, sizeof(char*), frobcmpvoid);

  for (; i < sizewords; i++)
    {
      for(k = 0;;k++)
	{
	  if(putchar(words[i][k]) == EOF) writeerror();
	  if(words[i][k] == ' ')
	    break;
        }
    }

  int j = 0;
  for(; j < sizewords; j++)
    {
      free(words[j]);
    }
  free(words);
  return 0;
}

// main routine
// just runs sfrob(), the main function of the file
int main(void)
{
  return sfrob();
}
