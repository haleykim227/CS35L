#include <unistd.h>
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

char transliterate(char c, char* from, char* to) {
    for (int i = 0; i < strlen(from); i++) {
        if (from[i] == c) {
            return to[i];
        }
    }

    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "tr2u: number of operands should be 2\n");
        exit(1);
    }
    
   if ((strlen(argv[1]) == 0) || (strlen(argv[2]) == 0)) {
        fprintf(stderr, "tr2u: FROM and TO cannot be empty\n");
        exit(1);
    }

    if (strlen(argv[1]) != strlen(argv[2])) {
        fprintf(stderr, "tr2u: from and to aren't the same length\n");
        exit(1);
    } 
    
    if (checkDuplicates(argv[1])) {
        fprintf(stderr, "tr2u: from has duplicate bytes\n");
        exit(1);
    }

    char c;
    while (read(STDIN_FILENO, &c, 1) > 0) {
        char transC = transliterate(c, argv[1], argv[2]);
        write(STDOUT_FILENO, &transC, 1);
    }
}


