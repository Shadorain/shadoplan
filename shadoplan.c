#include <stdio.h>
#include <string.h>
#include <config.h>

typedef struct Entry Entry;
struct Entry {
    char name[100];
    char descstr[512];
    
};

/* void add */

void addDesc(Entry *e) {}

int main () {

    int STATE = shadoplanCurses(); //Start NCurses
    if (STATE==1) {
        printf("Exit Code 1, NCurses Menu has crashed. Please restart program.");
    }

    return 0;
}
