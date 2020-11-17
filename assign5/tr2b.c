#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

bool checkDuplicates(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        char c = str[i];
        for (int j = i+1; j < strlen(str); j++) {
            if (str[j] == c) {
                return 1;
            }
        }
    }
    return 0;
}

// translates c from the letter in from to corresponding letter in to. if not in from, returns c
char transliterate(char c, char* from, char* to) {
    for (int i = 0; i < strlen(from); i++) {
        if (from[i] == c) {
            return to[i];
        }
    }

    return c;
}

int main(int argc, char *argv[]) {
    // check for # of arguments
    if (argc != 3) {
        fprintf(stderr, "tr2b: number of operands should be 2\n");
        exit(1);    
    }

    // check if FROM or TO are empty
    if ((strlen(argv[1]) == 0) || (strlen(argv[2]) == 0)) {
        fprintf(stderr, "tr2b: FROM and TO cannot be empty\n");
        exit(1);
    } 

    // check for same length && if empty
    if (strlen(argv[1]) != strlen(argv[2])) {
        fprintf(stderr, "tr2b: from and to aren't the same length\n");
        exit(1);
    }

    //check for duplicate bytes
    if (checkDuplicates(argv[1])) {
        fprintf(stderr, "tr2b: from has duplicate bytes\n");
        exit(1);
    }

    int c = getchar();
    while (c != EOF) {
        putchar(transliterate((char)c, argv[1], argv[2]));
        c = getchar();
    }
}
