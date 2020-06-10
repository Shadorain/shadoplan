// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Files
#include "config.h"
#include "options.h"
#include "shadomenu.h"

typedef struct Entry Entry;
struct Rules {
    char VERSION[3];
    int priority;
    char title[96];
    char desc[512];
    char cat[36];
    char due[8];

    // Allows for 2048 Categories of size 36
    char cats[36][512];
};

// Function Declaration
void add (struct Rules r);
void list (char method[]);

// -a/--add option, adds new todo to list
void add (struct Rules r) {
    FILE *td;
    FILE *ct;
    char buf[36];
    char *bufcat;

    // Opens and writes data to file
    td = fopen(tdpath, "a");
    fprintf(td, "\"%s\",\"%s\",\"%d\",\"%s\",\"%s\"\n",r.title,r.desc,r.priority,r.cat,r.due); 
    // Close Opened File
    fclose(td);

    // Check categories file if added category exists, if not add
    ct = fopen(categories, "a+");
    while (fgets(buf, sizeof(buf), ct) != 0) {
        if (strcmp(buf,r.cat))
            fprintf(ct, "%s\n",r.cat);
    }
    /* for (int i=0; i<3; i++) { */ 
    /*     bufcat = strtok(buf, "\",\""); */
    /* } */
    /* printf("%s",bufcat); */
    fclose(ct);
}

// -l/--list option, displays list in specified way
void list (char method[]) {
    FILE *fp;
    char buf[652];

    // Opens and reads data
    fp = fopen(tdpath, "r");

    if (!strcmp(method,"plain"))
        printf("Title, Description, Priority, Category, Due-Date\n");

    while (fgets(buf, sizeof(buf), fp) != 0) {
        /* fgets(buf, 652, fp); */
        if (!strcmp(method,"tree")) {
            
        } else if (!strcmp(method,"plain")) {
            printf("%s",buf);
        } else if (!strcmp(method,"interactive")) {
            
        } else if (!strcmp(method,"date")) {
            
        } else if (!strcmp(method, "priority")) {

        }
    }
    // Close Opened File
    fclose(fp);
}

int main (int argc, char *argv[]) {
    struct Rules r;
    int TUI = 1;
    strncpy(r.VERSION, "1.0", 3);
    // Checks if options given, if not then runs Tui
    if (argc==2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) { // Prints Version
        printf("shadoplan-%s\n", r.VERSION);
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
                r.priority=0;
            else if (argc < 7) { //If category empty
                r.priority=atoi(argv[5]);
                strcpy(r.cat,"");
            } else if (argc < 8) { //If Due Date empty
                r.priority=atoi(argv[5]);
                strcpy(r.cat, argv[6]);
                strcpy(r.due,"");
            } else if (argc > 8)
                usage();
            else {
                r.priority=atoi(argv[5]);
                strcpy(r.cat, argv[6]);
                strcpy(r.due, argv[7]);
            }
            if(!(r.priority >= 0 && r.priority < 10)) {
                printf("Priority must be a digit (0-9)\n");
                usage();
            }
            strcpy(r.title, argv[3]);
            strcpy(r.desc, argv[4]);
            add(r);
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
            else if (argc==4 && !strcmp(argv[3], "priority"))
                list("priority");
            else
                helpText('t');
            exit(0);
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
