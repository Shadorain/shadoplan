// Libraris
#include <stdio.h>
#include <string.h>
// Files
#include "config.h"
#include "options.h"
#include "shadomenu.h"

typedef struct Entry Entry;
struct Entry {
    char name[100];
    char descstr[512];
    
};

/* void add */

void addDesc(Entry *e) {}

int main () {
    usage();
    int TUI = 1;
    if (TUI==1) {
        int STATE = shadoplanCurses(); //Start NCurses
        if (STATE==1) {
            printf("Exit Code 1, NCurses Menu has crashed. Please restart program.");
        }
    }
    return 0;
}
