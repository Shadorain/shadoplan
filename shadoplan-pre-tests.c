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
    char name[96];
    char descstr[512];
    
};

// Function Declaration
void add (char title[], char desc[], int priority, char category[], char due[]);

void add (char title[], char desc[], int priority, char category[], char due[]) {

}

int main (int argc, char *argv[]) {
    char VERSION[3] = "1.0";
    int priority, TUI = 1;
    char title[96],desc[512],cat[30],due[7];
    // Checks if options given, if not then runs Tui
    for (int i = 1; i < argc; i++) {
        printf(argc);
        if (!strcmp(argv[i], "-v")) // Prints Version
            printf("shadoplan-%s", VERSION);
        else if (!strcmp(argv[i], "-h")) { // General Help Menu
            printf("Run 'man shadoplan' for full documentation.\n");
            usage();
        } else if (!strcmp(argv[++i], "-h")) // Depth Help Menu
            helpText(*argv);
        else if (!strcmp(argv[++i], "-a")) { //|| (!strcmp(argv[++i], "--add"))) { // Add Option
            strcpy(title, argv[2+i]);
            printf(title,"\n");
            strcpy(desc, argv[3+i]);
            printf(desc,"\n");
            priority=atoi(argv[4+i]);
            printf(priority,"\n");
            strcpy(cat, argv[5+i]);
            printf(cat,"\n");
            strcpy(due, argv[6+i]);
            printf(due,"\n");
            if (!strcmp(argv[2+i],"") || !strcmp(argv[3+i], "")) //If Title or Description empty
                usage();
            if (!strcmp(argv[4+i],"")) //If priority empty
                priority=0;
            else if (!strcmp(argv[5+i],"")) //If category empty
                strcpy(cat,"");
            else if (!strcmp(argv[6+i],"")) //If Due Date empty
                strcpy(due,"");
            add(title, desc, priority, cat, due);
        } else if (i + 1 == argc)
            usage();
        else
            usage();
    }

    if (TUI==1) {
        int STATE = shadoplanCurses(); //Start NCurses
        if (STATE==1) {
            printf("Exit Code 1, NCurses Menu has crashed. Please restart program.");
        }
    }
    return 0;
}
