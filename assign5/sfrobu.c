#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>

int f_flag = 0;

int frobcmp(char const* a, char const* b) {
    int index = 0;
    char aChar = a[0];
    char bChar = b[0];
    char pattern = 42;  // 00101010
    
    // Loop through each character in a and b
    while (aChar != ' ' && bChar != ' ') {
        // Deobfuscate each character
        char deObfsA = aChar ^ pattern;
        char deObfsB = bChar ^ pattern;
        
        if (f_flag) {
            unsigned char unsignedA = (unsigned char) deObfsA;
            unsigned char unsignedB = (unsigned char) deObfsB;
            deObfsA = (char) toupper((int)unsignedA);
            deObfsB = (char) toupper((int)unsignedB);
        }
        // If one is less than the other, return that one
        if (deObfsA < deObfsB) {
            return -1;
        }
        else if (deObfsB < deObfsA) {
            return 1;
        }

        index++;
        aChar = a[index];
        bChar = b[index];
    }

    // Strings are same up to this point: return string with shortest length
    if (aChar == ' ') {
        if (bChar == ' ') {
            // a and b are same length & same string
            return 0;
        }
        // a is shorter than b 
        return -1;
    }
    // b is shorter than a
    return 1;
}

// Wrapper for frombcmp so it can be passed into qsort
int frobcmpWrapper(const void * a , const void * b) {
    const char* aStr = *(const char**) a;
    const char* bStr = *(const char**) b;
    return frobcmp(aStr, bStr);
}

int main(int argc, char *argv[]) {
    // Check arguments
    if (argc != 1 && argc != 2) {
        fprintf(stderr, "Error: Wrong number of operands\n");
        exit(1);
    }
    else if (argc == 2) {
        if (!strcmp(argv[1], "-f")) {
            //printf("-f flag\n");
            f_flag = 1;
        }
        else {
            fprintf(stderr, "Error: operand must be -f\n");
            exit(1);
        }
    }

    // Check if errors reading in from stdin
    if (ferror(stdin)) {
        fprintf(stderr, "Error reading stdin\n");
        exit(1);
    }

    struct stat inputBuff;
    if (fstat(STDIN_FILENO, &inputBuff) < 0) {
        fprintf(stderr, "Error with reading file in.\n");
        exit(1);
    }

    //char** strings;
    int maxBufferSize = 0;
    int bufferCount = 0;
    if (S_ISREG(inputBuff.st_mode)) {
        maxBufferSize = inputBuff.st_size;
    }    

    // Read stdin into buffer
    char* buffer = (char*) malloc(sizeof(char) * maxBufferSize);
    if (buffer == NULL) {
        fprintf(stderr, "Error with memory allocation\n");
        exit(1);
    }

    if (read(STDIN_FILENO, buffer, inputBuff.st_size) < 0) {
        free(buffer);
        fprintf(stderr, "Error reading stdin into buffer!");
        exit(1);
    }

    // Initialize array of strings
    char** strings= (char **) malloc(sizeof(char*));
    
    // Check for memory allocation errors
    if (strings == NULL) {
        free(buffer);
        fprintf(stderr, "Error with memory allocation\n");
        exit(1);
    }       

    int numStr = 0;

    // Initialize current string
    int maxStrSize = 20;
    char* str = (char*) malloc(maxStrSize*sizeof(char));
    if (str == NULL) {
        free(buffer);
        free(strings);
        fprintf(stderr, "Error with memory allocation\n");
        exit(1);
    }

    int strlen = 0;

    int ch;
    bool noEOF = 1;
    // Read words from buffer
    while (noEOF) {
        if (bufferCount < maxBufferSize) {
            ch = buffer[bufferCount];
            bufferCount++;
            //printf("hi");
        }
        else {
            // Read from stdin char by char
            int result = read(STDIN_FILENO, &ch, 1);
            if (result == -1) {
                free(buffer);
                for (int i = 0; i < numStr; i++)
                    free(strings[i]);
                free(strings);
                free(str);
                fprintf(stderr, "Error reading stdin\n");
                exit(1);
            }
            if (!result) {
                noEOF = 0;
            }
        }
          
        if (!noEOF) break;
    
        char c = (char) ch;
        // Check if need to reallocate space
        if (strlen == maxStrSize) {
            maxStrSize += 20;
            char* temp = (char*) realloc(str, maxStrSize*sizeof(char));
            if (temp == NULL) {
                free(buffer);
                for (int i = 0; i < numStr; i++)
                    free(strings[i]);
                free(strings);
                free(str);
                fprintf(stderr, "Error with memory allocation\n");
                exit(1);
            }
            str = temp;
        }  
        // Add current character to current string
        str[strlen] = c;
        strlen++;

        // If current char is space
        if (c == ' ') {
            // Add current string to array of strings
            strings[numStr] = str;
            numStr++;
            
            char** temp = (char**) realloc(strings, (numStr+1)*sizeof(char*));
            if (temp == NULL) {
                free(buffer);
                for (int i = 0; i < numStr; i++) 
                    free(strings[i]);
                free(strings);
                fprintf(stderr, "Error with memory allocation\n");
                exit(1);
            }
            strings = temp;
            // Start a new current string
            maxStrSize = 20;
            str = (char*) malloc(maxStrSize*sizeof(char));
            strlen = 0;
        }
    }
    
    // Check if need to append extra space
    if (strlen != 0) {
        // Check if need to reallocate
        if (strlen == maxStrSize) {
            maxStrSize += 20;
            char* temp = (char*) realloc(str, maxStrSize*sizeof(char));
            if (temp == NULL) {
                free(buffer);
                for (int i = 0; i < numStr; i++)
                    free(strings[i]);
                free(strings);
                free(str);
                fprintf(stderr, "Error with memory allocation\n");
                exit(1);
            }
            str = temp;
        }
        // Append space
        str[strlen] = ' ';
        strlen++;
        strings[numStr] = str;
        numStr++;
    }

    // Get pointer for frobcmp to pass into qsort
    int (*frobcmpPtr)(const void *, const void *) = &frobcmpWrapper; 
    
    qsort(strings, numStr, sizeof(void *), frobcmpPtr);
    
    // Print out sorted list
    for (int i = 0; i < numStr; i++) {
        int j = 0;
        while (strings[i][j] != ' ') {
            write(STDOUT_FILENO, &strings[i][j], 1);
            j++;
        }
        write(STDOUT_FILENO, " ", 1);
    }

    // Deallocate memory
    for (int i = 0; i < numStr; i++) {
        free(strings[i]);
    }
    free(strings);
    free(buffer);
    exit(0);
}
