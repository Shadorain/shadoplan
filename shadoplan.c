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
void list (char method[]);

// -a/--add option, adds new todo to list
void add (char title[], char desc[], int priority, char category[], char due[]) {
    FILE *fp;
    // Opens and writes data to file
    fp = fopen("/home/shadow/.todos/todo", "a");
    fprintf(fp, "\"%s\",%s,%d,\"%s\",\"%s\"\n",title,desc,priority,category,due); 
    // Close Opened File
    fclose(fp);
}

// -l/--list option, displays list in specified way
void list (char method[]) {
    /* FILE *fp; */
    // Opens and reads data
    /* fp = fopen("/home/shadow/.todos/todo", "r"); */
    
    printf("SUCCESS\n");
    printf("%s\n",method);
    exit(0);

    // Close Opened File
    /* fclose(fp); */
}

int main (int argc, char *argv[]) {
    char VERSION[3] = "1.0";
    int priority, TUI = 1;
    char title[96],desc[512],cat[30],due[7];
    // Checks if options given, if not then runs Tui
    if (argc==2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) { // Prints Version
        printf("shadoplan-%s\n", VERSION);
        exit(0);
    } else if (argc==2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) { // General Help Menu
        printf("Run 'man shadoplan' for full documentation.\n");
        usage();
    } else if (argc > 2) {
        if (!strcmp(argv[2], "-h") || !strcmp(argv[2], "--help")) { // Depth Help Menu
            char arg=*argv[1];
            helpText(arg);
        } else if (!strcmp(argv[2], "-a") || (!strcmp(argv[2], "--add"))) { // Add Option
            if (argc < 5) //If Title or Description empty
                usage();
            else if (argc < 6) //If priority empty
                priority=0;
            else if (argc < 7) { //If category empty
                priority=atoi(argv[5]);
                strcpy(cat,"");
            } else if (argc < 8) { //If Due Date empty
                priority=atoi(argv[5]);
                strcpy(cat, argv[6]);
                strcpy(due,"");
            } else if (argc > 8)
                usage();
            else {
                priority=atoi(argv[5]);
                strcpy(cat, argv[6]);
                strcpy(due, argv[7]);
            }
            if(!(priority >= 0 && priority < 10)) {
                printf("Priority must be a digit (0-9)\n");
                usage();
            }
            strcpy(title, argv[3]);
            strcpy(desc, argv[4]);
            add(title, desc, priority, cat, due);
            exit(0);
        } else if (!strcmp(argv[2], "-l") || (!strcmp(argv[2], "--list"))) { // List Option
            if (argc < 4)
                list("tree");
            else if (argc==4 && !strcmp(argv[3], "tree"))
                list("tree");
            else if (argc==4 && !strcmp(argv[3], "plain"))
                list("plain");
            else if (argc==4 && !strcmp(argv[3], "interactive"))
                list("interactive");
            else if (argc==4 && !strcmp(argv[3], "date"))
                list("date");
            else
                helpText('t');
        } else
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
