// Libraries
#include <stdio.h>
#include <stdlib.h>
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

int main (int argc, char *argv[]) {
    char VERSION[3] = "1.0";
    int TUI = 1;
    // Checks if options given, if not then runs Tui
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-v")) { // Prints Version
            printf("shadoplan-%s", VERSION);
            exit(0);
        } else if (!strcmp(argv[i], "-h")) { // General Help Menu
            helpText("");
            exit(0);
        } else if (!strcmp(argv[++i], "-h")) { // Depth Help Menu
            helpText(*argv);
            exit(0);
        } else if (i + 1 == argc) {
            usage();
            exit(0);
        } else {
            usage();
            exit(0);
        }
    }

    if (TUI==1) {
        int STATE = shadoplanCurses(); //Start NCurses
        if (STATE==1) {
            printf("Exit Code 1, NCurses Menu has crashed. Please restart program.");
        }
    }
    return 0;
}
